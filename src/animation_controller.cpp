#include "AnimationController.hpp"
#include "Log.hpp"
#include "Math.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"
#include "Time.hpp"

#include <algorithm>
#include <filesystem>
#include <pybind11/stl.h>

namespace fs = std::filesystem;

namespace kn
{
static std::vector<AnimationController*> _controllers;

AnimationController::AnimationController() { _controllers.push_back(this); }
AnimationController::~AnimationController() { std::erase(_controllers, this); }

void AnimationController::loadSpriteSheet(const std::string& filePath, const Vec2& frameSize,
                                          const std::vector<SheetStrip>& strips)
{
    if (frameSize.x <= 0 || frameSize.y <= 0)
    {
        throw std::invalid_argument("Frame size must be positive non-zero values");
    }
    if (strips.empty())
    {
        throw std::invalid_argument("No strips provided for sprite sheet");
    }

    if (renderer::_get() == nullptr)
        throw std::runtime_error(
            "Renderer not initialized; create a window before loading sprite sheets");

    const auto texPtr = std::make_shared<Texture>(filePath);
    const Vec2 size = texPtr->getSize();

    const int frameWidth = static_cast<int>(frameSize.x);
    const int frameHeight = static_cast<int>(frameSize.y);

    if (static_cast<int>(size.x) % frameWidth || static_cast<int>(size.y) % frameHeight)
        throw std::runtime_error(filePath + " dimensions are not divisible by frame dimensions");

    const int maxFramesPerRow = static_cast<int>(size.x) / frameWidth;

    for (size_t stripIndex = 0; stripIndex < strips.size(); ++stripIndex)
    {
        const auto& strip = strips[stripIndex];

        const std::string& name = strip.name;
        if (m_animMap.contains(name))
            throw std::runtime_error("Animation duplicate: " + name);

        if (strip.frameCount <= 0)
            throw std::invalid_argument("Frame count must be positive for strip: " + name);

        if (strip.frameCount > maxFramesPerRow)
            throw std::runtime_error("Frame count (" + std::to_string(strip.frameCount) +
                                     ") exceeds sprite sheet width for strip: " + name);

        const int y = static_cast<int>(stripIndex) * frameHeight;
        if (y + frameHeight > static_cast<int>(size.y))
            throw std::runtime_error("Strip index " + std::to_string(stripIndex) +
                                     " exceeds sprite sheet height");

        // Extract only the specified number of frames from this strip/row
        Animation newAnim;
        newAnim.fps = strip.fps;
        for (int i = 0; i < strip.frameCount; ++i)
        {
            const int x = i * frameWidth;
            const Frame frame{texPtr, {x, y, frameWidth, frameHeight}};
            newAnim.frames.emplace_back(frame);
        }

        m_animMap[name] = std::move(newAnim);
        m_currAnim = name;
    }
}

void AnimationController::set(const std::string& name)
{
    if (!m_animMap.contains(name))
        throw std::invalid_argument("Animation not found: " + name);

    m_currAnim = name;
}

void AnimationController::play(const std::string& name)
{
    set(name);
    rewind();
    resume();
}

void AnimationController::playFrom(const int frameIndex)
{
    const auto& [frames, _] = m_animMap.at(m_currAnim);

    if (frameIndex < 0 || frameIndex >= static_cast<int>(frames.size()))
        throw std::out_of_range("Frame index out of range: " + std::to_string(frameIndex));

    m_index = static_cast<double>(frameIndex);
    m_prevIndex = m_index;
    resume();
}

const Frame& AnimationController::getCurrentFrame() const
{
    const auto& [frames, _] = m_animMap.at(m_currAnim);

    // Clamp index to valid range to prevent out-of-bounds access
    const auto frameIndex = static_cast<size_t>(std::max(0.0, std::floor(m_index)));
    const auto safeIndex = std::min(frameIndex, frames.size() - 1);

    return frames.at(safeIndex);
}

void AnimationController::setPlaybackSpeed(const double speed)
{
    m_playbackSpeed = speed;
    if (speed == 0)
        pause();
}

double AnimationController::getPlaybackSpeed() const { return m_playbackSpeed; }

bool AnimationController::isLooping() const { return m_looping; }

void AnimationController::setLooping(const bool loop) { m_looping = loop; }

bool AnimationController::isFinished() const { return m_prevIndex > m_index; }

std::string AnimationController::getCurrentAnimationName() const { return m_currAnim; }

int AnimationController::getFrameIndex() const { return static_cast<int>(std::floor(m_index)); }

double AnimationController::getProgress() const
{
    const auto& [frames, _] = m_animMap.at(m_currAnim);
    const auto frameCount = static_cast<double>(frames.size());

    if (frameCount == 0.0)
        return 0.0;

    return m_index / frameCount;
}

void AnimationController::rewind()
{
    m_index = 0.0;
    m_prevIndex = 0.0;
}

void AnimationController::pause() { m_paused = true; }

void AnimationController::resume()
{
    if (m_playbackSpeed > 0.0)
        m_paused = false;
}

void AnimationController::update(const double delta)
{
    if (m_paused)
        return;
    if (m_currAnim.empty())
        return;

    const auto& [frames, fps] = m_animMap.at(m_currAnim);

    // Store previous index for loop detection
    m_prevIndex = m_index;

    m_index += delta * fps * m_playbackSpeed;
    const auto frameCount = static_cast<double>(frames.size());

    if (m_looping)
    {
        // Loop back to the beginning
        m_index = fmod(m_index + frameCount, frameCount);
        return;
    }

    // When not looping, clamp to the ends and pause
    if (m_index >= frameCount)
    {
        m_index = frameCount;
        pause();
    }
    else if (m_index < 0.0)
    {
        m_index = 0.0;
        pause();
    }
}

namespace animation_controller
{
void _tick()
{
    const double delta = time::getDelta();
    for (const auto& controller : _controllers)
        controller->update(delta);
}

void _bind(const py::module_& module)
{
    py::classh<Frame>(module, "Frame", R"doc(
A single animation frame containing texture and rectangle data.

Represents one frame of an animation with its associated texture and the rectangle
defining which portion of the texture to display.
        )doc")
        .def_readonly("tex", &Frame::tex, R"doc(
The texture containing the frame image.
        )doc")
        .def_readonly("src", &Frame::src, R"doc(
The rectangle defining the frame bounds within the texture.
        )doc");

    py::classh<Animation>(module, "Animation", R"doc(
A complete animation sequence with frames and playback settings.

Contains a sequence of frames and the frames per second (FPS) rate for playback timing.
        )doc")
        .def_readonly("frames", &Animation::frames, R"doc(
The list of frames in the animation sequence.
        )doc")
        .def_readonly("fps", &Animation::fps, R"doc(
The frames per second rate for animation playback.
        )doc");

    py::classh<SheetStrip>(module, "SheetStrip", R"doc(
A descriptor for one horizontal strip (row) in a sprite sheet.

Defines a single animation within a sprite sheet by specifying the animation name,
the number of frames to extract from the strip, and the playback speed in frames
per second (FPS).
    )doc")
        .def(py::init<>([](const std::string& name, int frame_count, double fps) -> SheetStrip
                        { return {name, frame_count, fps}; }),
             py::arg("name"), py::arg("frame_count"), py::arg("fps"), R"doc(
Create a sprite sheet strip definition.

Args:
    name (str): Unique identifier for this animation.
    frame_count (int): Number of frames to extract from this strip/row.
    fps (float): Frames per second for playback timing.
             )doc")
        .def_readwrite("name", &SheetStrip::name, R"doc(
The unique name identifier for this animation strip.

Type:
    str: The animation name used to reference this strip.
        )doc")
        .def_readwrite("frame_count", &SheetStrip::frameCount, R"doc(
The number of frames in this animation strip.

Specifies how many frames to extract from the horizontal strip in the sprite sheet,
reading from left to right.

Type:
    int: The number of frames (must be positive).
        )doc")
        .def_readwrite("fps", &SheetStrip::fps, R"doc(
The playback speed in frames per second.

Determines how fast the animation plays. Higher values result in faster playback.

Type:
    float: The frames per second for this animation.
        )doc");

    py::classh<AnimationController>(module, "AnimationController", R"doc(
Manages and controls sprite animations with multiple animation sequences.

The AnimationController handles loading animations from sprite sheets or image folders,
managing playback state, and providing frame-by-frame animation control.
    )doc")
        .def(py::init<>())
        .def_property_readonly("current_animation_name",
                               &AnimationController::getCurrentAnimationName, R"doc(
The name of the currently active animation.

Returns:
    str: The name of the current animation, or empty string if none is set.
                               )doc")
        .def_property_readonly("current_frame", &AnimationController::getCurrentFrame,
                               py::return_value_policy::reference_internal, R"doc(
The current animation frame being displayed.

Returns:
    Frame: The current frame with texture and rectangle data.

Raises:
    RuntimeError: If no animation is currently set or the animation has no frames.
                               )doc")
        .def_property_readonly("frame_index", &AnimationController::getFrameIndex, R"doc(
The current frame index in the animation sequence.

Returns the integer frame index (0-based) of the currently displayed frame.

Returns:
    int: The current frame index.
                               )doc")
        .def_property_readonly("progress", &AnimationController::getProgress, R"doc(
The normalized progress through the current animation.

Returns a value between 0.0 (start) and 1.0 (end) representing how far through
the animation sequence the playback has progressed. Useful for UI progress bars
or triggering events at specific points in the animation.

Returns:
    float: The animation progress as a value between 0.0 and 1.0.
                               )doc")
        .def_property("playback_speed", &AnimationController::getPlaybackSpeed,
                      &AnimationController::setPlaybackSpeed, R"doc(
The playback speed multiplier for animation timing.

A value of 1.0 represents normal speed, 2.0 is double speed, 0.5 is half speed.
Setting to 0 will pause the animation.

Returns:
    float: The current playback speed multiplier.
                      )doc")
        .def_property("looping", &AnimationController::isLooping, &AnimationController::setLooping,
                      R"doc(
Whether the animation should loop when it reaches the end.

Returns:
    bool: True if the animation is set to loop, False otherwise.
                      )doc")
        .def("load_sprite_sheet", &AnimationController::loadSpriteSheet, py::arg("file_path"),
             py::arg("frame_size"), py::arg("strips"), R"doc(
Load one or more animations from a sprite sheet texture.

Divides the sprite sheet into horizontal strips, where each strip represents a different
animation. Each strip is divided into equal-sized frames based on the specified frame size.
Frames are read left-to-right within each strip, and strips are read top-to-bottom.

Args:
    file_path (str): Path to the sprite sheet image file.
    frame_size (Vec2): Size of each frame as (width, height).
    strips (list[SheetStrip]): List of strip definitions.

Raises:
    ValueError: If frame size is not positive, no strips provided, frame count is not
               positive, or frame count exceeds sprite sheet width.
    RuntimeError: If sprite sheet dimensions are not divisible by frame dimensions,
                 duplicate animation names exist, or strip index exceeds sprite sheet height.
             )doc")
        .def("set", &AnimationController::set, py::arg("name"), R"doc(
Set the current active animation by name without affecting playback state.

Switches to the specified animation while preserving the current frame index and
playback state (paused/playing). Useful for seamless animation transitions.

Args:
    name (str): The name of the animation to activate.

Raises:
    ValueError: If the specified animation name is not found.
             )doc")
        .def("play", &AnimationController::play, py::arg("name"), R"doc(
Play an animation from the beginning.

Switches to the specified animation, rewinds it to frame 0, and starts playback.

Args:
    name (str): The name of the animation to play.

Raises:
    ValueError: If the specified animation name is not found.
             )doc")
        .def("play_from", &AnimationController::playFrom, py::arg("frame_index"), R"doc(
Start playing the current animation from a specific frame.

Sets the animation to the specified frame index and resumes playback. Useful for
starting animations mid-sequence or implementing custom animation logic.

Args:
    frame_index (int): The frame index to start from (0-based).

Raises:
    IndexError: If the frame index is out of range for the current animation.
             )doc")
        .def("is_finished", &AnimationController::isFinished, R"doc(
Check if the animation completed a full loop during the last update.

Returns True if the animation looped back to the beginning during the most recent
frame update. This method is const and can be called multiple times per frame
with consistent results.

Returns:
    bool: True if the animation completed a loop during the last update.
             )doc")
        .def("rewind", &AnimationController::rewind, R"doc(
Reset the animation to the beginning.

Sets the animation back to frame 0 and resets loop detection state.
            )doc")
        .def("pause", &AnimationController::pause, R"doc(
Pause the animation playback.

Stops animation frame advancement while preserving the current frame position.
            )doc")
        .def("resume", &AnimationController::resume, R"doc(
Resume paused animation playback.

Resumes animation frame advancement if the playback speed is greater than 0.
Does nothing if the animation is already playing or playback speed is 0.
            )doc");
}
} // namespace animation_controller
} // namespace kn

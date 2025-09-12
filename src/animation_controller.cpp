#include "Math.hpp"
#include "Texture.hpp"
#include "Time.hpp"

#include <algorithm>
#include <filesystem>
#include <pybind11/stl.h>

#include "AnimationController.hpp"

namespace fs = std::filesystem;

namespace kn
{
static std::vector<AnimationController*> _controllers;

AnimationController::AnimationController() { _controllers.push_back(this); }
AnimationController::~AnimationController() { std::erase(_controllers, this); }

void AnimationController::loadSpriteSheet(const std::string& name, const std::string& filePath,
                                          const Vec2& frameSize, const int fps)
{
    const auto texPtr = std::make_shared<Texture>(filePath);
    const Vec2 size = texPtr->getSize();

    const int frameWidth = static_cast<int>(frameSize.x);
    const int frameHeight = static_cast<int>(frameSize.y);

    if (static_cast<int>(size.x) % frameWidth || static_cast<int>(size.y) % frameHeight)
        throw std::runtime_error(filePath + " dimensions are not divisible by frame dimensions");

    if (m_animMap.contains(name))
        m_animMap.erase(name);

    Animation newAnim;
    newAnim.fps = fps;
    for (int y = 0; y < size.y; y += frameHeight)
        for (int x = 0; x < size.x; x += frameWidth)
        {
            const Frame frame{texPtr, {x, y, frameWidth, frameHeight}};
            newAnim.frames.emplace_back(frame);
        }

    if (newAnim.frames.empty())
        throw std::runtime_error("No frames found in " + filePath);

    m_animMap[name] = std::move(newAnim);
    m_currAnim = name;
}

void AnimationController::loadFolder(const std::string& name, const std::string& dirPath,
                                     const int fps)
{
    if (m_animMap.contains(name))
        m_animMap.erase(name);

    Animation newAnim;
    newAnim.fps = fps;

    // Common image extensions supported by SDL
    const std::vector<std::string> validExtensions = {".png", ".jpg", ".jpeg",
                                                      ".bmp", ".tga", ".gif"};

    for (const auto& entry : fs::directory_iterator(dirPath))
    {
        if (!entry.is_regular_file())
            continue;

        const std::string filePath = entry.path().string();
        const std::string extension = entry.path().extension().string();

        // Convert extension to lowercase for comparison
        std::string lowerExt = extension;
        std::transform(lowerExt.begin(), lowerExt.end(), lowerExt.begin(), ::tolower);

        // Check if file has a valid image extension
        if (std::find(validExtensions.begin(), validExtensions.end(), lowerExt) ==
            validExtensions.end())
            continue;

        try
        {
            const auto texPtr = std::make_shared<Texture>(filePath);
            const Frame frame{texPtr, texPtr->getRect()};
            newAnim.frames.emplace_back(frame);
        }
        catch (const std::exception&)
        {
            // Skip files that can't be loaded as textures
            continue;
        }
    }

    if (newAnim.frames.empty())
        throw std::runtime_error("No valid image frames found in " + dirPath);

    m_animMap[name] = std::move(newAnim);
    m_currAnim = name;
}

void AnimationController::remove(const std::string& name)
{
    if (m_animMap.contains(name))
        m_animMap.erase(name);
}

void AnimationController::set(const std::string& name, const bool rewind)
{
    if (!m_animMap.contains(name))
        throw std::invalid_argument("Animation not found: " + name);

    m_currAnim = name;

    if (rewind)
    {
        m_index = 0.0;
        m_prevIndex = 0.0;
    }
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

bool AnimationController::isFinished() const { return m_prevIndex > m_index; }

std::string AnimationController::getCurrentAnimationName() const { return m_currAnim; }

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

    const auto& [frames, fps] = m_animMap.at(m_currAnim);

    // Store previous index for loop detection
    m_prevIndex = m_index;

    m_index += delta * fps * m_playbackSpeed;
    const auto frameCount = static_cast<double>(frames.size());
    m_index = fmod(m_index + frameCount, frameCount);
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
        .def_readonly("rect", &Frame::rect, R"doc(
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

    py::classh<AnimationController>(module, "AnimationController", R"doc(
Manages and controls sprite animations with multiple animation sequences.

The AnimationController handles loading animations from sprite sheets or image folders,
managing playback state, and providing frame-by-frame animation control.
    )doc")
        .def(py::init<>())

        .def("load_sprite_sheet", &AnimationController::loadSpriteSheet, py::arg("name"),
             py::arg("file_path"), py::arg("frame_size"), py::arg("fps"), R"doc(
Load animation frames from a sprite sheet texture.

Divides the sprite sheet into equal-sized frames based on the specified frame size.
The frames are read left-to-right, top-to-bottom.

Args:
    name (str): Unique identifier for the animation.
    file_path (str): Path to the sprite sheet image file.
    frame_size (Vec2): Size of each frame as (width, height).
    fps (int): Frames per second for playback timing.

Raises:
    RuntimeError: If sprite sheet dimensions are not divisible by frame dimensions,
                 or no frames are found.
             )doc")
        .def("load_folder", &AnimationController::loadFolder, py::arg("name"), py::arg("dir_path"),
             py::arg("fps"), R"doc(
Load animation frames from a directory of image files.

Loads all valid image files from the specified directory as animation frames.
Supported formats include PNG, JPG, JPEG, BMP, TGA, and GIF.

Args:
    name (str): Unique identifier for the animation.
    dir_path (str): Path to the directory containing image files.
    fps (int): Frames per second for playback timing.

Raises:
    RuntimeError: If no valid image files are found in the directory.
             )doc")
        .def("remove", &AnimationController::remove, py::arg("name"), R"doc(
Remove an animation from the controller.

Args:
    name (str): The name of the animation to remove.

Note:
    If the removed animation is currently active, the controller will be left
    without a current animation.
             )doc")
        .def("set", &AnimationController::set, py::arg("name"), py::arg("rewind") = false, R"doc(
Set the current active animation by name.

Switches to the specified animation and resets playback to the beginning.

Args:
    name (str): The name of the animation to activate.
    rewind (bool): Whether to rewind the animation to the start.

Raises:
    ValueError: If the specified animation name is not found.
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
            )doc")

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
        .def_property("playback_speed", &AnimationController::getPlaybackSpeed,
                      &AnimationController::setPlaybackSpeed, R"doc(
The playback speed multiplier for animation timing.

A value of 1.0 represents normal speed, 2.0 is double speed, 0.5 is half speed.
Setting to 0 will pause the animation.

Returns:
    float: The current playback speed multiplier.
                      )doc");
}
} // namespace animation_controller
} // namespace kn

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>

#include <algorithm>
#include <filesystem>

#include "bindings/python/bindings.hpp"
#include "kraken/animation/AnimationController.hpp"
#include "kraken/core/Log.hpp"
#include "kraken/core/Time.hpp"
#include "kraken/graphics/Renderer.hpp"
#include "kraken/graphics/Texture.hpp"
#include "kraken/math/Math.hpp"

namespace kn::animation_controller
{
void _bind(nb::module_& module)
{
    using namespace nb::literals;

    nb::class_<SheetStrip>(module, "SheetStrip", R"doc(
A descriptor for one horizontal strip (row) in a sprite sheet.

Defines a single animation within a sprite sheet by specifying the animation name,
the number of frames to extract from the strip, and the playback speed in frames
per second (FPS).
    )doc")

        .def(nb::init<const std::string&, int, double>(), "name"_a, "frame_count"_a, "fps"_a, R"doc(
Create a sprite sheet strip definition.

Args:
    name (str): Unique identifier for this animation.
    frame_count (int): Number of frames to extract from this strip/row.
    fps (float): Frames per second for playback timing.
             )doc")

        .def_rw("name", &SheetStrip::name, R"doc(
The unique name identifier for this animation strip.

Type:
    str: The animation name used to reference this strip.
        )doc")
        .def_rw("frame_count", &SheetStrip::frameCount, R"doc(
The number of frames in this animation strip.

Specifies how many frames to extract from the horizontal strip in the sprite sheet,
reading from left to right.

Type:
    int: The number of frames (must be positive).
        )doc")
        .def_rw("fps", &SheetStrip::fps, R"doc(
The playback speed in frames per second.

Determines how fast the animation plays. Higher values result in faster playback.

Type:
    float: The frames per second for this animation.
        )doc");

    nb::class_<AnimationController>(module, "AnimationController", R"doc(
Manages and controls sprite animations with multiple animation sequences.

The AnimationController handles loading animations from sprite sheets or image folders,
managing playback state, and providing frame-by-frame animation control.
    )doc")
        .def(nb::init<>())

        .def_prop_ro("current_animation_name", &AnimationController::getCurrentAnimationName, R"doc(
The name of the currently active animation.

Returns:
    str: The name of the current animation, or empty string if none is set.
    )doc")
        .def_prop_ro(
            "frame_area", &AnimationController::getCurrentClip, nb::rv_policy::reference_internal,
            R"doc(
The clip area (atlas region) for the current animation frame.

Returns:
    Rect: The source rectangle defining which portion of the texture to display.

Raises:
    RuntimeError: If no animation is currently set or the animation has no frames.
    )doc"
        )
        .def_prop_ro("frame_index", &AnimationController::getFrameIndex, R"doc(
The current frame index in the animation sequence.

Returns the integer frame index (0-based) of the currently displayed frame.

Returns:
    int: The current frame index.
    )doc")
        .def_prop_ro("progress", &AnimationController::getProgress, R"doc(
The normalized progress through the current animation.

Returns a value between 0.0 (start) and 1.0 (end) representing how far through
the animation sequence the playback has progressed. Useful for UI progress bars
or triggering events at specific points in the animation.

Returns:
    float: The animation progress as a value between 0.0 and 1.0.
    )doc")
        .def_prop_rw(
            "playback_speed", &AnimationController::getPlaybackSpeed,
            &AnimationController::setPlaybackSpeed, R"doc(
The playback speed multiplier for animation timing.

A value of 1.0 represents normal speed, 2.0 is double speed, 0.5 is half speed.
Setting to 0 will pause the animation.

Returns:
    float: The current playback speed multiplier.
    )doc"
        )
        .def_prop_rw(
            "looping", &AnimationController::isLooping, &AnimationController::setLooping,
            R"doc(
Whether the animation should loop when it reaches the end.

Returns:
    bool: True if the animation is set to loop, False otherwise.
    )doc"
        )
        .def(
            "add_sheet", &AnimationController::addSheet, "frame_width"_a, "frame_height"_a,
            "strips"_a, R"doc(
Add animations from a sprite sheet definition.

Divides an atlas into horizontal strips, where each strip represents a different animation.
Each strip is divided into equal-sized frames based on the specified frame size.
Frames are read left-to-right within each strip, and strips are read top-to-bottom.

Args:
    frame_width (int): The width of each frame in pixels.
    frame_height (int): The height of each frame in pixels.
    strips (Sequence[SheetStrip]): List of strip definitions.

Raises:
    ValueError: If frame size is not positive, no strips provided, frame count is not positive.
    RuntimeError: If duplicate animation names exist.
    )doc"
        )
        .def("set", &AnimationController::set, "name"_a, R"doc(
Set the current active animation by name without affecting playback state.

Switches to the specified animation while preserving the current frame index and
playback state (paused/playing). Useful for seamless animation transitions.

Args:
    name (str): The name of the animation to activate.

Raises:
    ValueError: If the specified animation name is not found.
             )doc")
        .def("play", &AnimationController::play, "name"_a, R"doc(
Play an animation from the beginning.

Switches to the specified animation, rewinds it to frame 0, and starts playback.

Args:
    name (str): The name of the animation to play.

Raises:
    ValueError: If the specified animation name is not found.
             )doc")
        .def("play_from", &AnimationController::playFrom, "frame_index"_a, R"doc(
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
}  // namespace kn::animation_controller

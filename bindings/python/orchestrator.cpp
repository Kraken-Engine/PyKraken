#include "kraken/animation/Orchestrator.hpp"

#include <nanobind/nanobind.h>
#include <nanobind/stl/function.h>
#include <nanobind/stl/unique_ptr.h>

#include <algorithm>
#include <cmath>
#include <random>

#include "bindings/python/bindings.hpp"
#include "kraken/core/Log.hpp"
#include "kraken/core/Time.hpp"

namespace kn::orchestrator
{
void _bind(nb::module_& module)
{
    using namespace nb::literals;

    auto subFx = module.def_submodule("fx", R"doc(
Predefined effects for use with the Orchestrator.
    )doc");

    // ----- Effect base class (not directly instantiable) -----
    nb::class_<Effect>(subFx, "Effect", R"doc(
Base class for timeline effects. Not directly instantiable.
    )doc")
        .def("clone", &Effect::clone, R"doc(
Create a copy of this effect. Used internally by the Orchestrator when adding effects to the timeline.
    )doc");

    // ----- Orchestrator -----
    nb::class_<Orchestrator>(module, "Orchestrator", R"doc(
Timeline animator for Transform objects.

Allows chaining effects to create complex animations that play over time.
Effects can run sequentially or in parallel.

Attributes:
    finalized (bool): Whether the orchestrator has been finalized.
    playing (bool): Whether the animation is currently playing.
    finished (bool): Whether the animation has completed.
    looping (bool): Whether the animation should loop when finished.

Methods:
    parallel(*effects): Add multiple effects to run in parallel.
    then(effect): Add a single effect to the timeline.
    finalize(): Finalize the orchestrator, preventing further edits.
    play(): Start playing the animation from the beginning.
    pause(): Pause the animation at the current position.
    resume(): Resume a paused animation.
    stop(): Stop the animation and reset to the beginning.
    rewind(): Reset the animation to the beginning without stopping.
    )doc")
        .def(nb::init<Transform&>(), "target"_a, R"doc(
Create an Orchestrator for animating transforms.

Args:
    target (Transform): The transform to animate.
             )doc")
        .def(
            "parallel",
            [](Orchestrator& self, const nb::args& effects) -> Orchestrator&
            {
                std::vector<std::unique_ptr<Effect>> effectVec;
                effectVec.reserve(effects.size());
                for (const auto& arg : effects)
                {
                    if (nb::isinstance<Effect>(arg))
                        effectVec.push_back(nb::cast<std::unique_ptr<Effect>>(arg));
                    else
                        throw nb::type_error("parallel() arguments must all be Effect objects");
                }

                return self.parallel(std::move(effectVec));
            },
            nb::rv_policy::reference,
            nb::sig(
                "def parallel(self, *effects: pykraken._pykraken.fx.Effect) -> "
                "pykraken._pykraken.Orchestrator"
            ),
            R"doc(
Add multiple effects to run in parallel.

Args:
    *effects: Variable number of Effect objects to run simultaneously.

Returns:
    Orchestrator: Self for method chaining.
             )doc"
        )
        .def("then", &Orchestrator::then, nb::rv_policy::reference, "effect"_a, R"doc(
Add a single effect to the timeline.

Args:
    effect: The Effect to add.

Returns:
    Orchestrator: Self for method chaining.
             )doc")
        .def("finalize", &Orchestrator::finalize, R"doc(
Finalize the orchestrator, preventing further edits.

Must be called before play(). Logs a warning if called multiple times.
             )doc")
        .def("play", &Orchestrator::play, R"doc(
Start playing the animation from the beginning.

Logs a warning if not finalized or if there are no steps.
             )doc")
        .def("pause", &Orchestrator::pause, R"doc(
Pause the animation at the current position.
             )doc")
        .def("resume", &Orchestrator::resume, R"doc(
Resume a paused animation.
             )doc")
        .def("stop", &Orchestrator::stop, R"doc(
Stop the animation and reset to the beginning.
             )doc")
        .def("rewind", &Orchestrator::rewind, R"doc(
Reset the animation to the beginning without stopping.
             )doc")
        .def_prop_ro("finalized", &Orchestrator::isFinalized, R"doc(
Whether the orchestrator has been finalized.
             )doc")
        .def_prop_ro("playing", &Orchestrator::isPlaying, R"doc(
Whether the animation is currently playing.
             )doc")
        .def_prop_ro("finished", &Orchestrator::isFinished, R"doc(
Whether the animation has completed.
             )doc")
        .def_prop_rw("looping", &Orchestrator::isLooping, &Orchestrator::setLooping, R"doc(
Whether the animation should loop when finished.
             )doc");

    // ----- fx functions (private, accessed via pykraken/fx.py) -----
    subFx.def("move_to", &fx::moveTo, "pos"_a, "dur"_a = 0.0, "ease"_a = nb::none(), R"doc(
Create a move-to effect.

Args:
    pos (Vec2): Target position.
    dur (float): Duration in seconds.
    ease (callable): Easing function (t -> t).

Returns:
    Effect: The move-to effect.
        )doc");

    subFx.def("scale_to", &fx::scaleTo, "scale"_a, "dur"_a = 0.0, "ease"_a = nb::none(), R"doc(
Create a scale-to effect.

Args:
    scale (Vec2): Target scale exact dimensions.
    dur (float): Duration in seconds.
    ease (callable): Easing function (t -> t).

Returns:
    Effect: The scale-to effect.
        )doc");

    subFx.def("scale_by", &fx::scaleBy, "scale"_a, "dur"_a = 0.0, "ease"_a = nb::none(), R"doc(
Create a scale-by effect.

Args:
    scale (float): Delta scalar to apply to the scale.
    dur (float): Duration in seconds.
    ease (callable): Easing function (t -> t).

Returns:
    Effect: The scale-by effect.
        )doc");

    subFx.def(
        "rotate_to", &fx::rotateTo, "angle"_a, "clockwise"_a = true, "dur"_a = 0.0,
        "ease"_a = nb::none(), R"doc(
Create a rotate-to effect.

Args:
    angle (float): Target angle in radians.
    clockwise (bool): Direction of rotation. True for clockwise, False for counterclockwise.
    dur (float): Duration in seconds.
    ease (callable): Easing function (t -> t).

Returns:
    Effect: The rotate-to effect.
        )doc"
    );

    subFx.def(
        "rotate_by", &fx::rotateBy, "delta"_a, "clockwise"_a = true, "dur"_a = 0.0,
        "ease"_a = nb::none(), R"doc(
Create a rotate-by effect.

Args:
    delta (float): Delta angle in radians to rotate by in radians.
    clockwise (bool): Direction of rotation. True for clockwise, False for counterclockwise.
    dur (float): Duration in seconds.
    ease (callable): Easing function (t -> t).

Returns:
    Effect: The rotate-by effect.
        )doc"
    );

    subFx.def("shake", &fx::shake, "amp"_a, "freq"_a, "dur"_a, R"doc(
Create a shake effect.

Args:
    amp (float): Shake amplitude in pixels.
    freq (float): Shake frequency in Hz.
    dur (float): Duration in seconds.

Returns:
    Effect: The shake effect.
        )doc");

    subFx.def("call", &fx::call, "callback"_a, R"doc(
Create an effect that calls a function.

Args:
    callback (callable): Function to call when this step is reached.

Returns:
    Effect: The call effect.
        )doc");

    subFx.def("wait", &fx::wait, "dur"_a, R"doc(
Create a wait/delay effect.

Args:
    dur (float): Duration to wait in seconds.

Returns:
    Effect: The wait effect.
        )doc");
}
}  // namespace kn::orchestrator

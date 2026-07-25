#include "kraken/animation/Ease.hpp"

#include <nanobind/nanobind.h>
#include <nanobind/stl/function.h>

#include <algorithm>
#include <cmath>
#include <utility>
#include <vector>

#include "bindings/python/bindings.hpp"
#include "kraken/core/Time.hpp"

namespace kn::ease
{
void _bind(nb::module_& module)
{
    using namespace nb::literals;

    auto subEase = module.def_submodule("ease", "Easing functions for animations.");

    subEase.def("linear", &linear, "t"_a, R"doc(
Linear easing.

Args:
    t (float): Normalized time (0.0 to 1.0).
Returns:
    float: Eased result.
    )doc");

    subEase.def("in_quad", &inQuad, "t"_a, R"doc(
Quadratic easing in (slow start).

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("out_quad", &outQuad, "t"_a, R"doc(
Quadratic easing out (fast start).

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("in_out_quad", &inOutQuad, "t"_a, R"doc(
Quadratic easing in and out.

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("in_cubic", &inCubic, "t"_a, R"doc(
Cubic easing in (very slow start).

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("out_cubic", &outCubic, "t"_a, R"doc(
Cubic easing out (fast then smooth).

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("in_out_cubic", &inOutCubic, "t"_a, R"doc(
Cubic easing in and out.

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("in_quart", &inQuart, "t"_a, R"doc(
Quartic easing in.

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("out_quart", &outQuart, "t"_a, R"doc(
Quartic easing out.

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("in_out_quart", &inOutQuart, "t"_a, R"doc(
Quartic easing in and out.

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("in_quint", &inQuint, "t"_a, R"doc(
Quintic easing in.

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("out_quint", &outQuint, "t"_a, R"doc(
Quintic easing out.

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("in_out_quint", &inOutQuint, "t"_a, R"doc(
Quintic easing in and out.

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("in_sin", &inSin, "t"_a, R"doc(
Sinusoidal easing in.

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("out_sin", &outSin, "t"_a, R"doc(
Sinusoidal easing out.

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("in_out_sin", &inOutSin, "t"_a, R"doc(
Sinusoidal easing in and out.

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("in_circ", &inCirc, "t"_a, R"doc(
Circular easing in.

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("out_circ", &outCirc, "t"_a, R"doc(
Circular easing out.

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("in_out_circ", &inOutCirc, "t"_a, R"doc(
Circular easing in and out.

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("in_expo", &inExpo, "t"_a, R"doc(
Exponential easing in.

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("out_expo", &outExpo, "t"_a, R"doc(
Exponential easing out.

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("in_out_expo", &inOutExpo, "t"_a, R"doc(
Exponential easing in and out.

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("in_elastic", &inElastic, "t"_a, R"doc(
Elastic easing in (springy start).

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("out_elastic", &outElastic, "t"_a, R"doc(
Elastic easing out (springy end).

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("in_out_elastic", &inOutElastic, "t"_a, R"doc(
Elastic easing in and out.

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("in_back", &inBack, "t"_a, R"doc(
Back easing in (overshoot at start).

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("out_back", &outBack, "t"_a, R"doc(
Back easing out (overshoot at end).

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("in_out_back", &inOutBack, "t"_a, R"doc(
Back easing in and out.

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("in_bounce", &inBounce, "t"_a, R"doc(
Bounce easing in (bounces toward target).

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("out_bounce", &outBounce, "t"_a, R"doc(
Bounce easing out (bounces after start).

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("in_out_bounce", &inOutBounce, "t"_a, R"doc(
Bounce easing in and out.

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    nb::class_<Tween>(module, "Tween", R"doc(
A class for animating values over time using easing functions.

This class supports pausing, resuming, reversing, and checking progress.
    )doc")

        .def(nb::init<EasingFunction, double>(), "ease_func"_a, "duration"_a, R"doc(
Create a Tween.

Args:
    ease_func (Callable[[float], float]): Easing function that maps [0, 1] → [0, 1].
    duration (float): Time in seconds for full animation.
    )doc")

        .def_rw("start_pos", &Tween::startPos, R"doc(
The starting position of the animation.
    )doc")
        .def_rw("end_pos", &Tween::endPos, R"doc(
The ending position of the animation.
    )doc")

        .def_prop_ro("is_done", &Tween::isDone, R"doc(
Check whether the animation has finished.
            )doc")
        .def_prop_ro("current_pos", &Tween::getCurrentPosition, R"doc(
    Get the current interpolated position snapshot.

    Returns:
        Vec2: Interpolated position.
            )doc")

        .def("pause", &Tween::pause, R"doc(
Pause the animation's progression.
    )doc")
        .def("resume", &Tween::resume, R"doc(
Resume the animation from its current state.
    )doc")
        .def("restart", &Tween::restart, R"doc(
Restart the animation from the beginning.
    )doc")
        .def("reverse", &Tween::reverse, R"doc(
Reverse the direction of the animation.
    )doc");
}
}  // namespace kn::ease

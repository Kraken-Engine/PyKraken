
#include "Time.hpp"

#include "Ease.hpp"
#include <cmath>
#include <pybind11/functional.h>
#include <utility>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#ifndef M_PI_2
#define M_PI_2 1.5707963267948966192313216916398
#endif

namespace kn
{
EasingAnimation::EasingAnimation(const Vec2& start, const Vec2& end, const double duration,
                                 ease::EasingFunction easeFunc)
    : startPos(start), endPos(end), duration(duration), easingFunc(std::move(easeFunc))
{
}

Vec2 EasingAnimation::step()
{
    if (state == State::PAUSED || state == State::DONE)
        return getCurrentPosition();

    const double delta = kn::time::getDelta();
    elapsedTime += forward ? delta : -delta;
    elapsedTime = std::max(0.0, std::min(elapsedTime, duration));

    if (elapsedTime == duration || elapsedTime == 0.0)
        state = State::DONE;

    return getCurrentPosition();
}

Vec2 EasingAnimation::getCurrentPosition() const
{
    double t = elapsedTime / duration;
    t = std::max(0.0, std::min(t, 1.0));
    const double easedT = easingFunc(t);
    return math::lerp(startPos, endPos, easedT);
}

void EasingAnimation::pause() { state = State::PAUSED; }

void EasingAnimation::resume()
{
    if (state != State::DONE)
        state = State::PLAYING;
}

void EasingAnimation::restart()
{
    elapsedTime = forward ? 0.0 : duration;
    state = State::PLAYING;
}

void EasingAnimation::reverse()
{
    forward = !forward;
    state = State::PLAYING;
}

bool EasingAnimation::isDone() const { return state == State::DONE; }

namespace ease
{
double linear(const double t) { return t; }

double inQuad(const double t) { return t * t; }

double outQuad(const double t) { return -(t * (t - 2)); }

double inOutQuad(const double t)
{
    if (t < 0.5)
        return 2 * t * t;

    return -2 * t * t + 4 * t - 1;
}

double inCubic(const double t) { return t * t * t; }

double outCubic(const double t)
{
    const double f = t - 1;
    return f * f * f + 1;
}

double inOutCubic(const double t)
{
    if (t < 0.5)
        return 4 * t * t * t;

    const double f = 2 * t - 2;
    return 0.5 * f * f * f + 1;
}

double inQuart(const double t) { return t * t * t * t; }

double outQuart(const double t)
{
    const double f = t - 1;
    return f * f * f * (1 - t) + 1;
}

double inOutQuart(const double t)
{
    if (t < 0.5)
        return 8 * t * t * t * t;

    const double f = t - 1;
    return -8 * f * f * f * f + 1;
}

double inQuint(const double t) { return t * t * t * t * t; }

double outQuint(const double t)
{
    const double f = t - 1;
    return f * f * f * f * f + 1;
}

double inOutQuint(const double t)
{
    if (t < 0.5)
        return 16 * t * t * t * t * t;

    const double f = 2 * t - 2;
    return 0.5 * f * f * f * f * f + 1;
}

double inSin(const double t) { return sin((t - 1) * M_PI_2) + 1; }

double outSin(const double t) { return sin(t * M_PI_2); }

double inOutSin(const double t) { return 0.5 * (1 - cos(t * M_PI)); }

double inCirc(const double t) { return 1 - sqrt(1 - t * t); }

double outCirc(const double t) { return sqrt((2 - t) * t); }

double inOutCirc(const double t)
{
    if (t < 0.5)
        return 0.5 * (1 - sqrt(1 - 4 * (t * t)));

    return 0.5 * (sqrt(-(2 * t - 3) * (2 * t - 1)) + 1);
}

double inExpo(const double t) { return t == 0.0 ? t : pow(2, 10 * (t - 1)); }

double outExpo(const double t) { return t == 1.0 ? t : 1 - pow(2, -10 * t); }

double inOutExpo(const double t)
{
    if (t == 0.0 || t == 1.0)
        return t;

    if (t < 0.5)
        return 0.5 * pow(2, 20 * t - 10);

    return -0.5 * pow(2, -20 * t + 10) + 1;
}

double inElastic(const double t) { return sin(13 * M_PI_2 * t) * pow(2, 10 * (t - 1)); }

double outElastic(const double t) { return sin(-13 * M_PI_2 * (t + 1)) * pow(2, -10 * t) + 1; }

double inOutElastic(const double t)
{
    if (t < 0.5)
        return 0.5 * sin(13 * M_PI_2 * (2 * t)) * pow(2, 10 * (2 * t - 1));

    return 0.5 * (sin(-13 * M_PI_2 * (2 * t - 1 + 1)) * pow(2, -10 * (2 * t - 1)) + 2);
}

double inBack(const double t) { return t * t * t - t * sin(t * M_PI); }

double outBack(const double t)
{
    const double f = 1 - t;
    return 1 - (f * f * f - f * sin(f * M_PI));
}

double inOutBack(const double t)
{
    if (t < 0.5)
    {
        const double f = 2 * t;
        return 0.5 * (f * f * f - f * sin(f * M_PI));
    }

    const double f = 1 - (2 * t - 1);
    return 0.5 * (1 - (f * f * f - f * sin(f * M_PI))) + 0.5;
}

double inBounce(const double t) { return 1 - outBounce(1 - t); }

double outBounce(const double t)
{
    if (t < 4 / 11.0)
        return 121 * t * t / 16.0;

    if (t < 8 / 11.0)
        return 363 / 40.0 * t * t - 99 / 10.0 * t + 17 / 5.0;

    if (t < 9 / 10.0)
        return 4356 / 361.0 * t * t - 35442 / 1805.0 * t + 16061 / 1805.0;

    return 54 / 5.0 * t * t - 513 / 25.0 * t + 268 / 25.0;
}

double inOutBounce(const double t)
{
    if (t < 0.5)
        return 0.5 * inBounce(t * 2);

    return 0.5 * outBounce(t * 2 - 1) + 0.5;
}

void _bind(py::module_& module)
{
    auto subEase = module.def_submodule("ease", "Easing functions and animation utilities");

    subEase.def("linear", &linear, py::arg("t"), R"doc(
Linear easing.

Args:
    t (float): Normalized time (0.0 to 1.0).
Returns:
    float: Eased result.
    )doc");

    subEase.def("in_quad", &inQuad, py::arg("t"), R"doc(
Quadratic easing in (slow start).

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("out_quad", &outQuad, py::arg("t"), R"doc(
Quadratic easing out (fast start).

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("in_out_quad", &inOutQuad, py::arg("t"), R"doc(
Quadratic easing in and out.

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("in_cubic", &inCubic, py::arg("t"), R"doc(
Cubic easing in (very slow start).

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("out_cubic", &outCubic, py::arg("t"), R"doc(
Cubic easing out (fast then smooth).

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("in_out_cubic", &inOutCubic, py::arg("t"), R"doc(
Cubic easing in and out.

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("in_quart", &inQuart, py::arg("t"), R"doc(
Quartic easing in.

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("out_quart", &outQuart, py::arg("t"), R"doc(
Quartic easing out.

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("in_out_quart", &inOutQuart, py::arg("t"), R"doc(
Quartic easing in and out.

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("in_quint", &inQuint, py::arg("t"), R"doc(
Quintic easing in.

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("out_quint", &outQuint, py::arg("t"), R"doc(
Quintic easing out.

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("in_out_quint", &inOutQuint, py::arg("t"), R"doc(
Quintic easing in and out.

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("in_sin", &inSin, py::arg("t"), R"doc(
Sinusoidal easing in.

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("out_sin", &outSin, py::arg("t"), R"doc(
Sinusoidal easing out.

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("in_out_sin", &inOutSin, py::arg("t"), R"doc(
Sinusoidal easing in and out.

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("in_circ", &inCirc, py::arg("t"), R"doc(
Circular easing in.

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("out_circ", &outCirc, py::arg("t"), R"doc(
Circular easing out.

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("in_out_circ", &inOutCirc, py::arg("t"), R"doc(
Circular easing in and out.

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("in_expo", &inExpo, py::arg("t"), R"doc(
Exponential easing in.

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("out_expo", &outExpo, py::arg("t"), R"doc(
Exponential easing out.

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("in_out_expo", &inOutExpo, py::arg("t"), R"doc(
Exponential easing in and out.

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("in_elastic", &inElastic, py::arg("t"), R"doc(
Elastic easing in (springy start).

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("out_elastic", &outElastic, py::arg("t"), R"doc(
Elastic easing out (springy end).

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("in_out_elastic", &inOutElastic, py::arg("t"), R"doc(
Elastic easing in and out.

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("in_back", &inBack, py::arg("t"), R"doc(
Back easing in (overshoot at start).

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("out_back", &outBack, py::arg("t"), R"doc(
Back easing out (overshoot at end).

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("in_out_back", &inOutBack, py::arg("t"), R"doc(
Back easing in and out.

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("in_bounce", &inBounce, py::arg("t"), R"doc(
Bounce easing in (bounces toward target).

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("out_bounce", &outBounce, py::arg("t"), R"doc(
Bounce easing out (bounces after start).

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    subEase.def("in_out_bounce", &inOutBounce, py::arg("t"), R"doc(
Bounce easing in and out.

Args:
    t (float): Normalized time.
Returns:
    float: Eased result.
    )doc");

    py::classh<EasingAnimation>(module, "EasingAnimation", R"doc(
A class for animating values over time using easing functions.

This class supports pausing, resuming, reversing, and checking progress.
    )doc")

        .def(py::init<const Vec2&, const Vec2&, double, EasingFunction>(), py::arg("start"),
             py::arg("end"), py::arg("duration"), py::arg("ease_func"), R"doc(
Create an EasingAnimation.

Args:
    start (Vec2): Starting position.
    end (Vec2): Ending position.
    duration (float): Time in seconds for full animation.
    ease_func (Callable): Easing function that maps [0, 1] → [0, 1].
    )doc")

        .def_property_readonly("is_done", &EasingAnimation::isDone, R"doc(
Check whether the animation has finished.
    )doc")

        .def("step", &EasingAnimation::step, R"doc(
Advance the animation get its current position.

Returns:
    Vec2: Interpolated position.
    )doc")

        .def("pause", &EasingAnimation::pause, R"doc(
Pause the animation's progression.
    )doc")

        .def("resume", &EasingAnimation::resume, R"doc(
Resume the animation from its current state.
    )doc")

        .def("restart", &EasingAnimation::restart, R"doc(
Restart the animation from the beginning.
    )doc")

        .def("reverse", &EasingAnimation::reverse, R"doc(
Reverse the direction of the animation.
    )doc");
}
} // namespace ease
} // namespace kn
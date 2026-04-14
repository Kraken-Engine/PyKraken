#include "Ease.hpp"

#ifdef KRAKEN_ENABLE_PYTHON
#include <nanobind/stl/function.h>
#endif  // KRAKEN_ENABLE_PYTHON

#include <algorithm>
#include <cmath>
#include <utility>
#include <vector>

#include "Time.hpp"

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#ifndef M_PI_2
#define M_PI_2 1.5707963267948966192313216916398
#endif

namespace kn
{
static std::vector<Tween*> _tweens;

Tween::Tween(ease::EasingFunction easeFunc, const double duration)
    : easingFunc(std::move(easeFunc)),
      duration(duration)
{
    _tweens.push_back(this);
}

Tween::~Tween()
{
    std::erase(_tweens, this);
}

void Tween::update(const double delta)
{
    if (state == State::PAUSED || state == State::DONE)
        return;

    const double maxDuration = std::max(duration, 0.0);
    elapsedTime += forward ? delta : -delta;
    elapsedTime = std::clamp(elapsedTime, 0.0, maxDuration);

    if (forward && elapsedTime >= maxDuration)
        state = State::DONE;
    else if (!forward && elapsedTime <= 0.0)
        state = State::DONE;
}

Vec2 Tween::getCurrentPosition() const
{
    const double maxDuration = std::max(duration, 0.0);
    if (maxDuration == 0.0)
        return forward ? endPos : startPos;

    double t = elapsedTime / maxDuration;
    t = std::clamp(t, 0.0, 1.0);
    const double easedT = easingFunc(t);
    return math::lerp(startPos, endPos, easedT);
}

void Tween::pause()
{
    state = State::PAUSED;
}

void Tween::resume()
{
    if (state != State::DONE)
        state = State::PLAYING;
}

void Tween::restart()
{
    const double maxDuration = std::max(duration, 0.0);
    elapsedTime = forward ? 0.0 : maxDuration;
    state = maxDuration == 0.0 ? State::DONE : State::PLAYING;
}

void Tween::reverse()
{
    forward = !forward;
    state = duration > 0.0 ? State::PLAYING : State::DONE;
}

bool Tween::isDone() const
{
    return state == State::DONE;
}

namespace ease
{
void _tick()
{
    const double delta = time::getDelta();
    for (auto* tween : _tweens)
        tween->update(delta);
}

double linear(const double t)
{
    return t;
}

double inQuad(const double t)
{
    return t * t;
}

double outQuad(const double t)
{
    return -(t * (t - 2));
}

double inOutQuad(const double t)
{
    if (t < 0.5)
        return 2 * t * t;

    return -2 * t * t + 4 * t - 1;
}

double inCubic(const double t)
{
    return t * t * t;
}

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

double inQuart(const double t)
{
    return t * t * t * t;
}

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

double inQuint(const double t)
{
    return t * t * t * t * t;
}

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

double inSin(const double t)
{
    return sin((t - 1) * M_PI_2) + 1;
}

double outSin(const double t)
{
    return sin(t * M_PI_2);
}

double inOutSin(const double t)
{
    return 0.5 * (1 - cos(t * M_PI));
}

double inCirc(const double t)
{
    return 1 - sqrt(1 - t * t);
}

double outCirc(const double t)
{
    return sqrt((2 - t) * t);
}

double inOutCirc(const double t)
{
    if (t < 0.5)
        return 0.5 * (1 - sqrt(1 - 4 * (t * t)));

    return 0.5 * (sqrt(-(2 * t - 3) * (2 * t - 1)) + 1);
}

double inExpo(const double t)
{
    return t == 0.0 ? t : pow(2, 10 * (t - 1));
}

double outExpo(const double t)
{
    return t == 1.0 ? t : 1 - pow(2, -10 * t);
}

double inOutExpo(const double t)
{
    if (t == 0.0 || t == 1.0)
        return t;

    if (t < 0.5)
        return 0.5 * pow(2, 20 * t - 10);

    return -0.5 * pow(2, -20 * t + 10) + 1;
}

double inElastic(const double t)
{
    return sin(13 * M_PI_2 * t) * pow(2, 10 * (t - 1));
}

double outElastic(const double t)
{
    return sin(-13 * M_PI_2 * (t + 1)) * pow(2, -10 * t) + 1;
}

double inOutElastic(const double t)
{
    if (t < 0.5)
        return 0.5 * sin(13 * M_PI_2 * (2 * t)) * pow(2, 10 * (2 * t - 1));

    return 0.5 * (sin(-13 * M_PI_2 * (2 * t - 1 + 1)) * pow(2, -10 * (2 * t - 1)) + 2);
}

double inBack(const double t)
{
    return t * t * t - t * sin(t * M_PI);
}

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

double inBounce(const double t)
{
    return 1 - outBounce(1 - t);
}

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

#ifdef KRAKEN_ENABLE_PYTHON
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
#endif  // KRAKEN_ENABLE_PYTHON

}  // namespace ease
}  // namespace kn

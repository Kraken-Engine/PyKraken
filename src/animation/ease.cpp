#include "kraken/animation/Ease.hpp"

#include <algorithm>
#include <cmath>
#include <utility>
#include <vector>

#include "kraken/core/Time.hpp"

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

}  // namespace ease
}  // namespace kn

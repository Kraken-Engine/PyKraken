#pragma once

#include "Math.hpp"

#include <functional>
#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace ease
{
void _bind(py::module_& module);

using EasingFunction = std::function<double(double)>;

class EasingAnimation
{
  public:
    EasingAnimation(const Vec2& start, const Vec2& end, double duration, EasingFunction easeFunc);
    ~EasingAnimation() = default;

    Vec2 step();

    void pause();

    void resume();

    void restart();

    void reverse();

    bool isDone();

  private:
    enum class State
    {
        PLAYING,
        PAUSED,
        DONE,
    };

    Vec2 startPos;
    Vec2 endPos;
    double duration;
    EasingFunction easingFunc;

    double elapsedTime = 0.0;
    State state = State::PLAYING;
    bool forward = true;

    Vec2 getCurrentPosition() const;
};

double linear(double t);

double inQuad(double t);

double outQuad(double t);

double inOutQuad(double t);

double inCubic(double t);

double outCubic(double t);

double inOutCubic(double t);

double inQuart(double t);

double outQuart(double t);

double inOutQuart(double t);

double inQuint(double t);

double outQuint(double t);

double inOutQuint(double t);

double inSin(double t);

double outSin(double t);

double inOutSin(double t);

double inCirc(double t);

double outCirc(double t);

double inOutCirc(double t);

double inExpo(double t);

double outExpo(double t);

double inOutExpo(double t);

double inElastic(double t);

double outElastic(double t);

double inOutElastic(double t);

double inBack(double t);

double outBack(double t);

double inOutBack(double t);

double inBounce(double t);

double outBounce(double t);

double inOutBounce(double t);
} // namespace ease

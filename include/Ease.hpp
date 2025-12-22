#pragma once

#include <pybind11/pybind11.h>

#include <functional>

#include "Math.hpp"

namespace py = pybind11;

namespace kn
{
namespace ease
{
using EasingFunction = std::function<double(double)>;

void _bind(py::module_& module);

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
}  // namespace ease

class EasingAnimation
{
  public:
    Vec2 startPos{};
    Vec2 endPos{};

    EasingAnimation(ease::EasingFunction easeFunc, double duration);
    ~EasingAnimation() = default;

    Vec2 step();

    void pause();

    void resume();

    void restart();

    void reverse();

    bool isDone() const;

  private:
    enum class State
    {
        PLAYING,
        PAUSED,
        DONE,
    };

    ease::EasingFunction easingFunc;
    double duration;

    double elapsedTime = 0.0;
    State state = State::PLAYING;
    bool forward = true;

    [[nodiscard]] Vec2 getCurrentPosition() const;
};
}  // namespace kn

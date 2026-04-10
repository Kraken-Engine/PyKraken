#pragma once

#ifdef KRAKEN_ENABLE_PYTHON
#include <nanobind/nanobind.h>
#endif  // KRAKEN_ENABLE_PYTHON

#include <functional>
#include <memory>
#include <vector>

#include "Math.hpp"
#include "Transform.hpp"

#ifdef KRAKEN_ENABLE_PYTHON
namespace nb = nanobind;
#endif  // KRAKEN_ENABLE_PYTHON

namespace kn
{
namespace orchestrator
{
#ifdef KRAKEN_ENABLE_PYTHON
void _bind(nb::module_& module);
#endif  // KRAKEN_ENABLE_PYTHON

void _tick();
}  // namespace orchestrator

class Effect
{
  public:
    double duration = 0.0;
    std::function<double(double)> easing = [](double t) { return t; };  // Linear by default

    virtual ~Effect() = default;

    virtual void start(Transform& transform) = 0;
    virtual void update(Transform& transform, double t) = 0;
};

class MoveToEffect : public Effect
{
  public:
    Vec2 targetPos;

    void start(Transform& transform) override;
    void update(Transform& transform, double t) override;

  private:
    Vec2 m_startPos;
};

class ScaleToEffect : public Effect
{
  public:
    Vec2 targetScale;

    void start(Transform& transform) override;
    void update(Transform& transform, double t) override;

  private:
    Vec2 m_startScale;
};

class RotateToEffect : public Effect
{
  public:
    double targetAngle = 0.0;
    bool clockwise = true;

    void start(Transform& transform) override;
    void update(Transform& transform, double t) override;

  private:
    double m_startAngle = 0.0;
};

class RotateByEffect : public Effect
{
  public:
    double deltaAngle = 0.0;
    bool clockwise = true;

    void start(Transform& transform) override;
    void update(Transform& transform, double t) override;

  private:
    double m_startAngle = 0.0;
};

class ShakeEffect : public Effect
{
  public:
    double amplitude = 0.0;
    double frequency = 0.0;

    void start(Transform& transform) override;
    void update(Transform& transform, double t) override;

  private:
    Vec2 m_originalPos;
};

class CallEffect : public Effect
{
  public:
    std::function<void()> callback;

    void start(Transform& transform) override;
    void update(Transform& transform, double t) override;

  private:
    bool m_called = false;
};

struct Step
{
    std::vector<std::shared_ptr<Effect>> effects;
    double duration = 0.0;
};

class Orchestrator
{
  public:
    Orchestrator() = default;
    ~Orchestrator();

    void setTarget(Transform* target);

    Orchestrator& parallel(const std::vector<std::shared_ptr<Effect>>& effects);
    Orchestrator& then(std::shared_ptr<Effect> effect);

    void finalize();
    void play();
    void pause();
    void resume();
    void stop();
    void rewind();

    [[nodiscard]] bool isFinalized() const;
    [[nodiscard]] bool isPlaying() const;
    [[nodiscard]] bool isFinished() const;

    void setLooping(bool loop);
    [[nodiscard]] bool isLooping() const;

  private:
    Transform* m_target = nullptr;
    std::vector<Step> m_steps;
    bool m_finalized = false;
    bool m_playing = false;
    bool m_looping = false;
    size_t m_currentStep = 0;
    double m_stepTime = 0.0;
    bool m_stepStarted = false;

    void update(double dt);

    friend void orchestrator::_tick();
};

}  // namespace kn

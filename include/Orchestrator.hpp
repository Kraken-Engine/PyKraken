#pragma once

#ifdef KRAKEN_ENABLE_PYTHON
#include <nanobind/nanobind.h>
#endif  // KRAKEN_ENABLE_PYTHON

#include <functional>
#include <memory>
#include <vector>

#include "Ease.hpp"
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

class Effect;

namespace fx
{

[[nodiscard]] std::unique_ptr<Effect> moveTo(
    const Vec2& pos, double dur = 0.0, const ease::EasingFunction& easeFunc = nullptr
);

[[nodiscard]] std::unique_ptr<Effect> scaleTo(
    const Vec2& scale, double dur = 0.0, const ease::EasingFunction& easeFunc = nullptr
);

[[nodiscard]] std::unique_ptr<Effect> scaleBy(
    double scale, double dur = 0.0, const ease::EasingFunction& easeFunc = nullptr
);

[[nodiscard]] std::unique_ptr<Effect> rotateTo(
    double angle, bool clockwise = true, double dur = 0.0,
    const ease::EasingFunction& easeFunc = nullptr
);

[[nodiscard]] std::unique_ptr<Effect> rotateBy(
    double deltaAngle, bool clockwise = true, double dur = 0.0,
    const ease::EasingFunction& easeFunc = nullptr
);

[[nodiscard]] std::unique_ptr<Effect> shake(double amp, double freq, double dur);

[[nodiscard]] std::unique_ptr<Effect> call(const std::function<void()>& callback);

[[nodiscard]] std::unique_ptr<Effect> wait(double dur);

}  // namespace fx

class Effect
{
  public:
    double duration = 0.0;
    std::function<double(double)> easing = [](double t) { return t; };  // Linear by default

    virtual ~Effect() = default;

    virtual std::unique_ptr<Effect> clone() const = 0;

    virtual void start(Transform& transform) = 0;
    virtual void update(Transform& transform, double t) = 0;
};

class MoveToEffect : public Effect
{
  public:
    std::unique_ptr<Effect> clone() const override;

  private:
    Vec2 targetPos;

    void start(Transform& transform) override;
    void update(Transform& transform, double t) override;

    Vec2 m_startPos;

    friend std::unique_ptr<Effect> fx::moveTo(const Vec2&, double, const ease::EasingFunction&);
};

class ScaleToEffect : public Effect
{
  public:
    std::unique_ptr<Effect> clone() const override;

  private:
    Vec2 targetScale;

    void start(Transform& transform) override;
    void update(Transform& transform, double t) override;

    Vec2 m_startScale;

    friend std::unique_ptr<Effect> fx::scaleTo(const Vec2&, double, const ease::EasingFunction&);
};

class ScaleByEffect : public Effect
{
  public:
    std::unique_ptr<Effect> clone() const override;

  private:
    Vec2 deltaScale;

    void start(Transform& transform) override;
    void update(Transform& transform, double t) override;

    Vec2 m_startScale;

    friend std::unique_ptr<Effect> fx::scaleBy(double, double, const ease::EasingFunction&);
};

class RotateToEffect : public Effect
{
  public:
    std::unique_ptr<Effect> clone() const override;

  private:
    double targetAngle = 0.0;
    bool clockwise = true;

    void start(Transform& transform) override;
    void update(Transform& transform, double t) override;

    double m_startAngle = 0.0;

    friend std::unique_ptr<Effect> fx::rotateTo(double, bool, double, const ease::EasingFunction&);
};

class RotateByEffect : public Effect
{
  public:
    std::unique_ptr<Effect> clone() const override;

  private:
    double deltaAngle = 0.0;
    bool clockwise = true;

    void start(Transform& transform) override;
    void update(Transform& transform, double t) override;

    double m_startAngle = 0.0;

    friend std::unique_ptr<Effect> fx::rotateBy(double, bool, double, const ease::EasingFunction&);
};

class ShakeEffect : public Effect
{
  public:
    std::unique_ptr<Effect> clone() const override;

  private:
    double amplitude = 0.0;
    double frequency = 0.0;

    void start(Transform& transform) override;
    void update(Transform& transform, double t) override;

    Vec2 m_originalPos;

    friend std::unique_ptr<Effect> fx::shake(double, double, double);
};

class CallEffect : public Effect
{
  public:
    std::unique_ptr<Effect> clone() const override;

  private:
    std::function<void()> callback;

    void start(Transform& transform) override;
    void update(Transform& transform, double t) override;

    bool m_called = false;

    friend std::unique_ptr<Effect> fx::call(const std::function<void()>&);
    friend std::unique_ptr<Effect> fx::wait(double);
};

struct Step
{
    std::vector<std::unique_ptr<Effect>> effects;
    double duration = 0.0;

    Step() = default;
    Step(const Step&) = delete;
    Step& operator=(const Step&) = delete;
    Step(Step&&) noexcept = default;
    Step& operator=(Step&&) noexcept = default;
};

class Orchestrator
{
  public:
    explicit Orchestrator(Transform& target);
    ~Orchestrator();

    Orchestrator(const Orchestrator&) = delete;
    Orchestrator& operator=(const Orchestrator&) = delete;
    Orchestrator(Orchestrator&&) = delete;
    Orchestrator& operator=(Orchestrator&&) = delete;

    void setTarget(Transform& target);

    Orchestrator& parallel(std::vector<std::unique_ptr<Effect>> effects);
    Orchestrator& then(std::unique_ptr<Effect> effect);

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

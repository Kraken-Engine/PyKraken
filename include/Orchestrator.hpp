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

struct Step
{
    std::vector<std::shared_ptr<Effect>> effects;
    double duration = 0.0;
};

class Orchestrator
{
  public:
    explicit Orchestrator(Transform& target);
    ~Orchestrator();

    void setTarget(Transform& target);

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

namespace fx
{
[[nodiscard]] std::shared_ptr<Effect> moveTo(
    const Vec2& pos, double dur = 0.0, const ease::EasingFunction& easeFunc = nullptr
);

[[nodiscard]] std::shared_ptr<Effect> scaleTo(
    const Vec2& scale, double dur = 0.0, const ease::EasingFunction& easeFunc = nullptr
);

[[nodiscard]] std::shared_ptr<Effect> scaleBy(
    double scale, double dur = 0.0, const ease::EasingFunction& easeFunc = nullptr
);

[[nodiscard]] std::shared_ptr<Effect> rotateTo(
    double angle, bool clockwise = true, double dur = 0.0,
    const ease::EasingFunction& easeFunc = nullptr
);

[[nodiscard]] std::shared_ptr<Effect> rotateBy(
    double deltaAngle, bool clockwise = true, double dur = 0.0,
    const ease::EasingFunction& easeFunc = nullptr
);

[[nodiscard]] std::shared_ptr<Effect> shake(double amp, double freq, double dur);

[[nodiscard]] std::shared_ptr<Effect> call(const std::function<void()>& callback);

[[nodiscard]] std::shared_ptr<Effect> wait(double dur);
}  // namespace fx

}  // namespace kn

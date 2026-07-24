#include "kraken/animation/Orchestrator.hpp"

#include <algorithm>
#include <cmath>
#include <random>

#include "kraken/core/Log.hpp"
#include "kraken/core/Time.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace kn
{
static std::vector<Orchestrator*> _orchestrators;

// ----- MoveToEffect -----
void MoveToEffect::start(Transform& transform)
{
    m_startPos = transform.pos;
}

void MoveToEffect::update(Transform& transform, double t)
{
    const double easedT = easing(t);
    transform.pos.x = m_startPos.x + (targetPos.x - m_startPos.x) * easedT;
    transform.pos.y = m_startPos.y + (targetPos.y - m_startPos.y) * easedT;
}

std::unique_ptr<Effect> MoveToEffect::clone() const
{
    auto effect = std::make_unique<MoveToEffect>();
    effect->targetPos = targetPos;
    effect->duration = duration;
    effect->easing = easing;
    return effect;
}

// ----- ScaleToEffect -----
void ScaleToEffect::start(Transform& transform)
{
    m_startScale = transform.scale;
}

void ScaleToEffect::update(Transform& transform, double t)
{
    transform.scale = m_startScale + (targetScale - m_startScale) * easing(t);
}

std::unique_ptr<Effect> ScaleToEffect::clone() const
{
    auto effect = std::make_unique<ScaleToEffect>();
    effect->targetScale = targetScale;
    effect->duration = duration;
    effect->easing = easing;
    return effect;
}

// ----- ScaleByEffect -----
void ScaleByEffect::start(Transform& transform)
{
    m_startScale = transform.scale;
}

void ScaleByEffect::update(Transform& transform, double t)
{
    transform.scale = m_startScale + deltaScale * easing(t);
}

std::unique_ptr<Effect> ScaleByEffect::clone() const
{
    auto effect = std::make_unique<ScaleByEffect>();
    effect->deltaScale = deltaScale;
    effect->duration = duration;
    effect->easing = easing;
    return effect;
}

// ----- RotateToEffect -----
void RotateToEffect::start(Transform& transform)
{
    m_startAngle = transform.angle;
}

void RotateToEffect::update(Transform& transform, double t)
{
    const double easedT = easing(t);
    double delta;
    if (clockwise)
    {
        delta = fmod(targetAngle - m_startAngle, 2 * M_PI);
        if (delta < 0)
            delta += 2 * M_PI;
    }
    else
    {
        delta = fmod(m_startAngle - targetAngle, 2 * M_PI);
        if (delta < 0)
            delta += 2 * M_PI;
        delta = -delta;
    }
    transform.angle = m_startAngle + delta * easedT;
}

std::unique_ptr<Effect> RotateToEffect::clone() const
{
    auto effect = std::make_unique<RotateToEffect>();
    effect->targetAngle = targetAngle;
    effect->clockwise = clockwise;
    effect->duration = duration;
    effect->easing = easing;
    return effect;
}

// ----- RotateByEffect -----
void RotateByEffect::start(Transform& transform)
{
    m_startAngle = transform.angle;
}

void RotateByEffect::update(Transform& transform, double t)
{
    const double easedT = easing(t);
    const double delta = !clockwise ? deltaAngle : -deltaAngle;
    transform.angle = m_startAngle + delta * easedT;
}

std::unique_ptr<Effect> RotateByEffect::clone() const
{
    auto effect = std::make_unique<RotateByEffect>();
    effect->deltaAngle = deltaAngle;
    effect->clockwise = clockwise;
    effect->duration = duration;
    effect->easing = easing;
    return effect;
}

// ----- ShakeEffect -----
std::unique_ptr<Effect> ShakeEffect::clone() const
{
    auto effect = std::make_unique<ShakeEffect>();
    effect->amplitude = amplitude;
    effect->frequency = frequency;
    effect->duration = duration;
    effect->easing = easing;
    return effect;
}

void ShakeEffect::start(Transform& transform)
{
    m_originalPos = transform.pos;
}

void ShakeEffect::update(Transform& transform, double t)
{
    if (t >= 1.0)
    {
        transform.pos = m_originalPos;
        return;
    }

    // Decay amplitude over time
    const double decay = 1.0 - t;
    const double time = t * duration;

    // Use sine waves with some randomness for shake
    static std::mt19937 rng(std::random_device{}());
    static std::uniform_real_distribution<double> dist(-1.0, 1.0);

    const Vec2 phase =
        {std::sin(time * frequency * 2.0 * M_PI + dist(rng) * 0.5),
         std::sin(time * frequency * 2.0 * M_PI * 1.1 + dist(rng) * 0.5)};

    transform.pos = m_originalPos + amplitude * decay * phase;
}

// ----- CallEffect -----
void CallEffect::start([[maybe_unused]] Transform& transform)
{
    m_called = false;
}

std::unique_ptr<Effect> CallEffect::clone() const
{
    auto effect = std::make_unique<CallEffect>();
    effect->callback = callback;
    effect->duration = duration;
    effect->easing = easing;
    return effect;
}

void CallEffect::update([[maybe_unused]] Transform& transform, [[maybe_unused]] double t)
{
    if (!m_called && callback)
    {
        callback();
        m_called = true;
    }
}

// ----- Orchestrator -----
Orchestrator::Orchestrator(Transform& target)
    : m_target(&target)
{
}

Orchestrator::~Orchestrator()
{
    std::erase(_orchestrators, this);
}

void Orchestrator::setTarget(Transform& target)
{
    m_target = &target;
}

Orchestrator& Orchestrator::parallel(std::vector<std::unique_ptr<Effect>> effects)
{
    if (m_finalized)
    {
        log::warn("Orchestrator is finalized, cannot add more steps");
        return *this;
    }

    Step step;

    // Duration is the max of all parallel effects
    for (const auto& effect : effects)
    {
        if (effect->duration > step.duration)
            step.duration = effect->duration;
    }

    step.effects = std::move(effects);

    m_steps.push_back(std::move(step));
    return *this;
}

Orchestrator& Orchestrator::then(std::unique_ptr<Effect> effect)
{
    std::vector<std::unique_ptr<Effect>> vec;
    vec.push_back(std::move(effect));

    return parallel(std::move(vec));
}

void Orchestrator::finalize()
{
    if (m_finalized)
    {
        log::warn("Orchestrator is already finalized");
        return;
    }

    m_finalized = true;
    _orchestrators.push_back(this);
}

void Orchestrator::play()
{
    if (!m_finalized)
    {
        log::warn("Orchestrator must be finalized before playing");
        return;
    }

    if (m_steps.empty())
    {
        log::warn("Orchestrator has no steps to play");
        return;
    }

    if (!m_target)
    {
        log::warn("Orchestrator has no target transform");
        return;
    }

    rewind();
    m_playing = true;
}

void Orchestrator::pause()
{
    m_playing = false;
}

void Orchestrator::resume()
{
    if (!m_finalized)
    {
        log::warn("Orchestrator must be finalized before resuming");
        return;
    }
    m_playing = true;
}

void Orchestrator::stop()
{
    m_playing = false;
    rewind();
}

void Orchestrator::rewind()
{
    m_currentStep = 0;
    m_stepTime = 0.0;
    m_stepStarted = false;
}

bool Orchestrator::isFinalized() const
{
    return m_finalized;
}

bool Orchestrator::isPlaying() const
{
    return m_playing;
}

bool Orchestrator::isFinished() const
{
    return !m_playing && m_currentStep >= m_steps.size() && m_finalized;
}

void Orchestrator::setLooping(bool loop)
{
    m_looping = loop;
}

bool Orchestrator::isLooping() const
{
    return m_looping;
}

void Orchestrator::update(double dt)
{
    if (!m_playing || !m_target || m_steps.empty())
        return;

    if (m_currentStep >= m_steps.size())
    {
        if (m_looping)
        {
            rewind();
            m_playing = true;
        }
        else
        {
            m_playing = false;
        }
        return;
    }

    auto& step = m_steps[m_currentStep];

    // Start effects if this is the first frame of the step
    if (!m_stepStarted)
    {
        for (auto& effect : step.effects)
            effect->start(*m_target);
        m_stepStarted = true;
    }

    // Update all effects in this step
    m_stepTime += dt;
    for (auto& effect : step.effects)
    {
        const double effectProgress = effect->duration > 0.0
                                          ? std::min(m_stepTime / effect->duration, 1.0)
                                          : 1.0;
        effect->update(*m_target, effectProgress);
    }

    // Move to next step if current step is complete
    if (m_stepTime >= step.duration)
    {
        m_currentStep++;
        m_stepTime = 0.0;
        m_stepStarted = false;
    }
}

namespace fx
{
std::unique_ptr<Effect> moveTo(const Vec2& pos, double dur, const ease::EasingFunction& easeFunc)
{
    auto effect = std::make_unique<MoveToEffect>();
    effect->targetPos = pos;
    effect->duration = dur;
    effect->easing = easeFunc ? easeFunc : [](double t) { return t; };
    return effect;
}

std::unique_ptr<Effect> scaleTo(const Vec2& scale, double dur, const ease::EasingFunction& easeFunc)
{
    auto effect = std::make_unique<ScaleToEffect>();
    effect->targetScale = scale;
    effect->duration = dur;
    effect->easing = easeFunc ? easeFunc : [](double t) { return t; };
    return effect;
}

std::unique_ptr<Effect> scaleBy(double scale, double dur, const ease::EasingFunction& easeFunc)
{
    auto effect = std::make_unique<ScaleByEffect>();
    effect->deltaScale = Vec2{scale, scale};
    effect->duration = dur;
    effect->easing = easeFunc ? easeFunc : [](double t) { return t; };
    return effect;
}

std::unique_ptr<Effect> rotateTo(
    double angle, bool clockwise, double dur, const ease::EasingFunction& easeFunc
)
{
    auto effect = std::make_unique<RotateToEffect>();
    effect->targetAngle = angle;
    effect->clockwise = clockwise;
    effect->duration = dur;
    effect->easing = easeFunc ? easeFunc : [](double t) { return t; };
    return effect;
}

std::unique_ptr<Effect> rotateBy(
    double deltaAngle, bool clockwise, double dur, const ease::EasingFunction& easeFunc
)
{
    auto effect = std::make_unique<RotateByEffect>();
    effect->deltaAngle = deltaAngle;
    effect->clockwise = clockwise;
    effect->duration = dur;
    effect->easing = easeFunc ? easeFunc : [](double t) { return t; };
    return effect;
}

std::unique_ptr<Effect> shake(double amp, double freq, double dur)
{
    auto effect = std::make_unique<ShakeEffect>();
    effect->amplitude = amp;
    effect->frequency = freq;
    effect->duration = dur;
    return effect;
}

std::unique_ptr<Effect> call(const std::function<void()>& callback)
{
    auto effect = std::make_unique<CallEffect>();
    effect->callback = callback;
    effect->duration = 0.0;  // Instant
    return effect;
}

std::unique_ptr<Effect> wait(double dur)
{
    auto effect = std::make_unique<CallEffect>();
    effect->callback = nullptr;
    effect->duration = dur;
    return effect;
}
}  // namespace fx

namespace orchestrator
{
void _tick()
{
    const double dt = time::getDelta();
    for (auto* orch : _orchestrators)
        orch->update(dt);
}

}  // namespace orchestrator
}  // namespace kn

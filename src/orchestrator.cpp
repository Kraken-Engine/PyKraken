#include "Orchestrator.hpp"

#ifdef KRAKEN_ENABLE_PYTHON
#include <nanobind/stl/function.h>
#include <nanobind/stl/shared_ptr.h>
#endif  // KRAKEN_ENABLE_PYTHON

#include <algorithm>
#include <cmath>
#include <random>

#include "Log.hpp"
#include "Time.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace kn
{
static std::vector<Orchestrator*> _orchestrators;

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

class ScaleByEffect : public Effect
{
  public:
    Vec2 deltaScale;

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

// ----- ScaleToEffect -----
void ScaleToEffect::start(Transform& transform)
{
    m_startScale = transform.scale;
}

void ScaleToEffect::update(Transform& transform, double t)
{
    transform.scale = m_startScale + (targetScale - m_startScale) * easing(t);
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

// ----- ShakeEffect -----
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

Orchestrator& Orchestrator::parallel(const std::vector<std::shared_ptr<Effect>>& effects)
{
    if (m_finalized)
    {
        log::warn("Orchestrator is finalized, cannot add more steps");
        return *this;
    }

    Step step;
    step.effects = effects;

    // Duration is the max of all parallel effects
    for (const auto& effect : effects)
    {
        if (effect->duration > step.duration)
            step.duration = effect->duration;
    }

    m_steps.push_back(std::move(step));
    return *this;
}

Orchestrator& Orchestrator::then(std::shared_ptr<Effect> effect)
{
    return parallel(std::vector<std::shared_ptr<Effect>>{std::move(effect)});
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
std::shared_ptr<Effect> moveTo(const Vec2& pos, double dur, const ease::EasingFunction& easeFunc)
{
    auto effect = std::make_shared<MoveToEffect>();
    effect->targetPos = pos;
    effect->duration = dur;
    effect->easing = easeFunc ? easeFunc : [](double t) { return t; };
    return effect;
}

std::shared_ptr<Effect> scaleTo(const Vec2& scale, double dur, const ease::EasingFunction& easeFunc)
{
    auto effect = std::make_shared<ScaleToEffect>();
    effect->targetScale = scale;
    effect->duration = dur;
    effect->easing = easeFunc ? easeFunc : [](double t) { return t; };
    return effect;
}

std::shared_ptr<Effect> scaleBy(double scale, double dur, const ease::EasingFunction& easeFunc)
{
    auto effect = std::make_shared<ScaleByEffect>();
    effect->deltaScale = Vec2{scale, scale};
    effect->duration = dur;
    effect->easing = easeFunc ? easeFunc : [](double t) { return t; };
    return effect;
}

std::shared_ptr<Effect> rotateTo(
    double angle, bool clockwise, double dur, const ease::EasingFunction& easeFunc
)
{
    auto effect = std::make_shared<RotateToEffect>();
    effect->targetAngle = angle;
    effect->clockwise = clockwise;
    effect->duration = dur;
    effect->easing = easeFunc ? easeFunc : [](double t) { return t; };
    return effect;
}

std::shared_ptr<Effect> rotateBy(
    double deltaAngle, bool clockwise, double dur, const ease::EasingFunction& easeFunc
)
{
    auto effect = std::make_shared<RotateByEffect>();
    effect->deltaAngle = deltaAngle;
    effect->clockwise = clockwise;
    effect->duration = dur;
    effect->easing = easeFunc ? easeFunc : [](double t) { return t; };
    return effect;
}

std::shared_ptr<Effect> shake(double amp, double freq, double dur)
{
    auto effect = std::make_shared<ShakeEffect>();
    effect->amplitude = amp;
    effect->frequency = freq;
    effect->duration = dur;
    return effect;
}

std::shared_ptr<Effect> call(const std::function<void()>& callback)
{
    auto effect = std::make_shared<CallEffect>();
    effect->callback = callback;
    effect->duration = 0.0;  // Instant
    return effect;
}

std::shared_ptr<Effect> wait(double dur)
{
    auto effect = std::make_shared<CallEffect>();
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

#ifdef KRAKEN_ENABLE_PYTHON
void _bind(nb::module_& module)
{
    using namespace nb::literals;

    auto subFx = module.def_submodule("fx", R"doc(
Predefined effects for use with the Orchestrator.
    )doc");

    // ----- Effect base class (not directly instantiable) -----
    nb::class_<Effect>(subFx, "Effect", R"doc(
Base class for timeline effects. Not directly instantiable.
    )doc");

    // ----- Orchestrator -----
    nb::class_<Orchestrator>(module, "Orchestrator", R"doc(
Timeline animator for Transform objects.

Allows chaining effects to create complex animations that play over time.
Effects can run sequentially or in parallel.

Attributes:
    finalized (bool): Whether the orchestrator has been finalized.
    playing (bool): Whether the animation is currently playing.
    finished (bool): Whether the animation has completed.
    looping (bool): Whether the animation should loop when finished.

Methods:
    parallel(*effects): Add multiple effects to run in parallel.
    then(effect): Add a single effect to the timeline.
    finalize(): Finalize the orchestrator, preventing further edits.
    play(): Start playing the animation from the beginning.
    pause(): Pause the animation at the current position.
    resume(): Resume a paused animation.
    stop(): Stop the animation and reset to the beginning.
    rewind(): Reset the animation to the beginning without stopping.
    )doc")
        .def(nb::init<Transform&>(), "target"_a, R"doc(
Create an Orchestrator for animating transforms.

Args:
    target (Transform): The transform to animate.
             )doc")
        .def(
            "parallel",
            [](Orchestrator& self, const nb::args& effects) -> Orchestrator&
            {
                std::vector<std::shared_ptr<Effect>> effectVec;
                effectVec.reserve(effects.size());
                for (const auto& arg : effects)
                {
                    if (nb::isinstance<Effect>(arg))
                        effectVec.push_back(nb::cast<std::shared_ptr<Effect>>(arg));
                    else
                        throw nb::type_error("parallel() arguments must all be Effect objects");
                }

                return self.parallel(effectVec);
            },
            nb::rv_policy::reference,
            nb::sig("def parallel(self, *effects: fx.Effect) -> Orchestrator"), R"doc(
Add multiple effects to run in parallel.

Args:
    *effects: Variable number of Effect objects to run simultaneously.

Returns:
    Orchestrator: Self for method chaining.
             )doc"
        )
        .def("then", &Orchestrator::then, nb::rv_policy::reference, "effect"_a, R"doc(
Add a single effect to the timeline.

Args:
    effect: The Effect to add.

Returns:
    Orchestrator: Self for method chaining.
             )doc")
        .def("finalize", &Orchestrator::finalize, R"doc(
Finalize the orchestrator, preventing further edits.

Must be called before play(). Logs a warning if called multiple times.
             )doc")
        .def("play", &Orchestrator::play, R"doc(
Start playing the animation from the beginning.

Logs a warning if not finalized or if there are no steps.
             )doc")
        .def("pause", &Orchestrator::pause, R"doc(
Pause the animation at the current position.
             )doc")
        .def("resume", &Orchestrator::resume, R"doc(
Resume a paused animation.
             )doc")
        .def("stop", &Orchestrator::stop, R"doc(
Stop the animation and reset to the beginning.
             )doc")
        .def("rewind", &Orchestrator::rewind, R"doc(
Reset the animation to the beginning without stopping.
             )doc")
        .def_prop_ro("finalized", &Orchestrator::isFinalized, R"doc(
Whether the orchestrator has been finalized.
             )doc")
        .def_prop_ro("playing", &Orchestrator::isPlaying, R"doc(
Whether the animation is currently playing.
             )doc")
        .def_prop_ro("finished", &Orchestrator::isFinished, R"doc(
Whether the animation has completed.
             )doc")
        .def_prop_rw("looping", &Orchestrator::isLooping, &Orchestrator::setLooping, R"doc(
Whether the animation should loop when finished.
             )doc");

    // ----- fx functions (private, accessed via pykraken/fx.py) -----
    subFx.def("move_to", &fx::moveTo, "pos"_a, "dur"_a = 0.0, "ease"_a = nb::none(), R"doc(
Create a move-to effect.

Args:
    pos (Vec2): Target position.
    dur (float): Duration in seconds.
    ease (callable): Easing function (t -> t).

Returns:
    Effect: The move-to effect.
        )doc");

    subFx.def("scale_to", &fx::scaleTo, "scale"_a, "dur"_a = 0.0, "ease"_a = nb::none(), R"doc(
Create a scale-to effect.

Args:
    scale (Vec2): Target scale exact dimensions.
    dur (float): Duration in seconds.
    ease (callable): Easing function (t -> t).

Returns:
    Effect: The scale-to effect.
        )doc");

    subFx.def("scale_by", &fx::scaleBy, "scale"_a, "dur"_a = 0.0, "ease"_a = nb::none(), R"doc(
Create a scale-by effect.

Args:
    scale (float): Delta scalar to apply to the scale.
    dur (float): Duration in seconds.
    ease (callable): Easing function (t -> t).

Returns:
    Effect: The scale-by effect.
        )doc");

    subFx.def(
        "rotate_to", &fx::rotateTo, "angle"_a, "clockwise"_a = true, "dur"_a = 0.0,
        "ease"_a = nb::none(), R"doc(
Create a rotate-to effect.

Args:
    angle (float): Target angle in radians.
    clockwise (bool): Direction of rotation. True for clockwise, False for counterclockwise.
    dur (float): Duration in seconds.
    ease (callable): Easing function (t -> t).

Returns:
    Effect: The rotate-to effect.
        )doc"
    );

    subFx.def(
        "rotate_by", &fx::rotateBy, "delta"_a, "clockwise"_a = true, "dur"_a = 0.0,
        "ease"_a = nb::none(), R"doc(
Create a rotate-by effect.

Args:
    delta (float): Delta angle in radians to rotate by in radians.
    clockwise (bool): Direction of rotation. True for clockwise, False for counterclockwise.
    dur (float): Duration in seconds.
    ease (callable): Easing function (t -> t).

Returns:
    Effect: The rotate-by effect.
        )doc"
    );

    subFx.def("shake", &fx::shake, "amp"_a, "freq"_a, "dur"_a, R"doc(
Create a shake effect.

Args:
    amp (float): Shake amplitude in pixels.
    freq (float): Shake frequency in Hz.
    dur (float): Duration in seconds.

Returns:
    Effect: The shake effect.
        )doc");

    subFx.def("call", &fx::call, "callback"_a, R"doc(
Create an effect that calls a function.

Args:
    callback (callable): Function to call when this step is reached.

Returns:
    Effect: The call effect.
        )doc");

    subFx.def("wait", &fx::wait, "dur"_a, R"doc(
Create a wait/delay effect.

Args:
    dur (float): Duration to wait in seconds.

Returns:
    Effect: The wait effect.
        )doc");
}
#endif  // KRAKEN_ENABLE_PYTHON

}  // namespace orchestrator
}  // namespace kn

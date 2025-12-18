#include "Orchestrator.hpp"
#include "Log.hpp"
#include "Time.hpp"

#include <cmath>
#include <pybind11/functional.h>
#include <pybind11/stl.h>
#include <random>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace kn
{
static std::vector<Orchestrator*> _orchestrators;

// ----- MoveToEffect -----
void MoveToEffect::start(Transform& transform) { m_startPos = transform.pos; }

void MoveToEffect::update(Transform& transform, double t)
{
    const double easedT = easing(t);
    transform.pos.x = m_startPos.x + (targetPos.x - m_startPos.x) * easedT;
    transform.pos.y = m_startPos.y + (targetPos.y - m_startPos.y) * easedT;
}

// ----- ScaleToEffect -----
void ScaleToEffect::start(Transform& transform) { m_startScale = transform.scale; }

void ScaleToEffect::update(Transform& transform, double t)
{
    const double easedT = easing(t);
    transform.scale.x = m_startScale.x + (targetScale.x - m_startScale.x) * easedT;
    transform.scale.y = m_startScale.y + (targetScale.y - m_startScale.y) * easedT;
}

// ----- RotateToEffect -----
void RotateToEffect::start(Transform& transform) { m_startAngle = transform.angle; }

void RotateToEffect::update(Transform& transform, double t)
{
    const double easedT = easing(t);
    transform.angle = m_startAngle + (targetAngle - m_startAngle) * static_cast<float>(easedT);
}

// ----- ShakeEffect -----
void ShakeEffect::start(Transform& transform) { m_originalPos = transform.pos; }

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

    const Vec2 phase = {std::sin(time * frequency * 2.0 * M_PI + dist(rng) * 0.5),
                        std::sin(time * frequency * 2.0 * M_PI * 1.1 + dist(rng) * 0.5)};

    transform.pos = m_originalPos + amplitude * decay * phase;
}

// ----- CallEffect -----
void CallEffect::start([[maybe_unused]] Transform& transform) { m_called = false; }

void CallEffect::update([[maybe_unused]] Transform& transform, [[maybe_unused]] double t)
{
    if (!m_called && callback)
    {
        callback();
        m_called = true;
    }
}

// ----- Orchestrator -----
Orchestrator::~Orchestrator() { std::erase(_orchestrators, this); }

void Orchestrator::setTarget(Transform* target) { m_target = target; }

Orchestrator& Orchestrator::addStep(const std::vector<std::shared_ptr<Effect>>& effects)
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

Orchestrator& Orchestrator::addStep(std::shared_ptr<Effect> effect)
{
    return addStep(std::vector<std::shared_ptr<Effect>>{std::move(effect)});
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

void Orchestrator::pause() { m_playing = false; }

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

bool Orchestrator::isFinalized() const { return m_finalized; }

bool Orchestrator::isPlaying() const { return m_playing; }

bool Orchestrator::isFinished() const
{
    return !m_playing && m_currentStep >= m_steps.size() && m_finalized;
}

void Orchestrator::setLooping(bool loop) { m_looping = loop; }

bool Orchestrator::isLooping() const { return m_looping; }

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
        const double effectProgress =
            effect->duration > 0.0 ? std::min(m_stepTime / effect->duration, 1.0) : 1.0;
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

namespace orchestrator
{
void _tick()
{
    const double dt = time::getDelta();
    for (auto* orch : _orchestrators)
        orch->update(dt);
}

void _bind(py::module_& module)
{
    // ----- Effect base class (not directly instantiable) -----
    py::class_<Effect, std::shared_ptr<Effect>>(module, "Effect", R"doc(
Base class for timeline effects. Not directly instantiable.
    )doc");

    // ----- Orchestrator -----
    py::classh<Orchestrator>(module, "Orchestrator", R"doc(
Timeline animator for Transform objects.

Allows chaining effects to create complex animations that play over time.
Effects can run sequentially or in parallel.
    )doc")
        .def(py::init(
                 [](const py::object& target) -> Orchestrator
                 {
                     Orchestrator orch;

                     if (target.is_none())
                         throw py::type_error(
                             "target must be a Transform or an object with a 'transform' "
                             "attribute, not None");

                     // Try to get transform attribute first (for Sprite-like objects)
                     if (py::hasattr(target, "transform"))
                     {
                         auto transformAttr = target.attr("transform");
                         auto* transform = transformAttr.cast<Transform*>();
                         orch.setTarget(transform);
                     }
                     else
                     {
                         // Try direct Transform cast
                         try
                         {
                             auto* transform = target.cast<Transform*>();
                             orch.setTarget(transform);
                         }
                         catch (const py::cast_error&)
                         {
                             throw py::type_error(
                                 "target must be a Transform or an object with a 'transform' "
                                 "attribute");
                         }
                     }
                     return orch;
                 }),
             py::arg("target"),
             R"doc(
Create an Orchestrator for animating transforms.

Args:
    target: Either a Transform object or an object with a 'transform' attribute (like Sprite).
             )doc")
        .def(
            "parallel",
            [](Orchestrator& self, const py::args& effects) -> Orchestrator&
            {
                std::vector<std::shared_ptr<Effect>> effectVec;
                for (const auto& arg : effects)
                {
                    try
                    {
                        effectVec.push_back(arg.cast<std::shared_ptr<Effect>>());
                    }
                    catch (const py::cast_error&)
                    {
                        throw py::type_error("parallel() arguments must be Effect objects");
                    }
                }
                return self.addStep(effectVec);
            },
            R"doc(
Add multiple effects to run in parallel.

Args:
    *effects: Variable number of Effect objects to run simultaneously.

Returns:
    Orchestrator: Self for method chaining.
             )doc")
        .def(
            "then", [](Orchestrator& self, const std::shared_ptr<Effect>& effect) -> Orchestrator&
            { return self.addStep(effect); }, py::arg("effect"), R"doc(
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
        .def_property_readonly("finalized", &Orchestrator::isFinalized, R"doc(
Whether the orchestrator has been finalized.
             )doc")
        .def_property_readonly("playing", &Orchestrator::isPlaying, R"doc(
Whether the animation is currently playing.
             )doc")
        .def_property_readonly("finished", &Orchestrator::isFinished, R"doc(
Whether the animation has completed.
             )doc")
        .def_property("looping", &Orchestrator::isLooping, &Orchestrator::setLooping, R"doc(
Whether the animation should loop when finished.
             )doc");

    // ----- fx functions (private, accessed via pykraken/fx.py) -----
    module.def(
        "_fx_move_to",
        [](const py::object& posObj, double dur,
           const py::object& easeObj) -> std::shared_ptr<Effect>
        {
            auto effect = std::make_shared<MoveToEffect>();
            if (!posObj.is_none())
            {
                try
                {
                    effect->targetPos = posObj.cast<Vec2>();
                }
                catch (const py::cast_error&)
                {
                    throw py::type_error("pos must be a Vec2");
                }
            }
            effect->duration = dur;
            if (!easeObj.is_none())
            {
                try
                {
                    effect->easing = easeObj.cast<std::function<double(double)>>();
                }
                catch (const py::cast_error&)
                {
                    throw py::type_error("ease must be a callable (float) -> float");
                }
            }
            return effect;
        },
        py::arg("pos") = py::none(), py::arg("dur") = 0.0, py::arg("ease") = py::none(),
        R"doc(
Create a move-to effect.

Args:
    pos (Vec2): Target position.
    dur (float): Duration in seconds.
    ease (callable): Easing function (t -> t).

Returns:
    Effect: The move-to effect.
        )doc");

    module.def(
        "_fx_scale_to",
        [](const py::object& scaleObj, double dur,
           const py::object& easeObj) -> std::shared_ptr<Effect>
        {
            auto effect = std::make_shared<ScaleToEffect>();
            if (scaleObj.is_none())
            {
                effect->targetScale = {1.0, 1.0};
            }
            else if (py::isinstance<py::float_>(scaleObj) || py::isinstance<py::int_>(scaleObj))
            {
                double s = scaleObj.cast<double>();
                effect->targetScale = {s, s};
            }
            else
            {
                try
                {
                    effect->targetScale = scaleObj.cast<Vec2>();
                }
                catch (const py::cast_error&)
                {
                    throw py::type_error("scale must be a number or Vec2");
                }
            }
            effect->duration = dur;
            if (!easeObj.is_none())
            {
                try
                {
                    effect->easing = easeObj.cast<std::function<double(double)>>();
                }
                catch (const py::cast_error&)
                {
                    throw py::type_error("ease must be a callable (float) -> float");
                }
            }
            return effect;
        },
        py::arg("scale") = py::none(), py::arg("dur") = 0.0, py::arg("ease") = py::none(),
        R"doc(
Create a scale-to effect.

Args:
    scale (float or Vec2): Target scale. A single number applies to both axes.
    dur (float): Duration in seconds.
    ease (callable): Easing function (t -> t).

Returns:
    Effect: The scale-to effect.
        )doc");

    module.def(
        "_fx_rotate_to",
        [](float angle, double dur, const py::object& easeObj) -> std::shared_ptr<Effect>
        {
            auto effect = std::make_shared<RotateToEffect>();
            effect->targetAngle = angle;
            effect->duration = dur;
            if (!easeObj.is_none())
            {
                try
                {
                    effect->easing = easeObj.cast<std::function<double(double)>>();
                }
                catch (const py::cast_error&)
                {
                    throw py::type_error("ease must be a callable (float) -> float");
                }
            }
            return effect;
        },
        py::arg("angle"), py::arg("dur") = 0.0, py::arg("ease") = py::none(),
        R"doc(
Create a rotate-to effect.

Args:
    angle (float): Target angle in radians.
    dur (float): Duration in seconds.
    ease (callable): Easing function (t -> t).

Returns:
    Effect: The rotate-to effect.
        )doc");

    module.def(
        "_fx_shake",
        [](double amp, double freq, double dur) -> std::shared_ptr<Effect>
        {
            auto effect = std::make_shared<ShakeEffect>();
            effect->amplitude = amp;
            effect->frequency = freq;
            effect->duration = dur;
            return effect;
        },
        py::arg("amp"), py::arg("freq"), py::arg("dur"),
        R"doc(
Create a shake effect.

Args:
    amp (float): Shake amplitude in pixels.
    freq (float): Shake frequency in Hz.
    dur (float): Duration in seconds.

Returns:
    Effect: The shake effect.
        )doc");

    module.def(
        "_fx_call",
        [](const std::function<void()>& callback) -> std::shared_ptr<Effect>
        {
            auto effect = std::make_shared<CallEffect>();
            effect->callback = callback;
            effect->duration = 0.0; // Instant
            return effect;
        },
        py::arg("callback"), R"doc(
Create an effect that calls a function.

Args:
    callback (callable): Function to call when this step is reached.

Returns:
    Effect: The call effect.
        )doc");

    module.def(
        "_fx_wait",
        [](double dur) -> std::shared_ptr<Effect>
        {
            // Create a dummy effect that just waits
            auto effect = std::make_shared<CallEffect>();
            effect->callback = nullptr;
            effect->duration = dur;
            return effect;
        },
        py::arg("dur"), R"doc(
Create a wait/delay effect.

Args:
    dur (float): Duration to wait in seconds.

Returns:
    Effect: The wait effect.
        )doc");
}
} // namespace orchestrator
} // namespace kn

#include "kraken/core/Time.hpp"

#include <SDL3/SDL.h>

#include <algorithm>
#include <limits>

namespace kn
{
static uint64_t _lastTick = 0;
static double _fps = 0.0;
static uint16_t _frameTarget = 0;
static double _delta = 0.0;
static double _maxDelta = std::numeric_limits<double>::infinity();
static double _scale = 1.0;

Timer::Timer(const double duration)
{
    setDuration(duration);
}

void Timer::start()
{
    m_startTime = std::chrono::steady_clock::now();
    m_started = true;
    m_paused = false;
    m_elapsedPausedTime = 0.0;  // Reset accumulated pause time when starting
}

void Timer::pause()
{
    if (m_started && !m_paused)
    {
        m_pauseTime = std::chrono::steady_clock::now();
        m_paused = true;
    }
}

void Timer::resume()
{
    if (m_started && m_paused)
    {
        const auto now = std::chrono::steady_clock::now();
        const auto pauseDuration = now - m_pauseTime;
        m_elapsedPausedTime += std::chrono::duration<double>(pauseDuration).count();
        m_paused = false;
    }
}

void Timer::reset()
{
    m_started = false;
    m_paused = false;
    m_elapsedPausedTime = 0.0;
}

void Timer::restart()
{
    reset();
    start();
}

bool Timer::isDone() const
{
    if (!m_started)
        return false;

    const auto now = std::chrono::steady_clock::now();
    const std::chrono::duration<double> totalElapsed = now - m_startTime;

    double effectiveElapsed;
    if (m_paused)
    {
        // If currently paused, don't count time since pause started
        const std::chrono::duration<double> pausedDuration = now - m_pauseTime;
        effectiveElapsed = totalElapsed.count() - m_elapsedPausedTime - pausedDuration.count();
    }
    else
    {
        // Not currently paused, just subtract accumulated pause time
        effectiveElapsed = totalElapsed.count() - m_elapsedPausedTime;
    }

    return effectiveElapsed >= m_duration;
}

double Timer::timeRemaining() const
{
    if (!m_started)
        return m_duration;

    const auto now = std::chrono::steady_clock::now();
    const std::chrono::duration<double> totalElapsed = now - m_startTime;

    double effectiveElapsed;
    if (m_paused)
    {
        // If currently paused, don't count time since pause started
        const std::chrono::duration<double> pausedDuration = now - m_pauseTime;
        effectiveElapsed = totalElapsed.count() - m_elapsedPausedTime - pausedDuration.count();
    }
    else
    {
        // Not currently paused, just subtract accumulated pause time
        effectiveElapsed = totalElapsed.count() - m_elapsedPausedTime;
    }

    const double remaining = m_duration - effectiveElapsed;
    return remaining > 0.0 ? remaining : 0.0;
}

double Timer::elapsedTime() const
{
    if (!m_started)
        return 0.0;

    const auto now = std::chrono::steady_clock::now();
    const std::chrono::duration<double> totalElapsed = now - m_startTime;

    double effectiveElapsed;
    if (m_paused)
    {
        // If currently paused, don't count time since pause started
        const std::chrono::duration<double> pausedDuration = now - m_pauseTime;
        effectiveElapsed = totalElapsed.count() - m_elapsedPausedTime - pausedDuration.count();
    }
    else
    {
        // Not currently paused, just subtract accumulated pause time
        effectiveElapsed = totalElapsed.count() - m_elapsedPausedTime;
    }

    return std::max(effectiveElapsed, 0.0);
}

double Timer::progress() const
{
    if (!m_started)
        return 0.0;

    const double elapsed = elapsedTime();
    return std::min(elapsed / m_duration, 1.0);
}

double Timer::getDuration() const
{
    return m_duration;
}

void Timer::setDuration(const double duration)
{
    if (duration <= 0)
    {
        throw std::invalid_argument("Timer duration must be greater than 0");
    }

    m_duration = duration;
}

namespace time
{
double getDelta()
{
    return _delta;
}

void setMaxDelta(const double maxDelta)
{
    if (maxDelta <= 0.0)
        throw std::invalid_argument("Max delta must be greater than 0.0");
    _maxDelta = maxDelta;
}

double getFPS()
{
    return _fps;
}

void setTarget(const uint16_t frameRate)
{
    _frameTarget = frameRate;
}

double getElapsed()
{
    return static_cast<double>(SDL_GetTicksNS()) / SDL_NS_PER_SECOND;
}

void delay(const uint64_t ms)
{
    SDL_Delay(ms);
}

void setScale(double scale)
{
    if (scale < 0.0)
        scale = 0.0;
    _scale = scale;
}

double getScale()
{
    return _scale;
}

void _tick()
{
    uint64_t now = SDL_GetTicksNS();

    // Stable first frame
    if (_lastTick == 0)
    {
        _lastTick = now;
        _delta = 0.0;
        _fps = 0;
        return;
    }

    uint64_t frameTime = now - _lastTick;

    if (_frameTarget > 0)
    {
        const uint64_t targetFrameTimeNS = SDL_NS_PER_SECOND / _frameTarget;
        if (frameTime < targetFrameTimeNS)
        {
            SDL_DelayNS(targetFrameTimeNS - frameTime);
            now = SDL_GetTicksNS();
            frameTime = now - _lastTick;
        }
    }

    _lastTick = now;
    _delta = static_cast<double>(frameTime) / SDL_NS_PER_SECOND;

    // Prevent division by zero and handle very small delta times
    _fps = _delta > 0.0 ? 1.0 / _delta : 0.0;

    // Cap delta
    if (_delta > _maxDelta)
        _delta = _maxDelta;

    _delta *= _scale;
}

}  // namespace time
}  // namespace kn

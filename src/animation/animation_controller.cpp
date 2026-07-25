
#include <algorithm>
#include <filesystem>

#include "kraken/animation/AnimationController.hpp"
#include "kraken/core/Log.hpp"
#include "kraken/core/Time.hpp"
#include "kraken/graphics/Renderer.hpp"
#include "kraken/graphics/Texture.hpp"
#include "kraken/math/Math.hpp"

namespace fs = std::filesystem;

namespace kn
{
static std::vector<AnimationController*> _controllers;

AnimationController::AnimationController()
{
    _controllers.push_back(this);
}
AnimationController::~AnimationController()
{
    std::erase(_controllers, this);
}

void AnimationController::addSheet(
    const int frameWidth, const int frameHeight, const std::vector<SheetStrip>& strips
)
{
    if (frameWidth <= 0 || frameHeight <= 0)
        throw std::invalid_argument("Frame size must be positive non-zero values");
    if (strips.empty())
        throw std::invalid_argument("No strips provided for animation controller");

    for (size_t stripIndex = 0; stripIndex < strips.size(); ++stripIndex)
    {
        const auto& strip = strips[stripIndex];

        const std::string& name = strip.name;
        if (m_animMap.contains(name))
            throw std::runtime_error("Animation duplicate: " + name);

        if (strip.frameCount <= 0)
            throw std::invalid_argument("Frame count must be positive for strip: " + name);

        const int y = static_cast<int>(stripIndex) * frameHeight;

        // Extract only the specified number of frames from this strip/row
        Animation newAnim;
        newAnim.fps = strip.fps;
        for (int i = 0; i < strip.frameCount; ++i)
        {
            const int x = i * frameWidth;
            newAnim.frames.emplace_back(x, y, frameWidth, frameHeight);
        }

        m_animMap[name] = std::move(newAnim);
        m_currAnim = name;
    }
}

void AnimationController::set(const std::string& name)
{
    if (!m_animMap.contains(name))
        throw std::invalid_argument("Animation not found: " + name);

    m_currAnim = name;
}

void AnimationController::play(const std::string& name)
{
    set(name);
    rewind();
    resume();
}

void AnimationController::playFrom(const int frameIndex)
{
    const auto& [frames, _] = m_animMap.at(m_currAnim);

    if (frameIndex < 0 || frameIndex >= static_cast<int>(frames.size()))
        throw std::out_of_range("Frame index out of range: " + std::to_string(frameIndex));

    m_index = static_cast<double>(frameIndex);
    m_prevIndex = m_index;
    resume();
}

const Rect& AnimationController::getCurrentClip() const
{
    const auto& [frames, _] = m_animMap.at(m_currAnim);

    // Clamp index to valid range to prevent out-of-bounds access
    const auto frameIndex = static_cast<size_t>(std::max(0.0, std::floor(m_index)));
    const auto safeIndex = std::min(frameIndex, frames.size() - 1);

    return frames.at(safeIndex);
}

void AnimationController::setPlaybackSpeed(const double speed)
{
    m_playbackSpeed = speed;
    if (speed == 0)
        pause();
}

double AnimationController::getPlaybackSpeed() const
{
    return m_playbackSpeed;
}

bool AnimationController::isLooping() const
{
    return m_looping;
}

void AnimationController::setLooping(const bool loop)
{
    m_looping = loop;
}

bool AnimationController::isFinished() const
{
    return m_prevIndex > m_index;
}

std::string AnimationController::getCurrentAnimationName() const
{
    return m_currAnim;
}

int AnimationController::getFrameIndex() const
{
    return static_cast<int>(std::floor(m_index));
}

double AnimationController::getProgress() const
{
    const auto& [frames, _] = m_animMap.at(m_currAnim);
    const auto frameCount = static_cast<double>(frames.size());

    if (frameCount == 0.0)
        return 0.0;

    return m_index / frameCount;
}

void AnimationController::rewind()
{
    m_index = 0.0;
    m_prevIndex = 0.0;
}

void AnimationController::pause()
{
    m_paused = true;
}

void AnimationController::resume()
{
    if (m_playbackSpeed > 0.0)
        m_paused = false;
}

void AnimationController::update(const double delta)
{
    if (m_paused)
        return;
    if (m_currAnim.empty())
        return;

    const auto& [frames, fps] = m_animMap.at(m_currAnim);

    // Store previous index for loop detection
    m_prevIndex = m_index;

    m_index += delta * fps * m_playbackSpeed;
    const auto frameCount = static_cast<double>(frames.size());

    if (m_looping)
    {
        // Loop back to the beginning
        m_index = fmod(m_index + frameCount, frameCount);
        return;
    }

    // When not looping, clamp to the ends and pause
    if (m_index >= frameCount)
    {
        m_index = frameCount;
        pause();
    }
    else if (m_index < 0.0)
    {
        m_index = 0.0;
        pause();
    }
}

namespace animation_controller
{
void _tick()
{
    const double delta = time::getDelta();
    for (const auto& controller : _controllers)
        controller->update(delta);
}

}  // namespace animation_controller
}  // namespace kn

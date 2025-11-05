#pragma once

#include <memory>
#include <pybind11/pybind11.h>
#include <unordered_map>
#include <vector>

#include "Rect.hpp"

namespace kn
{
class Texture;

namespace animation_controller
{
void _bind(const py::module_& module);

void _tick();
} // namespace animation_controller

struct Frame
{
    std::shared_ptr<Texture> tex;
    Rect src;
};

struct Animation
{
    std::vector<Frame> frames;
    int fps;
};

struct SheetStrip
{
    std::string name;
    int frameCount;
    int fps;
};

class AnimationController
{
  public:
    AnimationController();
    ~AnimationController();

    void loadSpriteSheet(const std::string& filePath, const Vec2& frameSize,
                         const std::vector<SheetStrip>& strips);

    void set(const std::string& name);
    void play(const std::string& name);
    void playFrom(int frameIndex);

    [[nodiscard]] const Frame& getCurrentFrame() const;
    [[nodiscard]] int getFrameIndex() const;
    [[nodiscard]] double getProgress() const;

    void setPlaybackSpeed(double speed);
    [[nodiscard]] double getPlaybackSpeed() const;

    void setLooping(bool loop);
    [[nodiscard]] bool isLooping() const;

    [[nodiscard]] bool isFinished() const;

    [[nodiscard]] std::string getCurrentAnimationName() const;

    void rewind();
    void pause();
    void resume();

  private:
    double m_playbackSpeed = 1.0;
    double m_index = 0.0;
    double m_prevIndex = 0.0;
    bool m_paused = false;
    bool m_looping = true;
    std::string m_currAnim;
    std::unordered_map<std::string, Animation> m_animMap;

    void update(double delta);

    friend void animation_controller::_tick();
};
} // namespace kn
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
    Rect rect;
};

struct Animation
{
    std::vector<Frame> frames;
    int fps;
};

class AnimationController
{
  public:
    AnimationController();
    ~AnimationController();

    void loadSpriteSheet(const std::string& name, const std::string& filePath,
                         const Vec2& frameSize, int fps);
    void loadFolder(const std::string& name, const std::string& dirPath, int fps);

    void remove(const std::string& name);
    void set(const std::string& name, bool rewind = false);

    [[nodiscard]] const Frame& getCurrentFrame() const;

    void setPlaybackSpeed(double speed);
    [[nodiscard]] double getPlaybackSpeed() const;

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
    std::string m_currAnim;
    std::unordered_map<std::string, Animation> m_animMap;

    void update(double delta);

    friend void animation_controller::_tick();
};
} // namespace kn
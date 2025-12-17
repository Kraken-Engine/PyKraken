#pragma once

#include <memory>
#include <optional>
#include <pybind11/pybind11.h>

#include "Math.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"

namespace py = pybind11;

namespace kn
{
/**
 * @brief Abstract base class for drawable game objects with a texture and transform.
 * @note This class cannot be instantiated directly; inherit from it and implement update().
 */
class Sprite
{
  public:
    Transform transform{};
    Vec2 velocity{0.0f, 0.0f};
    std::optional<Rect> clip{};  // Source rectangle for texture sampling (nullopt = full texture)
    std::shared_ptr<Texture> texture = nullptr;
    bool visible = true;

    Sprite() = default;
    explicit Sprite(std::shared_ptr<Texture> texture);
    Sprite(std::shared_ptr<Texture> texture, const Transform& transform);
    virtual ~Sprite() = default;

    /**
     * @brief Draw the sprite to the screen.
     * @note This method is non-virtual and should not be overridden.
     */
    void draw() const;

    /**
     * @brief Update the sprite state. Must be implemented by derived classes.
     * @param dt Delta time since last update.
     */
    virtual void update() = 0;

    /**
     * @brief Move the sprite by applying velocity to position.
     */
    void move();
};

namespace sprite
{
void _bind(py::module_& module);
} // namespace sprite
} // namespace kn

#include "Sprite.hpp"

#include "Time.hpp"

namespace kn
{
Sprite::Sprite(std::shared_ptr<Texture> texture)
    : texture(std::move(texture))
{
}

Sprite::Sprite(std::shared_ptr<Texture> texture, const Transform& transform)
    : transform(transform),
      texture(std::move(texture))
{
}

void Sprite::draw() const
{
    if (!visible || !texture)
        return;

    renderer::draw(*texture, transform);
}

void Sprite::move()
{
    transform.pos += velocity * time::getDelta();
}

// Trampoline class to allow Python inheritance from abstract Sprite class
class PySprite : public Sprite, public py::trampoline_self_life_support
{
  public:
    using Sprite::Sprite;  // Inherit constructors

    void update() override
    {
        PYBIND11_OVERRIDE_PURE(void, Sprite, update);
    }
};

namespace sprite
{
void _bind(py::module_& module)
{
    py::classh<Sprite, PySprite>(module, "Sprite", R"doc(
        Abstract base class for drawable game objects with a texture and transform.

        This class cannot be instantiated directly. Inherit from it and implement
        the update() method.

        Attributes:
            transform (Transform): The sprite's position, rotation, and scale.
            velocity (Vec2): The sprite's velocity vector.
            texture (Texture): The sprite's texture (can be None).
            visible (bool): Whether the sprite should be drawn.

        Methods:
            draw(): Draw the sprite to the screen.
            update(): Update the sprite state (must be overridden).
            move(): Apply frame-independent velocity to position.
    )doc")
        .def(py::init<>(), "Create a sprite with no texture yet.")
        .def(
            py::init<std::shared_ptr<Texture>>(), py::arg("texture"),
            R"doc(
             Create a sprite with a texture.

             Args:
                 texture (Texture): The sprite's texture.
             )doc"
        )
        .def(
            py::init<std::shared_ptr<Texture>, const Transform&>(), py::arg("texture"),
            py::arg("transform"),
            R"doc(
             Create a sprite with a texture and transform.

                Args:
                    texture (Texture): The sprite's texture.
                    transform (Transform): The sprite's initial transform.
             )doc"
        )

        .def_readwrite("transform", &Sprite::transform, "The sprite's transform.")
        .def_readwrite("velocity", &Sprite::velocity, "The sprite's velocity.")
        .def_readwrite("texture", &Sprite::texture, "The sprite's texture.")
        .def_readwrite("visible", &Sprite::visible, "Whether the sprite is visible.")

        .def("draw", &Sprite::draw, "Draw the sprite to the screen with its current transform.")
        .def("update", &Sprite::update, "Update the sprite state (must be overridden).")
        .def("move", &Sprite::move, "Apply frame-independent velocity to position.");
}
}  // namespace sprite
}  // namespace kn

#pragma once

#include <box2d/box2d.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace kn
{
class Vec2;

namespace physics
{
class Body;
enum class BodyType;

void _bind(py::module_& module);

class World
{
  public:
    explicit World(const Vec2& gravity);
    ~World();

    Body createBody(BodyType type);
    void destroyBody(Body& body);

    void step(float timeStep, int subStepCount);

    void setGravity(const Vec2& gravity);
    Vec2 getGravity() const;

    bool isValid() const;

  private:
    b2WorldId m_worldId = b2_nullWorldId;

    void _checkValid() const;
};
}  // namespace physics
}  // namespace kn

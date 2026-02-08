#pragma once

#include <box2d/box2d.h>
#include <pybind11/pybind11.h>

#include <vector>

#include "Math.hpp"
#include "physics/Body.hpp"

namespace py = pybind11;

namespace kn
{
class Rect;

namespace physics
{
class DistanceJoint;
class FilterJoint;
class MotorJoint;
class TargetJoint;
class PrismaticJoint;
class RevoluteJoint;
class WeldJoint;
class WheelJoint;

void _bind(py::module_& module);

struct Collision
{
    Body bodyA;
    Body bodyB;
    Vec2 point;
    Vec2 normal;
    float approachSpeed;
};

class World
{
  public:
    explicit World(const Vec2& gravity);
    ~World();

    Body createBody(BodyType type);

    // Joints
    DistanceJoint createDistanceJoint(
        const Body& bodyA, const Body& bodyB, const Vec2& anchorA, const Vec2& anchorB
    );
    FilterJoint createFilterJoint(const Body& bodyA, const Body& bodyB);
    MotorJoint createMotorJoint(const Body& bodyA, const Body& bodyB);
    TargetJoint createTargetJoint(
        const Body& groundBody, const Body& pulledBody, const Vec2& target
    );
    PrismaticJoint createPrismaticJoint(
        const Body& bodyA, const Body& bodyB, const Vec2& anchor, const Vec2& axis
    );
    RevoluteJoint createRevoluteJoint(const Body& bodyA, const Body& bodyB, const Vec2& anchor);
    WeldJoint createWeldJoint(const Body& bodyA, const Body& bodyB, const Vec2& anchor);
    WheelJoint createWheelJoint(
        const Body& bodyA, const Body& bodyB, const Vec2& anchor, const Vec2& axis
    );

    void step(float timeStep, int subStepCount);

    std::vector<Collision> getCollisions();

    std::vector<Body> queryPoint(const Vec2& point);
    std::vector<Body> queryAABB(const Rect& rect);

    void setGravity(const Vec2& gravity);
    Vec2 getGravity() const;

    bool isValid() const;

  private:
    b2WorldId m_worldId = b2_nullWorldId;

    struct QueryContext
    {
        std::vector<Body> bodies;
        Vec2 point;
        bool isPointQuery = false;
    };

    static bool QueryCallback(b2ShapeId shapeId, void* context);

    void _checkValid() const;
    void _checkBodiesForJoint(const Body& bodyA, const Body& bodyB) const;
};
}  // namespace physics
}  // namespace kn

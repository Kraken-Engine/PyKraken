#pragma once

#include <box2d/box2d.h>
#include <pybind11/pybind11.h>

#include <functional>
#include <vector>

#include "Math.hpp"
#include "physics/bodies/Body.hpp"
#include "physics/bodies/CharacterBody.hpp"
#include "physics/bodies/RigidBody.hpp"
#include "physics/bodies/StaticBody.hpp"

namespace py = pybind11;

namespace kn
{
class Rect;
class Circle;
class Capsule;
class Polygon;
struct Transform;

namespace tilemap
{
class Layer;
}

namespace physics
{
void _bind(py::module_& module);
void _tick();

void setFixedDelta(float fixedDelta);
float getFixedDelta();
void setMaxSubsteps(int maxSubsteps);
int getMaxSubsteps();

class DistanceJoint;
class FilterJoint;
class MotorJoint;
class MouseJoint;
class PrismaticJoint;
class RevoluteJoint;
class WeldJoint;
class WheelJoint;

struct Collision
{
    Body bodyA;
    Body bodyB;
    Vec2 point;
    Vec2 normal;
    float approachSpeed;
};

struct CastHit
{
    Body body;
    Vec2 point;
    Vec2 normal;
    float fraction = 0.0f;
};

class World
{
  public:
    explicit World(const Vec2& gravity);
    ~World();

    // Joints
    DistanceJoint createDistanceJoint(
        const Body& bodyA, const Body& bodyB, const Vec2& anchorA, const Vec2& anchorB
    );
    FilterJoint createFilterJoint(const Body& bodyA, const Body& bodyB);
    MotorJoint createMotorJoint(const Body& bodyA, const Body& bodyB);
    MouseJoint createMouseJoint(const Body& groundBody, const Body& pulledBody, const Vec2& target);
    PrismaticJoint createPrismaticJoint(
        const Body& bodyA, const Body& bodyB, const Vec2& anchor, const Vec2& axis
    );
    RevoluteJoint createRevoluteJoint(const Body& bodyA, const Body& bodyB, const Vec2& anchor);
    WeldJoint createWeldJoint(const Body& bodyA, const Body& bodyB, const Vec2& anchor);
    WheelJoint createWheelJoint(
        const Body& bodyA, const Body& bodyB, const Vec2& anchor, const Vec2& axis
    );

    void step(float timeStep, int subStepCount);

    // Queries
    std::vector<Collision> getCollisions();
    std::vector<Body> queryPoint(const Vec2& point);
    std::vector<Body> queryAABB(const Rect& rect);

    // Ray and Shape casts
    std::vector<CastHit> rayCast(const Vec2& origin, const Vec2& translation);
    std::vector<CastHit> shapeCast(
        const Circle& circle, const Transform& transform, const Vec2& translation
    );
    std::vector<CastHit> shapeCast(
        const Capsule& capsule, const Transform& transform, const Vec2& translation
    );
    std::vector<CastHit> shapeCast(
        const Polygon& polygon, const Transform& transform, const Vec2& translation
    );
    std::vector<CastHit> shapeCast(
        const Rect& rect, const Transform& transform, const Vec2& translation
    );

    StaticBody fromMapLayer(const tilemap::Layer& layer);

    void setGravity(const Vec2& gravity);
    Vec2 getGravity() const;

    bool isValid() const;

    void addFixedUpdate(py::object callback);
    void clearFixedUpdates();

    b2WorldId _getWorldId() const;

  private:
    struct FixedUpdateCallback
    {
        py::object callback;
        py::weakref weakOwner;
        py::object unboundMethod;
        bool isBound = false;
    };

    b2WorldId m_worldId = b2_nullWorldId;
    std::vector<FixedUpdateCallback> m_fixedUpdateCallbacks;

    struct QueryContext
    {
        std::vector<Body> bodies;
        Vec2 point;
        bool isPointQuery = false;
    };

    static bool QueryCallback(b2ShapeId shapeId, void* context);
    static float CastCallback(
        b2ShapeId shapeId, b2Vec2 point, b2Vec2 normal, float fraction, void* context
    );

    void _checkValid() const;
    void _checkBodiesForJoint(const Body& bodyA, const Body& bodyB) const;

    friend void _tick();
};
}  // namespace physics
}  // namespace kn

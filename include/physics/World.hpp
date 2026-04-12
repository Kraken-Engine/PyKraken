#pragma once

#include <box2d/box2d.h>
#ifdef KRAKEN_ENABLE_PYTHON
#include <nanobind/nanobind.h>
#endif  // KRAKEN_ENABLE_PYTHON

#include <functional>
#include <vector>

#include "Color.hpp"
#include "Math.hpp"
#include "physics/bodies/Body.hpp"

#ifdef KRAKEN_ENABLE_PYTHON
namespace nb = nanobind;
#endif  // KRAKEN_ENABLE_PYTHON

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
#ifdef KRAKEN_ENABLE_PYTHON
void _bind(nb::module_& module);
#endif  // KRAKEN_ENABLE_PYTHON

void _tick();

void setFixedDelta(float fixedDelta);
float getFixedDelta();
void setMaxSubsteps(int maxSubsteps);
int getMaxSubsteps();

class CharacterBody;
class RigidBody;
class StaticBody;

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
    float fraction;
};

struct DebugDrawOptions
{
    bool filledShapes = false;

    bool shapes = true;
    bool joints = true;
    bool jointExtras = true;
    bool bounds = false;
    bool mass = false;
    bool bodyNames = false;
    bool contacts = false;
    bool graphColors = false;
    bool contactNormals = false;
    bool contactImpulses = false;
    bool contactFeatures = false;
    bool frictionImpulses = false;
    bool islands = false;

    bool useDrawingBounds = false;
    b2AABB drawingBounds = {};
};

class World
{
  public:
    explicit World(const Vec2& gravity = Vec2::ZERO);
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

    void addFixedUpdate(std::function<void(float)> callback);
    void clearFixedUpdates();

    void debugDraw(const Color& color = Color::RED, const DebugDrawOptions& options = {}) const;

  private:
    b2WorldId m_worldId = b2_nullWorldId;
    std::vector<std::function<void(float)>> m_fixedUpdateCallbacks;

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

    b2WorldId _getWorldId() const;

    friend void _tick();
    friend class CharacterBody;
    friend class RigidBody;
    friend class StaticBody;
};
}  // namespace physics
}  // namespace kn

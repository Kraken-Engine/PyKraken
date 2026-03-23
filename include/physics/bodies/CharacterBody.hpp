#pragma once

#include <box2d/box2d.h>

#include <vector>

#include "Math.hpp"
#include "physics/bodies/Body.hpp"

namespace kn
{
namespace physics
{

// Collision cast result data
struct CastResult
{
    b2Vec2 point;
    b2Vec2 normal;
    b2BodyId bodyId;
    float fraction;
    bool hit;
};

class CharacterBody : public Body
{
  public:
    enum class MotionMode
    {
        Grounded,
        Floating,
    } motionMode = MotionMode::Grounded;

    Vec2 velocity;

    // Movement parameters
    float mass = 1.0f;
    float maxSpeed = 6.0f;
    float acceleration = 20.0f;
    float friction = 8.0f;
    float stopSpeed = 3.0f;
    float airSteer = 0.2f;

    CharacterBody(World& world);
    ~CharacterBody() = default;

    void moveAndSlide(double delta = -1.0);

    bool isOnFloor() const;
    bool isOnCeiling() const;
    bool isOnWall() const;

    void setCapsuleShape(const class Capsule& capsule);
    class Capsule getCapsuleShape() const;

  private:
    World* m_world = nullptr;
    b2Capsule m_capsule = {{0.0f, -0.5f}, {0.0f, 0.5f}, 0.3f};
    b2ShapeId m_capsuleShapeId = b2_nullShapeId;  // Track shape for updates
    b2Transform m_transform = {};

    bool m_isOnFloor = false;
    bool m_isOnCeiling = false;
    bool m_isOnWall = false;

    // Movement state
    static constexpr int m_planeCapacity = 8;
    b2CollisionPlane m_planes[m_planeCapacity] = {};
    int m_planeCount = 0;

    // Helper methods for collision detection and response
    void _queryGroundCollision(CastResult& result);
    static bool _planeResultFcn(b2ShapeId shapeId, const b2PlaneResult* planeResult, void* context);
};

}  // namespace physics
}  // namespace kn

#include <algorithm>
#include <cmath>
#include <stdexcept>

#include "Capsule.hpp"
#include "Time.hpp"
#include "physics/World.hpp"
#include "physics/bodies/CharacterBody.hpp"

namespace kn::physics
{
namespace
{

struct CastContext
{
    CastResult* result;
    b2BodyId ignoreBodyId;
};

// Ray cast callback for detecting ground collisions
static float GroundCastCallback(
    b2ShapeId shapeId, b2Vec2 point, b2Vec2 normal, float fraction, void* context
)
{
    CastContext* ctx = (CastContext*)context;
    b2BodyId hitBody = b2Shape_GetBody(shapeId);

    if (B2_ID_EQUALS(hitBody, ctx->ignoreBodyId))
        return -1.0f;  // Continue search, skip this body

    ctx->result->point = point;
    ctx->result->normal = normal;
    ctx->result->bodyId = hitBody;
    ctx->result->fraction = fraction;
    ctx->result->hit = true;
    return fraction;
}

}  // namespace

CharacterBody::CharacterBody(World& world)
    : Body(b2_nullBodyId),
      m_world(&world)
{
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_kinematicBody;
    m_bodyId = b2CreateBody(world._getWorldId(), &bodyDef);
    // Initialize transform using body's current transform
    m_transform.p = {0.0f, 0.0f};
    m_transform.q = b2Rot_identity;

    // Create initial capsule shape with default collision filter
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    m_capsuleShapeId = b2CreateCapsuleShape(m_bodyId, &shapeDef, &m_capsule);
}

void CharacterBody::setCapsuleShape(const Capsule& capsule)
{
    // Update internal capsule data
    m_capsule.center1 = static_cast<b2Vec2>(capsule.p1);
    m_capsule.center2 = static_cast<b2Vec2>(capsule.p2);
    m_capsule.radius = static_cast<float>(capsule.radius);

    // Destroy the old shape if it exists
    if (b2Shape_IsValid(m_capsuleShapeId))
        b2DestroyShape(m_capsuleShapeId, false);

    // Create the new capsule shape
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    m_capsuleShapeId = b2CreateCapsuleShape(m_bodyId, &shapeDef, &m_capsule);
}

Capsule CharacterBody::getCapsuleShape() const
{
    return Capsule{
        {m_capsule.center1.x, m_capsule.center1.y},
        {m_capsule.center2.x, m_capsule.center2.y},
        m_capsule.radius
    };
}

void CharacterBody::moveAndSlide(double delta)
{
    if (!isValid())
        throw std::runtime_error("Invalid CharacterBody cannot move");

    float timeStep = delta >= 0.0 ? static_cast<float>(delta)
                                  : static_cast<float>(kn::time::getDelta());
    if (timeStep <= 0.0f)
        return;

    // Get current position and rotation from Box2D body
    b2Vec2 currentPos = b2Body_GetPosition(m_bodyId);
    b2Rot currentRot = b2Body_GetRotation(m_bodyId);
    m_transform.p = currentPos;
    m_transform.q = currentRot;

    // Reset collision state
    m_isOnFloor = false;
    m_isOnCeiling = false;
    m_isOnWall = false;

    // Apply gravity
    if (motionMode == MotionMode::Grounded)
    {
        // Detect ground collision
        CastResult groundResult = {};
        _queryGroundCollision(groundResult);
        m_isOnFloor = groundResult.hit;
    }

    // Apply friction and movement response
    float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);

    bool shouldApplyFriction = (motionMode == MotionMode::Grounded && m_isOnFloor) ||
                               (motionMode == MotionMode::Floating);

    if (shouldApplyFriction)
    {
        // On ground or floating: apply friction
        if (speed < stopSpeed)
        {
            velocity.x = 0.0f;
            velocity.y = 0.0f;
        }
        else
        {
            float frictionDecel = friction * timeStep;
            float newSpeed = std::max(0.0f, speed - frictionDecel);
            if (speed > 0.01f)
            {
                float scale = newSpeed / speed;
                velocity.x *= scale;
                velocity.y *= scale;
            }
        }
    }

    // Calculate movement for this frame
    b2Vec2 targetVelocity = {static_cast<float>(velocity.x), static_cast<float>(velocity.y)};
    b2Vec2 targetPos = currentPos + targetVelocity * timeStep;

    b2QueryFilter filter = b2DefaultQueryFilter();
    float tolerance = 0.01f;

    // We will accumulate iterations if we hit things
    for (int iteration = 0; iteration < 5; ++iteration)
    {
        m_planeCount = 0;

        b2Capsule mover;
        mover.center1 = b2TransformPoint(m_transform, m_capsule.center1);
        mover.center2 = b2TransformPoint(m_transform, m_capsule.center2);
        mover.radius = m_capsule.radius;

        b2World_CollideMover(m_world->_getWorldId(), &mover, filter, _planeResultFcn, this);

        b2Vec2 delta = targetPos - m_transform.p;
        b2PlaneSolverResult result = b2SolvePlanes(delta, m_planes, m_planeCount);

        float fraction =
            b2World_CastMover(m_world->_getWorldId(), &mover, result.translation, filter);

        b2Vec2 moveDelta = {fraction * result.translation.x, fraction * result.translation.y};
        m_transform.p.x += moveDelta.x;
        m_transform.p.y += moveDelta.y;

        if (b2LengthSquared(moveDelta) < tolerance * tolerance)
        {
            break;
        }
    }

    targetVelocity = b2ClipVector(targetVelocity, m_planes, m_planeCount);
    velocity.x = targetVelocity.x;
    velocity.y = targetVelocity.y;

    // Update position in Box2D
    b2Body_SetTransform(m_bodyId, m_transform.p, currentRot);
}

void CharacterBody::_queryGroundCollision(CastResult& result)
{
    // Find the bottom-most center of the capsule (Y is down)
    b2Vec2 center1 = b2TransformPoint(m_transform, m_capsule.center1);
    b2Vec2 center2 = b2TransformPoint(m_transform, m_capsule.center2);
    b2Vec2 bottomCenter = (center1.y > center2.y) ? center1 : center2;

    // Create a small circle proxy to cast downward
    b2Circle circle = {bottomCenter, 0.05f};
    b2ShapeProxy proxy = b2MakeProxy(&bottomCenter, 1, circle.radius);

    // Cast downward (positive Y is down) just past the capsule radius
    b2Vec2 translation = {0.0f, m_capsule.radius + 0.1f};

    // Query against all bodies
    b2QueryFilter filter = b2DefaultQueryFilter();

    CastContext ctx = {&result, m_bodyId};
    b2World_CastShape(
        m_world->_getWorldId(), &proxy, translation, filter, GroundCastCallback, &ctx
    );
}

bool CharacterBody::_planeResultFcn(
    b2ShapeId shapeId, const b2PlaneResult* planeResult, void* context
)
{
    CharacterBody* self = static_cast<CharacterBody*>(context);

    // Ignore self
    b2BodyId hitBody = b2Shape_GetBody(shapeId);
    if (B2_ID_EQUALS(hitBody, self->m_bodyId))
        return true;

    if (self->m_planeCount < CharacterBody::m_planeCapacity)
    {
        b2CollisionPlane* plane = self->m_planes + self->m_planeCount;
        plane->plane = planeResult->plane;
        plane->pushLimit = FLT_MAX;
        plane->push = 0.0f;
        plane->clipVelocity = true;

        if (b2Body_GetType(hitBody) == b2_dynamicBody)
        {
            // Allow physical pushing of rigid bodies based on CharacterBody mass
            // and softer clipping for smooth movement over/through lightweight objects
            plane->pushLimit = 0.5f;

            b2Vec2 n = planeResult->plane.normal;
            b2Vec2 v = {static_cast<float>(self->velocity.x), static_cast<float>(self->velocity.y)};
            float vn = b2Dot(v, n);

            // If character is moving into the dynamic body
            if (vn < 0.0f)
            {
                // Calculate impulse proportional to collision normal velocity and character mass
                float pushForce = -vn * self->mass;
                b2Vec2 impulse = {n.x * -pushForce, n.y * -pushForce};
                b2Body_ApplyLinearImpulse(hitBody, impulse, planeResult->point, true);
            }
        }

        self->m_planeCount += 1;

        if (self->motionMode == MotionMode::Floating)
        {
            self->m_isOnWall = true;
        }
        else
        {
            // rudimentary floor / wall / ceiling check
            float ny = plane->plane.normal.y;
            if (ny < -0.5f)
                self->m_isOnFloor = true;
            else if (ny > 0.5f)
                self->m_isOnCeiling = true;
            else
                self->m_isOnWall = true;
        }
    }
    return true;
}

bool CharacterBody::isOnFloor() const
{
    return m_isOnFloor;
}

bool CharacterBody::isOnCeiling() const
{
    return m_isOnCeiling;
}

bool CharacterBody::isOnWall() const
{
    return m_isOnWall;
}

}  // namespace kn::physics

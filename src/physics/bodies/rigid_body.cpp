#include <box2d/box2d.h>

#include "Math.hpp"
#include "physics/World.hpp"
#include "physics/bodies/RigidBody.hpp"

namespace kn::physics
{
RigidBody::RigidBody(World& world)
    : Body(b2_nullBodyId)
{
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    m_bodyId = b2CreateBody(world._getWorldId(), &bodyDef);
}

void RigidBody::setLinearVelocity(const Vec2& velocity)
{
    _checkValid();
    b2Body_SetLinearVelocity(m_bodyId, static_cast<b2Vec2>(velocity));
}

Vec2 RigidBody::getLinearVelocity() const
{
    _checkValid();
    const b2Vec2 vel = b2Body_GetLinearVelocity(m_bodyId);
    return {vel.x, vel.y};
}

void RigidBody::setAngularVelocity(float velocity)
{
    _checkValid();
    b2Body_SetAngularVelocity(m_bodyId, velocity);
}

float RigidBody::getAngularVelocity() const
{
    _checkValid();
    return b2Body_GetAngularVelocity(m_bodyId);
}

void RigidBody::setLinearDamping(float damping)
{
    _checkValid();
    b2Body_SetLinearDamping(m_bodyId, damping);
}

float RigidBody::getLinearDamping() const
{
    _checkValid();
    return b2Body_GetLinearDamping(m_bodyId);
}

void RigidBody::setAngularDamping(float damping)
{
    _checkValid();
    b2Body_SetAngularDamping(m_bodyId, damping);
}

float RigidBody::getAngularDamping() const
{
    _checkValid();
    return b2Body_GetAngularDamping(m_bodyId);
}

void RigidBody::setFixedRotation(bool fixed)
{
    _checkValid();
    b2Body_SetFixedRotation(m_bodyId, fixed);
}

bool RigidBody::isFixedRotation() const
{
    _checkValid();
    return b2Body_IsFixedRotation(m_bodyId);
}

bool RigidBody::isAwake() const
{
    _checkValid();
    return b2Body_IsAwake(m_bodyId);
}

void RigidBody::wake()
{
    _checkValid();
    b2Body_SetAwake(m_bodyId, true);
}

void RigidBody::applyForce(const Vec2& force, const Vec2& point, bool wake)
{
    _checkValid();
    b2Body_ApplyForce(m_bodyId, static_cast<b2Vec2>(force), static_cast<b2Vec2>(point), wake);
}

void RigidBody::applyForceToCenter(const Vec2& force, bool wake)
{
    _checkValid();
    b2Body_ApplyForceToCenter(m_bodyId, static_cast<b2Vec2>(force), wake);
}

void RigidBody::applyTorque(float torque, bool wake)
{
    _checkValid();
    b2Body_ApplyTorque(m_bodyId, torque, wake);
}

void RigidBody::applyLinearImpulse(const Vec2& impulse, const Vec2& point, bool wake)
{
    _checkValid();
    b2Body_ApplyLinearImpulse(
        m_bodyId, static_cast<b2Vec2>(impulse), static_cast<b2Vec2>(point), wake
    );
}

void RigidBody::applyLinearImpulseToCenter(const Vec2& impulse, bool wake)
{
    _checkValid();
    b2Body_ApplyLinearImpulseToCenter(m_bodyId, static_cast<b2Vec2>(impulse), wake);
}

void RigidBody::applyAngularImpulse(float impulse, bool wake)
{
    _checkValid();
    b2Body_ApplyAngularImpulse(m_bodyId, impulse, wake);
}

float RigidBody::getMass() const
{
    _checkValid();
    return b2Body_GetMass(m_bodyId);
}
}  // namespace kn::physics

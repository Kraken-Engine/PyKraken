#pragma once

#include "physics/bodies/Body.hpp"

namespace kn
{
class Vec2;

namespace physics
{
class World;

class RigidBody : public Body
{
  public:
    RigidBody(World& world);
    ~RigidBody() = default;

    void setLinearVelocity(const Vec2& velocity);
    Vec2 getLinearVelocity() const;

    void setAngularVelocity(float velocity);
    float getAngularVelocity() const;

    void setLinearDamping(float damping);
    float getLinearDamping() const;

    void setAngularDamping(float damping);
    float getAngularDamping() const;

    void setFixedRotation(bool fixed);
    bool isFixedRotation() const;

    bool isAwake() const;
    void wake();

    void applyForce(const Vec2& force, const Vec2& point, bool wake = true);
    void applyForceToCenter(const Vec2& force, bool wake = true);
    void applyTorque(float torque, bool wake = true);
    void applyLinearImpulse(const Vec2& impulse, const Vec2& point, bool wake = true);
    void applyLinearImpulseToCenter(const Vec2& impulse, bool wake = true);
    void applyAngularImpulse(float impulse, bool wake = true);

    float getMass() const;

    void setBullet(bool isBullet);
    bool isBullet() const;
};
}  // namespace physics
}  // namespace kn

#pragma once

#include "physics/joints/Joint.hpp"

namespace kn
{
class Vec2;

namespace physics
{
class MotorJoint : public Joint
{
  public:
    void setLinearOffset(const Vec2& linearOffset);
    Vec2 getLinearOffset() const;

    void setAngularOffset(float angularOffset);
    float getAngularOffset() const;

    void setMaxForce(float maxForce);
    float getMaxForce() const;

    void setMaxTorque(float maxTorque);
    float getMaxTorque() const;

    void setCorrectionFactor(float factor);
    float getCorrectionFactor() const;

  protected:
    MotorJoint(b2JointId jointId);

    friend class World;
};
}  // namespace physics
}  // namespace kn

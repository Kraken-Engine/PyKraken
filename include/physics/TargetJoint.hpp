#pragma once

#include "physics/Joint.hpp"

namespace kn
{
class Vec2;

namespace physics
{
class TargetJoint : public Joint
{
  public:
    TargetJoint() = default;

    void setTarget(const Vec2& target);
    Vec2 getTarget() const;

    void setSpringHertz(float hertz);
    float getSpringHertz() const;

    void setSpringDampingRatio(float dampingRatio);
    float getSpringDampingRatio() const;

    void setMaxForce(float maxForce);
    float getMaxForce() const;

  protected:
    TargetJoint(b2JointId jointId);

    friend class World;
};
}  // namespace physics
}  // namespace kn

#pragma once

#include "physics/joints/Joint.hpp"

namespace kn
{
class Vec2;

namespace physics
{
class MouseJoint : public Joint
{
  public:
    void setTarget(const Vec2& target);
    Vec2 getTarget() const;

    void setSpringHertz(float hertz);
    float getSpringHertz() const;

    void setSpringDampingRatio(float dampingRatio);
    float getSpringDampingRatio() const;

    void setMaxForce(float maxForce);
    float getMaxForce() const;

  protected:
    MouseJoint(b2JointId jointId);

    friend class World;
};
}  // namespace physics
}  // namespace kn

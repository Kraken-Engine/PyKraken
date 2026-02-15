#pragma once

#include "physics/joints/Joint.hpp"

namespace kn::physics
{
class WeldJoint : public Joint
{
  public:
    void setLinearHertz(float hertz);
    float getLinearHertz() const;

    void setLinearDampingRatio(float dampingRatio);
    float getLinearDampingRatio() const;

    void setAngularHertz(float hertz);
    float getAngularHertz() const;

    void setAngularDampingRatio(float dampingRatio);
    float getAngularDampingRatio() const;

  protected:
    WeldJoint(b2JointId jointId);

    friend class World;
};
}  // namespace kn::physics

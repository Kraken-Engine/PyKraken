#pragma once

#include "physics/joints/Joint.hpp"

namespace kn::physics
{
class WheelJoint : public Joint
{
  public:
    void enableSpring(bool enable);
    bool isSpringEnabled() const;

    void setSpringHertz(float hertz);
    float getSpringHertz() const;

    void setSpringDampingRatio(float dampingRatio);
    float getSpringDampingRatio() const;

    void enableLimit(bool enable);
    bool isLimitEnabled() const;

    float getLowerLimit() const;
    float getUpperLimit() const;
    void setLimits(float lower, float upper);

    void enableMotor(bool enable);
    bool isMotorEnabled() const;

    void setMotorSpeed(float speed);
    float getMotorSpeed() const;

    void setMaxMotorTorque(float torque);
    float getMaxMotorTorque() const;

    float getMotorTorque() const;

  protected:
    WheelJoint(b2JointId jointId);

    friend class World;
};
}  // namespace kn::physics

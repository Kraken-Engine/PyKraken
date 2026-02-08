#pragma once

#include "physics/Joint.hpp"

namespace kn::physics
{
class RevoluteJoint : public Joint
{
  public:
    RevoluteJoint() = default;

    void enableSpring(bool enable);
    bool isSpringEnabled() const;

    void setSpringHertz(float hertz);
    float getSpringHertz() const;

    void setSpringDampingRatio(float dampingRatio);
    float getSpringDampingRatio() const;

    void setTargetAngle(float angle);
    float getTargetAngle() const;

    float getAngle() const;

    void enableLimit(bool enable);
    bool isLimitEnabled() const;

    float getLowerLimit() const;
    float getUpperLimit() const;
    void setLimits(float lower, float upper);

    void enableMotor(bool enable);
    bool isMotorEnabled() const;

    void setMotorSpeed(float speed);
    float getMotorSpeed() const;

    float getMotorTorque() const;

    void setMaxMotorTorque(float torque);
    float getMaxMotorTorque() const;

  protected:
    RevoluteJoint(b2JointId jointId);

    friend class World;
};
}  // namespace kn::physics

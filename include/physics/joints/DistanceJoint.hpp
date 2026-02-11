#pragma once

#include "physics/joints/Joint.hpp"

namespace kn::physics
{
class DistanceJoint : public Joint
{
  public:
    void setLength(float length);
    float getLength() const;

    void enableSpring(bool enable);
    bool isSpringEnabled() const;

    void setSpringHertz(float hertz);
    float getSpringHertz() const;

    void setSpringDampingRatio(float dampingRatio);
    float getSpringDampingRatio() const;

    void enableLimit(bool enable);
    bool isLimitEnabled() const;

    void setLengthRange(float minLength, float maxLength);
    float getMinLength() const;
    float getMaxLength() const;

    float getCurrentLength() const;

    void enableMotor(bool enable);
    bool isMotorEnabled() const;

    void setMotorSpeed(float speed);
    float getMotorSpeed() const;

    void setMaxMotorForce(float force);
    float getMaxMotorForce() const;

    float getMotorForce() const;

  protected:
    DistanceJoint(b2JointId jointId);

    friend class World;
};
}  // namespace kn::physics

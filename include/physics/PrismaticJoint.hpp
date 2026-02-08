#pragma once

#include "physics/Joint.hpp"

namespace kn::physics
{
class PrismaticJoint : public Joint
{
  public:
    PrismaticJoint() = default;

    void enableSpring(bool enable);
    bool isSpringEnabled() const;

    void setSpringHertz(float hertz);
    float getSpringHertz() const;

    void setSpringDampingRatio(float dampingRatio);
    float getSpringDampingRatio() const;

    void setTargetTranslation(float translation);
    float getTargetTranslation() const;

    void enableLimit(bool enable);
    bool isLimitEnabled() const;

    float getLowerLimit() const;
    float getUpperLimit() const;
    void setLimits(float lower, float upper);

    void enableMotor(bool enable);
    bool isMotorEnabled() const;

    void setMotorSpeed(float speed);
    float getMotorSpeed() const;

    void setMaxMotorForce(float force);
    float getMaxMotorForce() const;

    float getMotorForce() const;

    float getTranslation() const;
    float getSpeed() const;

  protected:
    PrismaticJoint(b2JointId jointId);

    friend class World;
};
}  // namespace kn::physics

#include "physics/joints/RevoluteJoint.hpp"

namespace kn::physics
{
RevoluteJoint::RevoluteJoint(b2JointId jointId)
    : Joint(jointId)
{
}

void RevoluteJoint::enableSpring(bool enable)
{
    _checkValid();
    b2RevoluteJoint_EnableSpring(m_jointId, enable);
}

bool RevoluteJoint::isSpringEnabled() const
{
    _checkValid();
    return b2RevoluteJoint_IsSpringEnabled(m_jointId);
}

void RevoluteJoint::setSpringHertz(float hertz)
{
    _checkValid();
    b2RevoluteJoint_SetSpringHertz(m_jointId, hertz);
}

float RevoluteJoint::getSpringHertz() const
{
    _checkValid();
    return b2RevoluteJoint_GetSpringHertz(m_jointId);
}

void RevoluteJoint::setSpringDampingRatio(float dampingRatio)
{
    _checkValid();
    b2RevoluteJoint_SetSpringDampingRatio(m_jointId, dampingRatio);
}

float RevoluteJoint::getSpringDampingRatio() const
{
    _checkValid();
    return b2RevoluteJoint_GetSpringDampingRatio(m_jointId);
}

void RevoluteJoint::setTargetAngle(float angle)
{
    _checkValid();
    b2RevoluteJoint_SetTargetAngle(m_jointId, angle);
}

float RevoluteJoint::getTargetAngle() const
{
    _checkValid();
    return b2RevoluteJoint_GetTargetAngle(m_jointId);
}

float RevoluteJoint::getAngle() const
{
    _checkValid();
    return b2RevoluteJoint_GetAngle(m_jointId);
}

void RevoluteJoint::enableLimit(bool enable)
{
    _checkValid();
    b2RevoluteJoint_EnableLimit(m_jointId, enable);
}

bool RevoluteJoint::isLimitEnabled() const
{
    _checkValid();
    return b2RevoluteJoint_IsLimitEnabled(m_jointId);
}

float RevoluteJoint::getLowerLimit() const
{
    _checkValid();
    return b2RevoluteJoint_GetLowerLimit(m_jointId);
}

float RevoluteJoint::getUpperLimit() const
{
    _checkValid();
    return b2RevoluteJoint_GetUpperLimit(m_jointId);
}

void RevoluteJoint::setLimits(float lower, float upper)
{
    _checkValid();
    b2RevoluteJoint_SetLimits(m_jointId, lower, upper);
}

void RevoluteJoint::enableMotor(bool enable)
{
    _checkValid();
    b2RevoluteJoint_EnableMotor(m_jointId, enable);
}

bool RevoluteJoint::isMotorEnabled() const
{
    _checkValid();
    return b2RevoluteJoint_IsMotorEnabled(m_jointId);
}

void RevoluteJoint::setMotorSpeed(float speed)
{
    _checkValid();
    b2RevoluteJoint_SetMotorSpeed(m_jointId, speed);
}

float RevoluteJoint::getMotorSpeed() const
{
    _checkValid();
    return b2RevoluteJoint_GetMotorSpeed(m_jointId);
}

float RevoluteJoint::getMotorTorque() const
{
    _checkValid();
    return b2RevoluteJoint_GetMotorTorque(m_jointId);
}

void RevoluteJoint::setMaxMotorTorque(float torque)
{
    _checkValid();
    b2RevoluteJoint_SetMaxMotorTorque(m_jointId, torque);
}

float RevoluteJoint::getMaxMotorTorque() const
{
    _checkValid();
    return b2RevoluteJoint_GetMaxMotorTorque(m_jointId);
}
}  // namespace kn::physics

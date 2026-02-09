#include "physics/joints/WheelJoint.hpp"

namespace kn::physics
{
WheelJoint::WheelJoint(b2JointId jointId)
    : Joint(jointId)
{
}

void WheelJoint::enableSpring(bool enable)
{
    _checkValid();
    b2WheelJoint_EnableSpring(m_jointId, enable);
}

bool WheelJoint::isSpringEnabled() const
{
    _checkValid();
    return b2WheelJoint_IsSpringEnabled(m_jointId);
}

void WheelJoint::setSpringHertz(float hertz)
{
    _checkValid();
    b2WheelJoint_SetSpringHertz(m_jointId, hertz);
}

float WheelJoint::getSpringHertz() const
{
    _checkValid();
    return b2WheelJoint_GetSpringHertz(m_jointId);
}

void WheelJoint::setSpringDampingRatio(float dampingRatio)
{
    _checkValid();
    b2WheelJoint_SetSpringDampingRatio(m_jointId, dampingRatio);
}

float WheelJoint::getSpringDampingRatio() const
{
    _checkValid();
    return b2WheelJoint_GetSpringDampingRatio(m_jointId);
}

void WheelJoint::enableLimit(bool enable)
{
    _checkValid();
    b2WheelJoint_EnableLimit(m_jointId, enable);
}

bool WheelJoint::isLimitEnabled() const
{
    _checkValid();
    return b2WheelJoint_IsLimitEnabled(m_jointId);
}

float WheelJoint::getLowerLimit() const
{
    _checkValid();
    return b2WheelJoint_GetLowerLimit(m_jointId);
}

float WheelJoint::getUpperLimit() const
{
    _checkValid();
    return b2WheelJoint_GetUpperLimit(m_jointId);
}

void WheelJoint::setLimits(float lower, float upper)
{
    _checkValid();
    b2WheelJoint_SetLimits(m_jointId, lower, upper);
}

void WheelJoint::enableMotor(bool enable)
{
    _checkValid();
    b2WheelJoint_EnableMotor(m_jointId, enable);
}

bool WheelJoint::isMotorEnabled() const
{
    _checkValid();
    return b2WheelJoint_IsMotorEnabled(m_jointId);
}

void WheelJoint::setMotorSpeed(float speed)
{
    _checkValid();
    b2WheelJoint_SetMotorSpeed(m_jointId, speed);
}

float WheelJoint::getMotorSpeed() const
{
    _checkValid();
    return b2WheelJoint_GetMotorSpeed(m_jointId);
}

void WheelJoint::setMaxMotorTorque(float torque)
{
    _checkValid();
    b2WheelJoint_SetMaxMotorTorque(m_jointId, torque);
}

float WheelJoint::getMaxMotorTorque() const
{
    _checkValid();
    return b2WheelJoint_GetMaxMotorTorque(m_jointId);
}

float WheelJoint::getMotorTorque() const
{
    _checkValid();
    return b2WheelJoint_GetMotorTorque(m_jointId);
}
}  // namespace kn::physics

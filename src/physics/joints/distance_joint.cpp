#include "physics/joints/DistanceJoint.hpp"

namespace kn::physics
{
DistanceJoint::DistanceJoint(b2JointId jointId)
    : Joint(jointId)
{
}

void DistanceJoint::setLength(float length)
{
    _checkValid();
    b2DistanceJoint_SetLength(m_jointId, length);
}

float DistanceJoint::getLength() const
{
    _checkValid();
    return b2DistanceJoint_GetLength(m_jointId);
}

void DistanceJoint::enableSpring(bool enable)
{
    _checkValid();
    b2DistanceJoint_EnableSpring(m_jointId, enable);
}

bool DistanceJoint::isSpringEnabled() const
{
    _checkValid();
    return b2DistanceJoint_IsSpringEnabled(m_jointId);
}

void DistanceJoint::setSpringHertz(float hertz)
{
    _checkValid();
    b2DistanceJoint_SetSpringHertz(m_jointId, hertz);
}

float DistanceJoint::getSpringHertz() const
{
    _checkValid();
    return b2DistanceJoint_GetSpringHertz(m_jointId);
}

void DistanceJoint::setSpringDampingRatio(float dampingRatio)
{
    _checkValid();
    b2DistanceJoint_SetSpringDampingRatio(m_jointId, dampingRatio);
}

float DistanceJoint::getSpringDampingRatio() const
{
    _checkValid();
    return b2DistanceJoint_GetSpringDampingRatio(m_jointId);
}

void DistanceJoint::enableLimit(bool enable)
{
    _checkValid();
    b2DistanceJoint_EnableLimit(m_jointId, enable);
}

bool DistanceJoint::isLimitEnabled() const
{
    _checkValid();
    return b2DistanceJoint_IsLimitEnabled(m_jointId);
}

void DistanceJoint::setLengthRange(float minLength, float maxLength)
{
    _checkValid();
    b2DistanceJoint_SetLengthRange(m_jointId, minLength, maxLength);
}

float DistanceJoint::getMinLength() const
{
    _checkValid();
    return b2DistanceJoint_GetMinLength(m_jointId);
}

float DistanceJoint::getMaxLength() const
{
    _checkValid();
    return b2DistanceJoint_GetMaxLength(m_jointId);
}

float DistanceJoint::getCurrentLength() const
{
    _checkValid();
    return b2DistanceJoint_GetCurrentLength(m_jointId);
}

void DistanceJoint::enableMotor(bool enable)
{
    _checkValid();
    b2DistanceJoint_EnableMotor(m_jointId, enable);
}

bool DistanceJoint::isMotorEnabled() const
{
    _checkValid();
    return b2DistanceJoint_IsMotorEnabled(m_jointId);
}

void DistanceJoint::setMotorSpeed(float speed)
{
    _checkValid();
    b2DistanceJoint_SetMotorSpeed(m_jointId, speed);
}

float DistanceJoint::getMotorSpeed() const
{
    _checkValid();
    return b2DistanceJoint_GetMotorSpeed(m_jointId);
}

void DistanceJoint::setMaxMotorForce(float force)
{
    _checkValid();
    b2DistanceJoint_SetMaxMotorForce(m_jointId, force);
}

float DistanceJoint::getMaxMotorForce() const
{
    _checkValid();
    return b2DistanceJoint_GetMaxMotorForce(m_jointId);
}

float DistanceJoint::getMotorForce() const
{
    _checkValid();
    return b2DistanceJoint_GetMotorForce(m_jointId);
}
}  // namespace kn::physics

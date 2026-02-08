#include "physics/PrismaticJoint.hpp"

namespace kn::physics
{
PrismaticJoint::PrismaticJoint(b2JointId jointId)
    : Joint(jointId)
{
}

void PrismaticJoint::enableSpring(bool enable)
{
    _checkValid();
    b2PrismaticJoint_EnableSpring(m_jointId, enable);
}

bool PrismaticJoint::isSpringEnabled() const
{
    _checkValid();
    return b2PrismaticJoint_IsSpringEnabled(m_jointId);
}

void PrismaticJoint::setSpringHertz(float hertz)
{
    _checkValid();
    b2PrismaticJoint_SetSpringHertz(m_jointId, hertz);
}

float PrismaticJoint::getSpringHertz() const
{
    _checkValid();
    return b2PrismaticJoint_GetSpringHertz(m_jointId);
}

void PrismaticJoint::setSpringDampingRatio(float dampingRatio)
{
    _checkValid();
    b2PrismaticJoint_SetSpringDampingRatio(m_jointId, dampingRatio);
}

float PrismaticJoint::getSpringDampingRatio() const
{
    _checkValid();
    return b2PrismaticJoint_GetSpringDampingRatio(m_jointId);
}

void PrismaticJoint::setTargetTranslation(float translation)
{
    _checkValid();
    b2PrismaticJoint_SetTargetTranslation(m_jointId, translation);
}

float PrismaticJoint::getTargetTranslation() const
{
    _checkValid();
    return b2PrismaticJoint_GetTargetTranslation(m_jointId);
}

void PrismaticJoint::enableLimit(bool enable)
{
    _checkValid();
    b2PrismaticJoint_EnableLimit(m_jointId, enable);
}

bool PrismaticJoint::isLimitEnabled() const
{
    _checkValid();
    return b2PrismaticJoint_IsLimitEnabled(m_jointId);
}

float PrismaticJoint::getLowerLimit() const
{
    _checkValid();
    return b2PrismaticJoint_GetLowerLimit(m_jointId);
}

float PrismaticJoint::getUpperLimit() const
{
    _checkValid();
    return b2PrismaticJoint_GetUpperLimit(m_jointId);
}

void PrismaticJoint::setLimits(float lower, float upper)
{
    _checkValid();
    b2PrismaticJoint_SetLimits(m_jointId, lower, upper);
}

void PrismaticJoint::enableMotor(bool enable)
{
    _checkValid();
    b2PrismaticJoint_EnableMotor(m_jointId, enable);
}

bool PrismaticJoint::isMotorEnabled() const
{
    _checkValid();
    return b2PrismaticJoint_IsMotorEnabled(m_jointId);
}

void PrismaticJoint::setMotorSpeed(float speed)
{
    _checkValid();
    b2PrismaticJoint_SetMotorSpeed(m_jointId, speed);
}

float PrismaticJoint::getMotorSpeed() const
{
    _checkValid();
    return b2PrismaticJoint_GetMotorSpeed(m_jointId);
}

void PrismaticJoint::setMaxMotorForce(float force)
{
    _checkValid();
    b2PrismaticJoint_SetMaxMotorForce(m_jointId, force);
}

float PrismaticJoint::getMaxMotorForce() const
{
    _checkValid();
    return b2PrismaticJoint_GetMaxMotorForce(m_jointId);
}

float PrismaticJoint::getMotorForce() const
{
    _checkValid();
    return b2PrismaticJoint_GetMotorForce(m_jointId);
}

float PrismaticJoint::getTranslation() const
{
    _checkValid();
    return b2PrismaticJoint_GetTranslation(m_jointId);
}

float PrismaticJoint::getSpeed() const
{
    _checkValid();
    return b2PrismaticJoint_GetSpeed(m_jointId);
}
}  // namespace kn::physics

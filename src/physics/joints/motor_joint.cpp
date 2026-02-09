#include "Math.hpp"
#include "physics/joints/MotorJoint.hpp"

namespace kn::physics
{
MotorJoint::MotorJoint(b2JointId jointId)
    : Joint(jointId)
{
}

void MotorJoint::setLinearOffset(const Vec2& linearOffset)
{
    _checkValid();
    b2MotorJoint_SetLinearOffset(m_jointId, static_cast<b2Vec2>(linearOffset));
}

Vec2 MotorJoint::getLinearOffset() const
{
    _checkValid();
    b2Vec2 offset = b2MotorJoint_GetLinearOffset(m_jointId);
    return {offset.x, offset.y};
}

void MotorJoint::setAngularOffset(float angularOffset)
{
    _checkValid();
    b2MotorJoint_SetAngularOffset(m_jointId, angularOffset);
}

float MotorJoint::getAngularOffset() const
{
    _checkValid();
    return b2MotorJoint_GetAngularOffset(m_jointId);
}

void MotorJoint::setMaxForce(float maxForce)
{
    _checkValid();
    b2MotorJoint_SetMaxForce(m_jointId, maxForce);
}

float MotorJoint::getMaxForce() const
{
    _checkValid();
    return b2MotorJoint_GetMaxForce(m_jointId);
}

void MotorJoint::setMaxTorque(float maxTorque)
{
    _checkValid();
    b2MotorJoint_SetMaxTorque(m_jointId, maxTorque);
}

float MotorJoint::getMaxTorque() const
{
    _checkValid();
    return b2MotorJoint_GetMaxTorque(m_jointId);
}

void MotorJoint::setCorrectionFactor(float factor)
{
    _checkValid();
    b2MotorJoint_SetCorrectionFactor(m_jointId, factor);
}

float MotorJoint::getCorrectionFactor() const
{
    _checkValid();
    return b2MotorJoint_GetCorrectionFactor(m_jointId);
}
}  // namespace kn::physics

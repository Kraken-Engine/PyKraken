#include "physics/WeldJoint.hpp"

namespace kn::physics
{
WeldJoint::WeldJoint(b2JointId jointId)
    : Joint(jointId)
{
}

void WeldJoint::setLinearHertz(float hertz)
{
    _checkValid();
    b2WeldJoint_SetLinearHertz(m_jointId, hertz);
}

float WeldJoint::getLinearHertz() const
{
    _checkValid();
    return b2WeldJoint_GetLinearHertz(m_jointId);
}

void WeldJoint::setLinearDampingRatio(float dampingRatio)
{
    _checkValid();
    b2WeldJoint_SetLinearDampingRatio(m_jointId, dampingRatio);
}

float WeldJoint::getLinearDampingRatio() const
{
    _checkValid();
    return b2WeldJoint_GetLinearDampingRatio(m_jointId);
}

void WeldJoint::setAngularHertz(float hertz)
{
    _checkValid();
    b2WeldJoint_SetAngularHertz(m_jointId, hertz);
}

float WeldJoint::getAngularHertz() const
{
    _checkValid();
    return b2WeldJoint_GetAngularHertz(m_jointId);
}

void WeldJoint::setAngularDampingRatio(float dampingRatio)
{
    _checkValid();
    b2WeldJoint_SetAngularDampingRatio(m_jointId, dampingRatio);
}

float WeldJoint::getAngularDampingRatio() const
{
    _checkValid();
    return b2WeldJoint_GetAngularDampingRatio(m_jointId);
}
}  // namespace kn::physics

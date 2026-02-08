#include "Math.hpp"
#include "physics/TargetJoint.hpp"

namespace kn::physics
{
TargetJoint::TargetJoint(b2JointId jointId)
    : Joint(jointId)
{
}

void TargetJoint::setTarget(const Vec2& target)
{
    _checkValid();
    b2MouseJoint_SetTarget(m_jointId, static_cast<b2Vec2>(target));
}

Vec2 TargetJoint::getTarget() const
{
    _checkValid();
    b2Vec2 target = b2MouseJoint_GetTarget(m_jointId);
    return {target.x, target.y};
}

void TargetJoint::setSpringHertz(float hertz)
{
    _checkValid();
    b2MouseJoint_SetSpringHertz(m_jointId, hertz);
}

float TargetJoint::getSpringHertz() const
{
    _checkValid();
    return b2MouseJoint_GetSpringHertz(m_jointId);
}

void TargetJoint::setSpringDampingRatio(float dampingRatio)
{
    _checkValid();
    b2MouseJoint_SetSpringDampingRatio(m_jointId, dampingRatio);
}

float TargetJoint::getSpringDampingRatio() const
{
    _checkValid();
    return b2MouseJoint_GetSpringDampingRatio(m_jointId);
}

void TargetJoint::setMaxForce(float maxForce)
{
    _checkValid();
    b2MouseJoint_SetMaxForce(m_jointId, maxForce);
}

float TargetJoint::getMaxForce() const
{
    _checkValid();
    return b2MouseJoint_GetMaxForce(m_jointId);
}
}  // namespace kn::physics

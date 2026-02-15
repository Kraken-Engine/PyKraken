#include "Math.hpp"
#include "physics/joints/MouseJoint.hpp"

namespace kn::physics
{
MouseJoint::MouseJoint(b2JointId jointId)
    : Joint(jointId)
{
}

void MouseJoint::setTarget(const Vec2& target)
{
    _checkValid();
    b2MouseJoint_SetTarget(m_jointId, static_cast<b2Vec2>(target));
}

Vec2 MouseJoint::getTarget() const
{
    _checkValid();
    b2Vec2 target = b2MouseJoint_GetTarget(m_jointId);
    return {target.x, target.y};
}

void MouseJoint::setSpringHertz(float hertz)
{
    _checkValid();
    b2MouseJoint_SetSpringHertz(m_jointId, hertz);
}

float MouseJoint::getSpringHertz() const
{
    _checkValid();
    return b2MouseJoint_GetSpringHertz(m_jointId);
}

void MouseJoint::setSpringDampingRatio(float dampingRatio)
{
    _checkValid();
    b2MouseJoint_SetSpringDampingRatio(m_jointId, dampingRatio);
}

float MouseJoint::getSpringDampingRatio() const
{
    _checkValid();
    return b2MouseJoint_GetSpringDampingRatio(m_jointId);
}

void MouseJoint::setMaxForce(float maxForce)
{
    _checkValid();
    b2MouseJoint_SetMaxForce(m_jointId, maxForce);
}

float MouseJoint::getMaxForce() const
{
    _checkValid();
    return b2MouseJoint_GetMaxForce(m_jointId);
}
}  // namespace kn::physics

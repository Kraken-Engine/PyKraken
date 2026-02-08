#include "physics/Joint.hpp"

#include <stdexcept>

#include "Math.hpp"
#include "physics/Body.hpp"

namespace kn::physics
{
Joint::~Joint() = default;

Joint::Joint(b2JointId jointId)
    : m_jointId(jointId)
{
}

bool Joint::isValid() const
{
    return b2Joint_IsValid(m_jointId);
}

void Joint::destroy()
{
    if (b2Joint_IsValid(m_jointId))
    {
        b2DestroyJoint(m_jointId);
        m_jointId = b2_nullJointId;
    }
}

Body Joint::getBodyA() const
{
    _checkValid();
    return Body(b2Joint_GetBodyA(m_jointId));
}

Body Joint::getBodyB() const
{
    _checkValid();
    return Body(b2Joint_GetBodyB(m_jointId));
}

void Joint::setCollideConnected(bool collide)
{
    _checkValid();
    b2Joint_SetCollideConnected(m_jointId, collide);
}

bool Joint::getCollideConnected() const
{
    _checkValid();
    return b2Joint_GetCollideConnected(m_jointId);
}

void Joint::setLocalAnchorA(const Vec2& anchor)
{
    _checkValid();
    b2Joint_SetLocalAnchorA(m_jointId, static_cast<b2Vec2>(anchor));
}

Vec2 Joint::getLocalAnchorA() const
{
    _checkValid();
    b2Vec2 anchor = b2Joint_GetLocalAnchorA(m_jointId);
    return {anchor.x, anchor.y};
}

void Joint::setLocalAnchorB(const Vec2& anchor)
{
    _checkValid();
    b2Joint_SetLocalAnchorB(m_jointId, static_cast<b2Vec2>(anchor));
}

Vec2 Joint::getLocalAnchorB() const
{
    _checkValid();
    b2Vec2 anchor = b2Joint_GetLocalAnchorB(m_jointId);
    return {anchor.x, anchor.y};
}

void Joint::_checkValid() const
{
    if (!b2Joint_IsValid(m_jointId))
        throw std::runtime_error("Attempted to use an invalid or destroyed Joint");
}
}  // namespace kn::physics

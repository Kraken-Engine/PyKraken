#pragma once

#include <box2d/box2d.h>

namespace kn
{
class Vec2;

namespace physics
{
class Body;
class World;

class Joint
{
  public:
    virtual ~Joint() = 0;

    bool isValid() const;
    void destroy();

    Body getBodyA() const;
    Body getBodyB() const;

    void setCollideConnected(bool collide);
    bool getCollideConnected() const;

    void setLocalAnchorA(const Vec2& anchor);
    Vec2 getLocalAnchorA() const;

    void setLocalAnchorB(const Vec2& anchor);
    Vec2 getLocalAnchorB() const;

  protected:
    b2JointId m_jointId = b2_nullJointId;

    Joint(b2JointId jointId);

    void _checkValid() const;

    friend class World;
};
}  // namespace physics
}  // namespace kn

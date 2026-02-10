#pragma once

#include <box2d/box2d.h>

#include <vector>

namespace kn
{
class Vec2;
class Polygon;
class Rect;
class Circle;
class Capsule;
struct Color;
struct Transform;

namespace physics
{
class World;

class Body
{
  public:
    Body() = default;
    ~Body() = default;

    void addCollider(
        const Circle& circle, float density = 1.0f, float friction = 0.2f, float restitution = 0.0f,
        bool enableEvents = false, bool isSensor = false
    );
    void addCollider(
        const Polygon& polygon, float density = 1.0f, float friction = 0.2f,
        float restitution = 0.0f, bool enableEvents = false, bool isSensor = false
    );
    void addCollider(
        const Rect& rect, float density = 1.0f, float friction = 0.2f, float restitution = 0.0f,
        bool enableEvents = false, bool isSensor = false
    );
    void addCollider(
        const Capsule& capsule, float density = 1.0f, float friction = 0.2f,
        float restitution = 0.0f, bool enableEvents = false, bool isSensor = false
    );

    void setPos(const Vec2& pos);
    Vec2 getPos() const;

    void setRotation(float rotation);
    float getRotation() const;

    Transform getTransform() const;

    bool isValid() const;

    void destroy();

    bool operator==(const Body& other) const;
    bool operator!=(const Body& other) const;

    void debugDraw() const;
    b2BodyType _getType() const;
    b2BodyId _getBodyId() const;

    void setCollisionLayer(uint64_t layer);
    uint64_t getCollisionLayer() const;

    void setCollisionMask(uint64_t mask);
    uint64_t getCollisionMask() const;

  protected:
    b2BodyId m_bodyId = b2_nullBodyId;
    b2Filter m_filter = b2DefaultFilter();

    Body(b2BodyId bodyId);

    void _checkValid() const;
    std::vector<b2ShapeId> _getShapeIds() const;

    friend class World;
    friend class Joint;
};
}  // namespace physics
}  // namespace kn

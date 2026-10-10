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
struct Transform;

namespace physics
{
class World;
class Collider;

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

    std::vector<Collider> getColliders() const;

    void setPos(const Vec2& pos);
    Vec2 getPos() const;

    void setRotation(float rotation);
    float getRotation() const;

    Transform getTransform() const;

    bool isValid() const;

    void destroy();

    bool operator==(const Body& other) const;
    bool operator!=(const Body& other) const;
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

    friend class Collider;
    friend class World;
    friend class Joint;
};
// Non-owning shape handle. The ID remains readable after destruction; check
// isValid() before accessing the body. A concave polygon has multiple colliders.
class Collider
{
  public:
    explicit Collider(b2ShapeId id)
        : m_id(id)
    {
    }
    uint64_t getId() const
    {
        return b2StoreShapeId(m_id);
    }
    bool isValid() const
    {
        return b2Shape_IsValid(m_id);
    }
    Body getBody() const;
    void setEventsEnabled(bool enabled);
    bool operator==(const Collider& other) const
    {
        return getId() == other.getId();
    }

  private:
    b2ShapeId m_id;
};

}  // namespace physics
}  // namespace kn

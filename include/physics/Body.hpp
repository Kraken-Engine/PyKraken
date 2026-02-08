#pragma once

#include <box2d/box2d.h>

#include <variant>
#include <vector>

#include "Circle.hpp"
#include "Polygon.hpp"
#include "Rect.hpp"

namespace py = pybind11;

namespace kn
{
class Vec2;
struct Color;

namespace physics
{
class World;

enum class BodyType
{
    Static = b2_staticBody,
    Kinematic = b2_kinematicBody,
    Dynamic = b2_dynamicBody,
};

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

    void setType(BodyType type);
    BodyType getType() const;

    void setPos(const Vec2& pos);
    Vec2 getPos() const;

    void setRotation(float rotation);
    float getRotation() const;

    void setLinearVelocity(const Vec2& velocity);
    Vec2 getLinearVelocity() const;

    void setAngularVelocity(float velocity);
    float getAngularVelocity() const;

    void setLinearDamping(float damping);
    float getLinearDamping() const;

    void setAngularDamping(float damping);
    float getAngularDamping() const;

    void setFixedRotation(bool fixed);
    bool isFixedRotation() const;

    bool isAwake() const;
    void wake();

    void applyForce(const Vec2& force, const Vec2& point, bool wake = true);
    void applyForceToCenter(const Vec2& force, bool wake = true);
    void applyTorque(float torque, bool wake = true);
    void applyLinearImpulse(const Vec2& impulse, const Vec2& point, bool wake = true);
    void applyLinearImpulseToCenter(const Vec2& impulse, bool wake = true);
    void applyAngularImpulse(float impulse, bool wake = true);

    float getMass() const;

    bool isValid() const;

    void destroy();

    bool operator==(const Body& other) const;
    bool operator!=(const Body& other) const;

    void draw(const Color& color) const;

  private:
    b2BodyId m_bodyId = b2_nullBodyId;
    std::vector<std::variant<Circle, Polygon, Rect>> m_shapes;

    Body(b2BodyId bodyId);

    void _checkValid() const;

    friend class World;
    friend class Joint;
};
}  // namespace physics
}  // namespace kn

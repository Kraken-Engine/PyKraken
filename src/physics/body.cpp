#include "physics/Body.hpp"

#include <mapbox/earcut.hpp>

#include "Circle.hpp"
#include "Color.hpp"
#include "Draw.hpp"
#include "Polygon.hpp"
#include "Rect.hpp"

namespace kn::physics
{
Body::Body(b2BodyId bodyId)
    : m_bodyId(bodyId)
{
}

void Body::addCollider(const Circle& circle, float density, float friction, float restitution)
{
    _checkValid();
    b2Circle b2c;
    b2c.center = static_cast<b2Vec2>(circle.pos);
    b2c.radius = static_cast<float>(circle.radius);

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = density;
    shapeDef.material.friction = friction;
    shapeDef.material.restitution = restitution;

    b2CreateCircleShape(m_bodyId, &shapeDef, &b2c);
    m_shapes.emplace_back(circle);
}

void Body::addCollider(const Polygon& polygon, float density, float friction, float restitution)
{
    _checkValid();
    const size_t numPoints = polygon.points.size();
    if (numPoints < 3)
        throw std::runtime_error("Polygon must have at least 3 points");

    if (polygon.isConvex())
    {
        std::vector<b2Vec2> b2Points;
        b2Points.reserve(numPoints);
        for (const auto& p : polygon.points)
            b2Points.push_back(static_cast<b2Vec2>(p));

        b2Hull hull = b2ComputeHull(b2Points.data(), static_cast<int>(numPoints));
        b2Polygon poly = b2MakePolygon(&hull, 0.f);

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density = density;
        shapeDef.material.friction = friction;
        shapeDef.material.restitution = restitution;

        b2CreatePolygonShape(m_bodyId, &shapeDef, &poly);
    }
    else
    {
        // Concave: triangulate using earcut
        const std::vector<std::vector<Vec2>> vertList = {polygon.points};
        const auto indices = mapbox::earcut<uint32_t>(vertList);

        for (size_t i = 0; i < indices.size(); i += 3)
        {
            b2Vec2 triangle[3];
            triangle[0] = static_cast<b2Vec2>(polygon.points[indices[i]]);
            triangle[1] = static_cast<b2Vec2>(polygon.points[indices[i + 1]]);
            triangle[2] = static_cast<b2Vec2>(polygon.points[indices[i + 2]]);

            b2Hull hull = b2ComputeHull(triangle, 3);
            b2Polygon poly = b2MakePolygon(&hull, 0.f);

            b2ShapeDef shapeDef = b2DefaultShapeDef();
            shapeDef.density = density;
            shapeDef.material.friction = friction;
            shapeDef.material.restitution = restitution;

            b2CreatePolygonShape(m_bodyId, &shapeDef, &poly);
        }
    }
    m_shapes.emplace_back(polygon);
}

void Body::addCollider(const Rect& rect, float density, float friction, float restitution)
{
    _checkValid();

    b2Vec2 pts[4] = {
        static_cast<b2Vec2>(rect.getTopLeft()),
        static_cast<b2Vec2>(rect.getTopRight()),
        static_cast<b2Vec2>(rect.getBottomRight()),
        static_cast<b2Vec2>(rect.getBottomLeft()),
    };

    b2Hull hull = b2ComputeHull(pts, 4);
    b2Polygon box = b2MakePolygon(&hull, 0.0f);

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = density;
    shapeDef.material.friction = friction;
    shapeDef.material.restitution = restitution;

    b2CreatePolygonShape(m_bodyId, &shapeDef, &box);
    m_shapes.emplace_back(rect);
}

void Body::setType(BodyType type)
{
    _checkValid();
    b2Body_SetType(m_bodyId, static_cast<b2BodyType>(type));
}

BodyType Body::getType() const
{
    _checkValid();
    return static_cast<BodyType>(b2Body_GetType(m_bodyId));
}

void Body::setPos(const Vec2& pos)
{
    _checkValid();
    b2Rot rotation = b2Body_GetRotation(m_bodyId);
    b2Body_SetTransform(m_bodyId, static_cast<b2Vec2>(pos), rotation);
}

Vec2 Body::getPos() const
{
    _checkValid();
    const b2Vec2 pos = b2Body_GetPosition(m_bodyId);
    return Vec2(pos.x, pos.y);
}

void Body::setRotation(float rotation)
{
    _checkValid();
    b2Vec2 position = b2Body_GetPosition(m_bodyId);
    b2Body_SetTransform(m_bodyId, position, b2MakeRot(rotation));
}

float Body::getRotation() const
{
    _checkValid();
    return b2Rot_GetAngle(b2Body_GetRotation(m_bodyId));
}

void Body::setLinearVelocity(const Vec2& velocity)
{
    _checkValid();
    b2Body_SetLinearVelocity(m_bodyId, static_cast<b2Vec2>(velocity));
}

Vec2 Body::getLinearVelocity() const
{
    _checkValid();
    const b2Vec2 vel = b2Body_GetLinearVelocity(m_bodyId);
    return Vec2(vel.x, vel.y);
}

void Body::setAngularVelocity(float velocity)
{
    _checkValid();
    b2Body_SetAngularVelocity(m_bodyId, velocity);
}

float Body::getAngularVelocity() const
{
    _checkValid();
    return b2Body_GetAngularVelocity(m_bodyId);
}

void Body::applyForce(const Vec2& force, const Vec2& point, bool wake)
{
    _checkValid();
    b2Body_ApplyForce(m_bodyId, static_cast<b2Vec2>(force), static_cast<b2Vec2>(point), wake);
}

void Body::applyForceToCenter(const Vec2& force, bool wake)
{
    _checkValid();
    b2Body_ApplyForceToCenter(m_bodyId, static_cast<b2Vec2>(force), wake);
}

void Body::applyTorque(float torque, bool wake)
{
    _checkValid();
    b2Body_ApplyTorque(m_bodyId, torque, wake);
}

void Body::applyLinearImpulse(const Vec2& impulse, const Vec2& point, bool wake)
{
    _checkValid();
    b2Body_ApplyLinearImpulse(
        m_bodyId, static_cast<b2Vec2>(impulse), static_cast<b2Vec2>(point), wake
    );
}

void Body::applyLinearImpulseToCenter(const Vec2& impulse, bool wake)
{
    _checkValid();
    b2Body_ApplyLinearImpulseToCenter(m_bodyId, static_cast<b2Vec2>(impulse), wake);
}

void Body::applyAngularImpulse(float impulse, bool wake)
{
    _checkValid();
    b2Body_ApplyAngularImpulse(m_bodyId, impulse, wake);
}

float Body::getMass() const
{
    _checkValid();
    return b2Body_GetMass(m_bodyId);
}

bool Body::isValid() const
{
    return b2Body_IsValid(m_bodyId);
}

void Body::_checkValid() const
{
    if (!b2Body_IsValid(m_bodyId))
        throw std::runtime_error("Attempted to use an invalid or destroyed Body");
}

void Body::draw(const Color& color) const
{
    _checkValid();
    const Vec2 bodyPos = getPos();
    const float bodyRot = getRotation();

    for (const auto& shapeVariant : m_shapes)
    {
        if (std::holds_alternative<Circle>(shapeVariant))
        {
            Circle circle = std::get<Circle>(shapeVariant);
            circle.pos = bodyPos + circle.pos.rotated(bodyRot);
            kn::draw::circle(circle, color);
        }
        else if (std::holds_alternative<Polygon>(shapeVariant))
        {
            Polygon poly = std::get<Polygon>(shapeVariant).copy();
            for (auto& p : poly.points)
                p = bodyPos + p.rotated(bodyRot);
            kn::draw::polygon(poly, color);
        }
        else if (std::holds_alternative<Rect>(shapeVariant))
        {
            const Rect& rect = std::get<Rect>(shapeVariant);
            const Vec2 center = {rect.x + rect.w / 2.0, rect.y + rect.h / 2.0};
            const Vec2 hSize = {rect.w / 2.0, rect.h / 2.0};
            std::vector<Vec2> points = {
                Vec2(center.x - hSize.x, center.y - hSize.y),
                Vec2(center.x + hSize.x, center.y - hSize.y),
                Vec2(center.x + hSize.x, center.y + hSize.y),
                Vec2(center.x - hSize.x, center.y + hSize.y),
            };
            for (auto& p : points)
                p = bodyPos + p.rotated(bodyRot);
            kn::draw::polygon(Polygon(points), color);
        }
    }
}
}  // namespace kn::physics

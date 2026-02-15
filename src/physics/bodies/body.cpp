#include "physics/bodies/Body.hpp"

#include <mapbox/earcut.hpp>

#include "Capsule.hpp"
#include "Circle.hpp"
#include "Color.hpp"
#include "Draw.hpp"
#include "Line.hpp"
#include "Polygon.hpp"
#include "Rect.hpp"
#include "Transform.hpp"

namespace kn::physics
{
Body::Body(b2BodyId bodyId)
    : m_bodyId(bodyId)
{
}

void Body::destroy()
{
    if (b2Body_IsValid(m_bodyId))
    {
        b2DestroyBody(m_bodyId);
        m_bodyId = b2_nullBodyId;
    }
}

void Body::addCollider(
    const Circle& circle, float density, float friction, float restitution, bool enableEvents,
    bool isSensor
)
{
    _checkValid();
    b2Circle b2c;
    b2c.center = static_cast<b2Vec2>(circle.pos);
    b2c.radius = static_cast<float>(circle.radius);

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = density;
    shapeDef.material.friction = friction;
    shapeDef.material.restitution = restitution;
    shapeDef.enableHitEvents = enableEvents;
    shapeDef.isSensor = isSensor;
    shapeDef.filter = m_filter;

    b2ShapeId shapeId = b2CreateCircleShape(m_bodyId, &shapeDef, &b2c);
}

void Body::addCollider(
    const Polygon& polygon, float density, float friction, float restitution, bool enableEvents,
    bool isSensor
)
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
        shapeDef.enableHitEvents = enableEvents;
        shapeDef.isSensor = isSensor;
        shapeDef.filter = m_filter;

        b2ShapeId shapeId = b2CreatePolygonShape(m_bodyId, &shapeDef, &poly);
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
            shapeDef.enableHitEvents = enableEvents;
            shapeDef.isSensor = isSensor;
            shapeDef.filter = m_filter;

            b2ShapeId shapeId = b2CreatePolygonShape(m_bodyId, &shapeDef, &poly);
        }
    }
}

void Body::addCollider(
    const Rect& rect, float density, float friction, float restitution, bool enableEvents,
    bool isSensor
)
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
    shapeDef.enableHitEvents = enableEvents;
    shapeDef.isSensor = isSensor;
    shapeDef.filter = m_filter;

    b2ShapeId shapeId = b2CreatePolygonShape(m_bodyId, &shapeDef, &box);
}

void Body::addCollider(
    const Capsule& capsule, float density, float friction, float restitution, bool enableEvents,
    bool isSensor
)
{
    _checkValid();
    b2Capsule b2c;
    b2c.center1 = static_cast<b2Vec2>(capsule.p1);
    b2c.center2 = static_cast<b2Vec2>(capsule.p2);
    b2c.radius = static_cast<float>(capsule.radius);

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = density;
    shapeDef.material.friction = friction;
    shapeDef.material.restitution = restitution;
    shapeDef.enableHitEvents = enableEvents;
    shapeDef.isSensor = isSensor;
    shapeDef.filter = m_filter;

    b2ShapeId shapeId = b2CreateCapsuleShape(m_bodyId, &shapeDef, &b2c);
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
    return {pos.x, pos.y};
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

Transform Body::getTransform() const
{
    _checkValid();
    const b2Transform xf = b2Body_GetTransform(m_bodyId);
    return {{xf.p.x, xf.p.y}, b2Rot_GetAngle(xf.q), {1.0f, 1.0f}};
}

bool Body::isValid() const
{
    return b2Body_IsValid(m_bodyId);
}

bool Body::operator==(const Body& other) const
{
    return m_bodyId.index1 == other.m_bodyId.index1 && m_bodyId.world0 == other.m_bodyId.world0 &&
           m_bodyId.generation == other.m_bodyId.generation;
}

bool Body::operator!=(const Body& other) const
{
    return !(*this == other);
}

void Body::_checkValid() const
{
    if (!b2Body_IsValid(m_bodyId))
        throw std::runtime_error("Attempted to use an invalid or destroyed Body");
}

std::vector<b2ShapeId> Body::_getShapeIds() const
{
    _checkValid();
    const int shapeCount = b2Body_GetShapeCount(m_bodyId);
    std::vector<b2ShapeId> shapeIds(shapeCount);
    b2Body_GetShapes(m_bodyId, shapeIds.data(), shapeCount);
    return shapeIds;
}

b2BodyType Body::_getType() const
{
    _checkValid();
    return b2Body_GetType(m_bodyId);
}

b2BodyId Body::_getBodyId() const
{
    _checkValid();
    return m_bodyId;
}

void Body::debugDraw() const
{
    _checkValid();
    const Vec2 bodyPos = getPos();
    const float bodyRot = getRotation();

    const Color color = {255, 0, 0, 255};

    for (const b2ShapeId shapeId : _getShapeIds())
    {
        if (!b2Shape_IsValid(shapeId))
            continue;

        const b2ShapeType shapeType = b2Shape_GetType(shapeId);

        if (shapeType == b2_circleShape)
        {
            const b2Circle circle = b2Shape_GetCircle(shapeId);
            Circle drawCircle{{circle.center.x, circle.center.y}, circle.radius};
            drawCircle.pos = bodyPos + drawCircle.pos.rotated(bodyRot);
            kn::draw::circle(drawCircle, color, 1.0, 16);

            const Vec2 corner = drawCircle.pos + Vec2(drawCircle.radius, 0.0).rotated(bodyRot);
            kn::draw::line(Line(drawCircle.pos, corner), color);
        }
        else if (shapeType == b2_polygonShape)
        {
            const b2Polygon polygon = b2Shape_GetPolygon(shapeId);
            std::vector<Vec2> points;
            for (int i = 0; i < polygon.count; ++i)
            {
                const b2Vec2& v = polygon.vertices[i];
                points.emplace_back(v.x, v.y);
            }
            for (auto& p : points)
                p = bodyPos + p.rotated(bodyRot);
            kn::draw::polygon(Polygon(points), color, false);

            // Draw center line
            const b2Vec2& center = polygon.centroid;
            const Vec2 worldCenter = bodyPos + Vec2{center.x, center.y}.rotated(bodyRot);
            kn::draw::line(Line(worldCenter, points[0]), color);
        }
        else if (shapeType == b2_capsuleShape)
        {
            const b2Capsule capsule = b2Shape_GetCapsule(shapeId);
            Capsule drawCapsule{
                {capsule.center1.x, capsule.center1.y},
                {capsule.center2.x, capsule.center2.y},
                capsule.radius
            };
            drawCapsule.p1 = bodyPos + drawCapsule.p1.rotated(bodyRot);
            drawCapsule.p2 = bodyPos + drawCapsule.p2.rotated(bodyRot);
            kn::draw::capsule(drawCapsule, color, 1.0, 16);
        }
    }
}
void Body::setCollisionLayer(uint64_t layer)
{
    m_filter.categoryBits = layer;
    if (b2Body_IsValid(m_bodyId))
    {
        for (const auto& shapeId : _getShapeIds())
        {
            b2Shape_SetFilter(shapeId, m_filter);
        }
    }
}

uint64_t Body::getCollisionLayer() const
{
    return m_filter.categoryBits;
}

void Body::setCollisionMask(uint64_t mask)
{
    m_filter.maskBits = mask;
    if (b2Body_IsValid(m_bodyId))
    {
        for (const auto& shapeId : _getShapeIds())
        {
            b2Shape_SetFilter(shapeId, m_filter);
        }
    }
}

uint64_t Body::getCollisionMask() const
{
    return m_filter.maskBits;
}
}  // namespace kn::physics

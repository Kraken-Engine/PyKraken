#include "physics/World.hpp"

#include <algorithm>

#include "Capsule.hpp"
#include "Circle.hpp"
#include "Color.hpp"
#include "Math.hpp"
#include "Polygon.hpp"
#include "Rect.hpp"
#include "TileMap.hpp"
#include "Time.hpp"
#include "Transform.hpp"
#include "physics/bodies/CharacterBody.hpp"
#include "physics/bodies/RigidBody.hpp"
#include "physics/bodies/StaticBody.hpp"
#include "physics/joints/DistanceJoint.hpp"
#include "physics/joints/FilterJoint.hpp"
#include "physics/joints/Joint.hpp"
#include "physics/joints/MotorJoint.hpp"
#include "physics/joints/MouseJoint.hpp"
#include "physics/joints/PrismaticJoint.hpp"
#include "physics/joints/RevoluteJoint.hpp"
#include "physics/joints/WeldJoint.hpp"
#include "physics/joints/WheelJoint.hpp"

namespace kn::physics
{
static std::vector<World*> _worlds;
static float _fixedDelta = 1.0f / 60.0f;
static int _maxSubsteps = 4;
static float _accumulator = 0.0f;

void _tick()
{
    if (_fixedDelta <= 0.0f)
        return;

    _accumulator += static_cast<float>(time::getDelta());

    // Limit accumulator to avoid "spiral of death"
    const float maxAccumulator = _fixedDelta * 8.0f;
    if (_accumulator > maxAccumulator)
        _accumulator = maxAccumulator;

    while (_accumulator >= _fixedDelta)
    {
        for (auto* world : _worlds)
        {
            auto it = world->m_fixedUpdateCallbacks.begin();
            while (it != world->m_fixedUpdateCallbacks.end())
            {
                bool shouldRemove = false;
                try
                {
                    if (it->isBound)
                    {
                        py::object owner = it->weakOwner();
                        if (owner.is_none())
                        {
                            shouldRemove = true;
                        }
                        else
                        {
                            it->unboundMethod(owner, _fixedDelta);
                        }
                    }
                    else
                    {
                        it->callback(_fixedDelta);
                    }
                }
                catch (const py::error_already_set& e)
                {
                    throw std::runtime_error(
                        "An error occurred in a fixed update callback: " + std::string(e.what())
                    );
                }

                if (shouldRemove)
                    it = world->m_fixedUpdateCallbacks.erase(it);
                else
                    ++it;
            }

            b2World_Step(world->m_worldId, _fixedDelta, _maxSubsteps);
        }
        _accumulator -= _fixedDelta;
    }
}

void setFixedDelta(const float fixedDelta)
{
    if (fixedDelta <= 0.0f)
        throw std::invalid_argument("Fixed delta must be greater than 0.");
    _fixedDelta = fixedDelta;
}

float getFixedDelta()
{
    return _fixedDelta;
}

void setMaxSubsteps(const int maxSubsteps)
{
    _maxSubsteps = maxSubsteps;
}

int getMaxSubsteps()
{
    return _maxSubsteps;
}

void World::addFixedUpdate(py::object callback)
{
    if (callback.is_none())
        throw std::invalid_argument("Callback cannot be None.");

    if (!py::hasattr(callback, "__call__"))
        throw std::invalid_argument("Callback must be a callable object.");

    FixedUpdateCallback wrapper;
    if (py::hasattr(callback, "__self__") && !py::getattr(callback, "__self__").is_none())
    {
        wrapper.isBound = true;
        wrapper.weakOwner = py::weakref(callback.attr("__self__"));
        wrapper.unboundMethod = callback.attr("__func__");
    }
    else
    {
        wrapper.isBound = false;
        wrapper.callback = std::move(callback);
    }
    m_fixedUpdateCallbacks.push_back(std::move(wrapper));
}

void World::clearFixedUpdates()
{
    m_fixedUpdateCallbacks.clear();
}

World::World(const Vec2& gravity)
{
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = static_cast<b2Vec2>(gravity);
    m_worldId = b2CreateWorld(&worldDef);

    _worlds.push_back(this);
}

World::~World()
{
    _worlds.erase(std::remove(_worlds.begin(), _worlds.end(), this), _worlds.end());

    if (b2World_IsValid(m_worldId))
        b2DestroyWorld(m_worldId);
}

DistanceJoint World::createDistanceJoint(
    const Body& bodyA, const Body& bodyB, const Vec2& anchorA, const Vec2& anchorB
)
{
    _checkValid();
    _checkBodiesForJoint(bodyA, bodyB);

    const auto pA = static_cast<b2Vec2>(anchorA);
    const auto pB = static_cast<b2Vec2>(anchorB);

    b2DistanceJointDef jointDef = b2DefaultDistanceJointDef();
    jointDef.bodyIdA = bodyA.m_bodyId;
    jointDef.bodyIdB = bodyB.m_bodyId;
    jointDef.localAnchorA = b2Body_GetLocalPoint(bodyA.m_bodyId, pA);
    jointDef.localAnchorB = b2Body_GetLocalPoint(bodyB.m_bodyId, pB);
    jointDef.length = b2Length(b2Sub(pB, pA));

    return DistanceJoint(b2CreateDistanceJoint(m_worldId, &jointDef));
}

FilterJoint World::createFilterJoint(const Body& bodyA, const Body& bodyB)
{
    _checkValid();
    _checkBodiesForJoint(bodyA, bodyB);

    b2FilterJointDef jointDef = b2DefaultFilterJointDef();
    jointDef.bodyIdA = bodyA.m_bodyId;
    jointDef.bodyIdB = bodyB.m_bodyId;

    return FilterJoint(b2CreateFilterJoint(m_worldId, &jointDef));
}

MotorJoint World::createMotorJoint(const Body& bodyA, const Body& bodyB)
{
    _checkValid();
    _checkBodiesForJoint(bodyA, bodyB);

    b2MotorJointDef jointDef = b2DefaultMotorJointDef();
    jointDef.bodyIdA = bodyA.m_bodyId;
    jointDef.bodyIdB = bodyB.m_bodyId;

    return MotorJoint(b2CreateMotorJoint(m_worldId, &jointDef));
}

MouseJoint World::createMouseJoint(
    const Body& groundBody, const Body& pulledBody, const Vec2& target
)
{
    _checkValid();
    _checkBodiesForJoint(groundBody, pulledBody);

    b2MouseJointDef jointDef = b2DefaultMouseJointDef();
    jointDef.bodyIdA = groundBody.m_bodyId;
    jointDef.bodyIdB = pulledBody.m_bodyId;
    jointDef.target = static_cast<b2Vec2>(target);

    return MouseJoint(b2CreateMouseJoint(m_worldId, &jointDef));
}

PrismaticJoint World::createPrismaticJoint(
    const Body& bodyA, const Body& bodyB, const Vec2& anchor, const Vec2& axis
)
{
    _checkValid();
    _checkBodiesForJoint(bodyA, bodyB);

    const auto b2anchor = static_cast<b2Vec2>(anchor);
    auto b2axis = static_cast<b2Vec2>(axis);
    b2axis = b2Normalize(b2axis);
    if (b2axis.x == 0.0f && b2axis.y == 0.0f)
        throw std::invalid_argument("Axis vector cannot be zero.");

    b2PrismaticJointDef jointDef = b2DefaultPrismaticJointDef();
    jointDef.bodyIdA = bodyA.m_bodyId;
    jointDef.bodyIdB = bodyB.m_bodyId;
    jointDef.localAnchorA = b2Body_GetLocalPoint(bodyA.m_bodyId, b2anchor);
    jointDef.localAnchorB = b2Body_GetLocalPoint(bodyB.m_bodyId, b2anchor);
    jointDef.localAxisA = b2Body_GetLocalVector(bodyA.m_bodyId, b2axis);
    jointDef.referenceAngle = b2Rot_GetAngle(b2Body_GetRotation(bodyB.m_bodyId)) -
                              b2Rot_GetAngle(b2Body_GetRotation(bodyA.m_bodyId));

    return PrismaticJoint(b2CreatePrismaticJoint(m_worldId, &jointDef));
}

RevoluteJoint World::createRevoluteJoint(const Body& bodyA, const Body& bodyB, const Vec2& anchor)
{
    _checkValid();
    _checkBodiesForJoint(bodyA, bodyB);

    b2RevoluteJointDef jointDef = b2DefaultRevoluteJointDef();
    jointDef.bodyIdA = bodyA.m_bodyId;
    jointDef.bodyIdB = bodyB.m_bodyId;
    jointDef.localAnchorA = b2Body_GetLocalPoint(bodyA.m_bodyId, static_cast<b2Vec2>(anchor));
    jointDef.localAnchorB = b2Body_GetLocalPoint(bodyB.m_bodyId, static_cast<b2Vec2>(anchor));
    jointDef.referenceAngle = b2Rot_GetAngle(b2Body_GetRotation(bodyB.m_bodyId)) -
                              b2Rot_GetAngle(b2Body_GetRotation(bodyA.m_bodyId));

    return RevoluteJoint(b2CreateRevoluteJoint(m_worldId, &jointDef));
}

WeldJoint World::createWeldJoint(const Body& bodyA, const Body& bodyB, const Vec2& anchor)
{
    _checkValid();
    _checkBodiesForJoint(bodyA, bodyB);

    b2WeldJointDef jointDef = b2DefaultWeldJointDef();
    jointDef.bodyIdA = bodyA.m_bodyId;
    jointDef.bodyIdB = bodyB.m_bodyId;
    jointDef.localAnchorA = b2Body_GetLocalPoint(bodyA.m_bodyId, static_cast<b2Vec2>(anchor));
    jointDef.localAnchorB = b2Body_GetLocalPoint(bodyB.m_bodyId, static_cast<b2Vec2>(anchor));
    jointDef.referenceAngle = b2Rot_GetAngle(b2Body_GetRotation(bodyB.m_bodyId)) -
                              b2Rot_GetAngle(b2Body_GetRotation(bodyA.m_bodyId));

    return WeldJoint(b2CreateWeldJoint(m_worldId, &jointDef));
}

WheelJoint World::createWheelJoint(
    const Body& bodyA, const Body& bodyB, const Vec2& anchor, const Vec2& axis
)
{
    _checkValid();
    _checkBodiesForJoint(bodyA, bodyB);

    const auto b2anchor = static_cast<b2Vec2>(anchor);
    auto b2axis = static_cast<b2Vec2>(axis);
    b2axis = b2Normalize(b2axis);
    if (b2axis.x == 0.0f && b2axis.y == 0.0f)
        throw std::invalid_argument("Axis vector cannot be zero.");

    b2WheelJointDef jointDef = b2DefaultWheelJointDef();
    jointDef.bodyIdA = bodyA.m_bodyId;
    jointDef.bodyIdB = bodyB.m_bodyId;
    jointDef.localAnchorA = b2Body_GetLocalPoint(bodyA.m_bodyId, b2anchor);
    jointDef.localAnchorB = b2Body_GetLocalPoint(bodyB.m_bodyId, b2anchor);
    jointDef.localAxisA = b2Body_GetLocalVector(bodyA.m_bodyId, b2axis);

    return WheelJoint(b2CreateWheelJoint(m_worldId, &jointDef));
}

void World::step(float timeStep, int subStepCount)
{
    _checkValid();
    b2World_Step(m_worldId, timeStep, subStepCount);
}

std::vector<Collision> World::getCollisions()
{
    _checkValid();
    b2ContactEvents events = b2World_GetContactEvents(m_worldId);

    std::vector<Collision> collisions;
    collisions.reserve(events.hitCount);
    for (int i = 0; i < events.hitCount; ++i)
    {
        const b2ContactHitEvent* event = events.hitEvents + i;
        Collision c;
        c.bodyA = Body(b2Shape_GetBody(event->shapeIdA));
        c.bodyB = Body(b2Shape_GetBody(event->shapeIdB));
        c.point = {event->point.x, event->point.y};
        c.normal = {event->normal.x, event->normal.y};
        c.approachSpeed = event->approachSpeed;
        collisions.push_back(c);
    }

    return collisions;
}

bool World::QueryCallback(b2ShapeId shapeId, void* context)
{
    auto* ctx = static_cast<QueryContext*>(context);
    if (ctx->isPointQuery)
    {
        if (!b2Shape_TestPoint(shapeId, static_cast<b2Vec2>(ctx->point)))
            return true;
    }

    ctx->bodies.push_back(Body(b2Shape_GetBody(shapeId)));
    return true;
}

std::vector<Body> World::queryPoint(const Vec2& point)
{
    _checkValid();
    b2AABB aabb;
    constexpr float d = 0.001f;
    aabb.lowerBound = {static_cast<float>(point.x) - d, static_cast<float>(point.y) - d};
    aabb.upperBound = {static_cast<float>(point.x) + d, static_cast<float>(point.y) + d};

    QueryContext ctx;
    ctx.point = point;
    ctx.isPointQuery = true;
    b2World_OverlapAABB(m_worldId, aabb, b2DefaultQueryFilter(), QueryCallback, &ctx);

    return ctx.bodies;
}

std::vector<Body> World::queryAABB(const Rect& rect)
{
    _checkValid();
    b2AABB aabb;
    aabb.lowerBound = static_cast<b2Vec2>(rect.getTopLeft());
    aabb.upperBound = static_cast<b2Vec2>(rect.getBottomRight());

    QueryContext ctx;
    ctx.isPointQuery = false;
    b2World_OverlapAABB(m_worldId, aabb, b2DefaultQueryFilter(), QueryCallback, &ctx);

    return ctx.bodies;
}

float World::CastCallback(
    b2ShapeId shapeId, b2Vec2 point, b2Vec2 normal, float fraction, void* context
)
{
    auto* hits = static_cast<std::vector<CastHit>*>(context);
    CastHit hit;
    hit.body = Body(b2Shape_GetBody(shapeId));
    hit.point = {point.x, point.y};
    hit.normal = {normal.x, normal.y};
    hit.fraction = fraction;
    hits->push_back(hit);
    return 1.0f;
}

std::vector<CastHit> World::rayCast(const Vec2& origin, const Vec2& translation)
{
    _checkValid();
    std::vector<CastHit> hits;
    b2World_CastRay(
        m_worldId, static_cast<b2Vec2>(origin), static_cast<b2Vec2>(translation),
        b2DefaultQueryFilter(), CastCallback, &hits
    );

    std::sort(
        hits.begin(), hits.end(),
        [](const CastHit& a, const CastHit& b) { return a.fraction < b.fraction; }
    );

    return hits;
}

std::vector<CastHit> World::shapeCast(
    const Circle& circle, const Transform& transform, const Vec2& translation
)
{
    _checkValid();
    b2Transform b2xf = static_cast<b2Transform>(transform);
    b2Vec2 worldCenter = b2TransformPoint(b2xf, static_cast<b2Vec2>(circle.pos));
    b2ShapeProxy proxy = b2MakeProxy(&worldCenter, 1, static_cast<float>(circle.radius));

    std::vector<CastHit> hits;
    b2World_CastShape(
        m_worldId, &proxy, static_cast<b2Vec2>(translation), b2DefaultQueryFilter(), CastCallback,
        &hits
    );

    std::sort(
        hits.begin(), hits.end(),
        [](const CastHit& a, const CastHit& b) { return a.fraction < b.fraction; }
    );

    return hits;
}

std::vector<CastHit> World::shapeCast(
    const Capsule& capsule, const Transform& transform, const Vec2& translation
)
{
    _checkValid();
    b2Transform b2xf = static_cast<b2Transform>(transform);
    b2Vec2 worldPoints[2] = {
        b2TransformPoint(b2xf, static_cast<b2Vec2>(capsule.p1)),
        b2TransformPoint(b2xf, static_cast<b2Vec2>(capsule.p2)),
    };
    b2ShapeProxy proxy = b2MakeProxy(worldPoints, 2, static_cast<float>(capsule.radius));

    std::vector<CastHit> hits;
    b2World_CastShape(
        m_worldId, &proxy, static_cast<b2Vec2>(translation), b2DefaultQueryFilter(), CastCallback,
        &hits
    );

    std::sort(
        hits.begin(), hits.end(),
        [](const CastHit& a, const CastHit& b) { return a.fraction < b.fraction; }
    );

    return hits;
}

std::vector<CastHit> World::shapeCast(
    const Polygon& polygon, const Transform& transform, const Vec2& translation
)
{
    _checkValid();
    if (polygon.points.size() < 3)
        return {};

    std::vector<b2Vec2> b2Points;
    b2Points.reserve(polygon.points.size());
    for (const auto& p : polygon.points)
        b2Points.push_back(static_cast<b2Vec2>(p));

    b2Hull hull = b2ComputeHull(b2Points.data(), static_cast<int>(b2Points.size()));
    b2Polygon poly = b2MakePolygon(&hull, 0.0f);

    b2Transform b2xf = static_cast<b2Transform>(transform);
    b2Vec2 worldPoints[B2_MAX_POLYGON_VERTICES];
    for (int i = 0; i < poly.count; ++i)
        worldPoints[i] = b2TransformPoint(b2xf, poly.vertices[i]);

    b2ShapeProxy proxy = b2MakeProxy(worldPoints, poly.count, poly.radius);

    std::vector<CastHit> hits;
    b2World_CastShape(
        m_worldId, &proxy, static_cast<b2Vec2>(translation), b2DefaultQueryFilter(), CastCallback,
        &hits
    );

    std::sort(
        hits.begin(), hits.end(),
        [](const CastHit& a, const CastHit& b) { return a.fraction < b.fraction; }
    );

    return hits;
}

std::vector<CastHit> World::shapeCast(
    const Rect& rect, const Transform& transform, const Vec2& translation
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
    b2Polygon poly = b2MakePolygon(&hull, 0.0f);

    b2Transform b2xf = static_cast<b2Transform>(transform);
    b2Vec2 worldPoints[B2_MAX_POLYGON_VERTICES];
    for (int i = 0; i < poly.count; ++i)
        worldPoints[i] = b2TransformPoint(b2xf, poly.vertices[i]);

    b2ShapeProxy proxy = b2MakeProxy(worldPoints, poly.count, 0.0f);

    std::vector<CastHit> hits;
    b2World_CastShape(
        m_worldId, &proxy, static_cast<b2Vec2>(translation), b2DefaultQueryFilter(), CastCallback,
        &hits
    );

    std::sort(
        hits.begin(), hits.end(),
        [](const CastHit& a, const CastHit& b) { return a.fraction < b.fraction; }
    );

    return hits;
}

StaticBody World::fromMapLayer(const tilemap::Layer& layer)
{
    _checkValid();
    if (layer.getType() != tmx::Layer::Type::Object)
    {
        throw std::runtime_error("Layer must be an ObjectGroup to create physics bodies.");
    }

    auto& objGroup = dynamic_cast<const tilemap::ObjectGroup&>(layer);
    StaticBody body(*this);

    for (const auto& obj : objGroup.getObjects())
    {
        if (!obj.visible)
            continue;

        const auto shape = obj.getShapeType();
        if (shape == tmx::Object::Shape::Rectangle)
        {
            body.addCollider(obj.getRect());
        }
        else if (shape == tmx::Object::Shape::Polygon)
        {
            body.addCollider(Polygon(obj.getVertices()));
        }
        // Skipping Point, Polyline (Lines), Ellipse, Text
    }

    return body;
}

void World::setGravity(const Vec2& gravity)
{
    _checkValid();
    b2World_SetGravity(m_worldId, static_cast<b2Vec2>(gravity));
}

Vec2 World::getGravity() const
{
    _checkValid();
    const b2Vec2 gravity = b2World_GetGravity(m_worldId);
    return {gravity.x, gravity.y};
}

bool World::isValid() const
{
    return b2World_IsValid(m_worldId);
}

void World::_checkValid() const
{
    if (!b2World_IsValid(m_worldId))
        throw std::runtime_error("Attempted to use an invalid or destroyed World");
}

void World::_checkBodiesForJoint(const Body& bodyA, const Body& bodyB) const
{
    if (!bodyA.isValid() || !bodyB.isValid())
        throw std::invalid_argument("Both bodies must be valid to create a joint.");

    const b2WorldId worldA = b2Body_GetWorld(bodyA.m_bodyId);
    const b2WorldId worldB = b2Body_GetWorld(bodyB.m_bodyId);
    if (worldA.index1 != m_worldId.index1 || worldB.index1 != m_worldId.index1)
        throw std::invalid_argument("Both bodies must belong to this World to create a joint.");
}

b2WorldId World::_getWorldId() const
{
    return m_worldId;
}
}  // namespace kn::physics

#include <algorithm>
#include <cmath>

#include "Capsule.hpp"
#include "Circle.hpp"
#include "Polygon.hpp"
#include "Time.hpp"
#include "Transform.hpp"
#include "physics/World.hpp"
#include "physics/bodies/CharacterBody.hpp"

namespace kn::physics
{
CharacterBody::CharacterBody(World& world)
    : Body(b2_nullBodyId),
      floorMaxAngle(math::toRadians(45.0))
{
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_kinematicBody;
    m_bodyId = b2CreateBody(world._getWorldId(), &bodyDef);
    m_pWorld = &world;
}

void CharacterBody::moveAndSlide(double dt)
{
    if (!isValid())
        throw std::runtime_error("Invalid CharacterBody cannot move");

    m_isOnFloor = false;
    m_isOnCeiling = false;
    m_isOnWall = false;

    constexpr int kMaxSlides = 4;
    constexpr double kSkin = 0.01;  // small separation to prevent re-hitting same surface

    const Vec2 downDir(0, 1);
    const Vec2 upDir(0, -1);

    // Dot threshold for "floor" classification
    const double floorMinDot = std::cos(floorMaxAngle);

    Transform transform;
    transform.pos = getPos();

    if (dt < 0.0)
        dt = time::getDelta();

    Vec2 remaining = velocity * dt;

    auto classifySurface = [&](const Vec2& n)
    {
        // n is normalized
        const double dotUp = std::clamp(math::dot(n, upDir), -1.0, 1.0);
        const double dotDown = std::clamp(math::dot(n, downDir), -1.0, 1.0);

        if (dotUp >= floorMinDot)
            m_isOnFloor = true;
        else if (dotDown >= floorMinDot)
            m_isOnCeiling = true;
        else
            m_isOnWall = true;
    };

    auto findClosestBlockingHit = [&](const std::vector<CastHit>& hits, const Vec2& motion,
                                      CastHit& outHit) -> bool
    {
        bool found = false;
        double bestFrac = 1.0;

        for (const auto& h : hits)
        {
            if (h.body == *this)
                continue;

            const Vec2 n = h.normal.normalized();

            double dn = math::dot(motion, n);
            if (dn > 0.0)
                dn = -dn;

            constexpr double kBlockEps = 1e-6;
            if (dn >= -kBlockEps)
                continue;  // not a blocking hit

            if (h.fraction < bestFrac)
            {
                bestFrac = h.fraction;
                outHit = h;
                found = true;
            }
        }

        return found;
    };

    // --- Cast & slide loop ---
    for (int iter = 0; iter < kMaxSlides; ++iter)
    {
        if (remaining.isZero())
            break;

        const std::vector<CastHit> hits = _castShapes(transform, remaining);

        CastHit hit;
        if (!findClosestBlockingHit(hits, remaining, hit))
        {
            // No collision: take full remaining move
            transform.pos += remaining;
            remaining = {0, 0};
            break;
        }

        if (hit.body._getType() == b2_dynamicBody)
        {
            const Vec2 n = hit.normal.normalized();
            const double vFn = math::dot(velocity, n);
            if (vFn < 0.0)
            {
                const Vec2 pushDir = -n;
                const double impulseMag = mass * (-vFn);
                const auto impulse = static_cast<b2Vec2>(pushDir * impulseMag);
                const auto point = static_cast<b2Vec2>(hit.point);
                b2Body_ApplyLinearImpulse(hit.body._getBodyId(), impulse, point, true);
            }
        }

        Vec2 n = hit.normal.normalized();

        // Move up to the hit point (minus skin)
        // fraction is along remaining, so traveled vector is remaining * fraction
        const double frac = std::clamp(hit.fraction, 0.0f, 1.0f);

        if (math::dot(remaining, n) > 0.0)
            n = -n;

        const double len = remaining.getLength();
        const double skinFrac = (len > 0.0) ? (kSkin / len) : 0.0;
        const double safeFrac = std::max(0.0, frac - skinFrac);

        transform.pos += remaining * safeFrac;  // stop short along travel
        transform.pos += n * kSkin;             // tiny push off surface

        // Classify floor/wall/ceiling
        classifySurface(n);

        // Slide: remove normal component from remaining motion *after* the hit
        Vec2 remainingAfter = remaining * (1.0 - safeFrac);

        // Remove into-normal component so we slide along surface
        const double into = math::dot(remainingAfter, n);
        if (into < 0.0)  // only if it's going into the surface
            remainingAfter -= into * n;

        remaining = remainingAfter;

        // Also slide the actual velocity so it doesn't keep pushing into the wall/floor next frame
        const double vInto = math::dot(velocity, n);
        if (vInto < 0.0)
            velocity -= vInto * n;
    }

    setPos(transform.pos);

    // --- Floor snap ---
    // Prevent snapping upward into floors when jumping.
    if (m_isOnFloor || floorSnapDistance <= 0.0 || velocity.y < 0.0)
        return;

    Transform snapT;
    snapT.pos = getPos();

    const Vec2 snapProbe(0.0, floorSnapDistance);
    auto probeHits = _castShapes(snapT, snapProbe);

    CastHit hit;
    if (!findClosestBlockingHit(probeHits, snapProbe, hit))
        return;

    Vec2 n = hit.normal.normalized();
    if (math::dot(snapProbe, n) > 0.0)
        n = -n;

    const double dotUp = std::clamp(math::dot(n, upDir), -1.0, 1.0);
    if (dotUp < floorMinDot)
        return;

    // Move down by the hit distance
    const double frac = std::clamp(hit.fraction, 0.0f, 1.0f);
    const Vec2 snapMove = snapProbe * frac;
    const Vec2 newPos = snapT.pos + snapMove + n * kSkin;

    setPos(newPos);

    m_isOnFloor = true;
    if (velocity.y > 0.0)
        velocity.y = 0.0;
}

std::vector<CastHit> CharacterBody::_castShapes(
    const Transform& transform, const Vec2& translation
) const
{
    std::vector<CastHit> hits;
    for (const b2ShapeId shapeId : _getShapeIds())
    {
        if (!b2Shape_IsValid(shapeId))
            continue;

        const b2ShapeType shapeType = b2Shape_GetType(shapeId);

        if (shapeType == b2_circleShape)
        {
            const b2Circle b2circle = b2Shape_GetCircle(shapeId);
            const Circle circle{{b2circle.center.x, b2circle.center.y}, b2circle.radius};
            const auto shapeHits = m_pWorld->shapeCast(circle, transform, translation);
            hits.insert(hits.end(), shapeHits.begin(), shapeHits.end());
        }
        else if (shapeType == b2_capsuleShape)
        {
            const b2Capsule b2capsule = b2Shape_GetCapsule(shapeId);
            const Capsule capsule{
                {b2capsule.center1.x, b2capsule.center1.y},
                {b2capsule.center2.x, b2capsule.center2.y},
                b2capsule.radius
            };
            const auto shapeHits = m_pWorld->shapeCast(capsule, transform, translation);
            hits.insert(hits.end(), shapeHits.begin(), shapeHits.end());
        }
        else if (shapeType == b2_polygonShape)
        {
            const b2Polygon b2polygon = b2Shape_GetPolygon(shapeId);
            std::vector<Vec2> points;
            for (int i = 0; i < b2polygon.count; ++i)
            {
                points.emplace_back(b2polygon.vertices[i].x, b2polygon.vertices[i].y);
            }
            const Polygon polygon(points);
            const auto shapeHits = m_pWorld->shapeCast(polygon, transform, translation);
            hits.insert(hits.end(), shapeHits.begin(), shapeHits.end());
        }
    }
    return hits;
}

bool CharacterBody::isOnFloor() const
{
    return m_isOnFloor;
}

bool CharacterBody::isOnCeiling() const
{
    return m_isOnCeiling;
}

bool CharacterBody::isOnWall() const
{
    return m_isOnWall;
}
}  // namespace kn::physics

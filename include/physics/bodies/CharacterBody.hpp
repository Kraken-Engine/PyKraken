#pragma once

#include <vector>

#include "Math.hpp"
#include "physics/bodies/Body.hpp"

namespace kn
{
class Capsule;
struct Transform;

namespace physics
{
class World;
struct CastHit;

class CharacterBody : public Body
{
  public:
    double floorMaxAngle;
    double floorSnapDistance = 5.0;
    double mass = 80.0;
    Vec2 velocity;

    CharacterBody(World& world);
    ~CharacterBody() = default;

    void moveAndSlide(double dt = -1.0);

    bool isOnFloor() const;
    bool isOnCeiling() const;
    bool isOnWall() const;

  private:
    bool m_isOnFloor = false;
    bool m_isOnCeiling = false;
    bool m_isOnWall = false;

    World* m_pWorld = nullptr;

    std::vector<CastHit> _castShapes(const Transform& transform, const Vec2& translation) const;
};
}  // namespace physics
}  // namespace kn

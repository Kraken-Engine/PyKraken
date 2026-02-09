#pragma once

#include "physics/bodies/Body.hpp"

namespace kn
{
namespace physics
{
class World;

class StaticBody : public Body
{
  public:
    StaticBody(World& world);
    ~StaticBody() = default;
};
}  // namespace physics
}  // namespace kn

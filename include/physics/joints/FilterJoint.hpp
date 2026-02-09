#pragma once

#include "physics/joints/Joint.hpp"

namespace kn::physics
{
class FilterJoint : public Joint
{
  public:
    FilterJoint() = default;

  protected:
    FilterJoint(b2JointId jointId);

    friend class World;
};
}  // namespace kn::physics

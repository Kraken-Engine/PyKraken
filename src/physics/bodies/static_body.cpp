#include "physics/World.hpp"
#include "physics/bodies/StaticBody.hpp"

namespace kn::physics
{
StaticBody::StaticBody(World& world)
    : Body(b2_nullBodyId)
{
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_staticBody;
    m_bodyId = b2CreateBody(world._getWorldId(), &bodyDef);
}
}  // namespace kn::physics

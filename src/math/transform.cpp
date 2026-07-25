#include "kraken/math/Transform.hpp"

#include <box2d/box2d.h>

namespace kn
{
Transform::operator b2Transform() const
{
    b2Transform xf;
    xf.p = static_cast<b2Vec2>(pos);
    xf.q = b2MakeRot(static_cast<float>(angle));
    return xf;
}
}  // namespace kn

namespace kn::transform
{
Transform composePair(const Transform& parent, Transform child)
{
    // Child translation is affected by parent scale + rotation
    child.pos *= parent.scale;
    child.pos.rotate(parent.angle);

    child.pos += parent.pos;
    child.angle += parent.angle;
    child.scale *= parent.scale;

    return child;
}

}  // namespace kn::transform

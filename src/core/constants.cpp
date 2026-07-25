#include "kraken/core/Constants.hpp"

#include <SDL3/SDL.h>

#include "kraken/core/_globals.hpp"
#include "kraken/input/Event.hpp"
#include "kraken/math/Math.hpp"

const kn::Vec2 kn::Anchor::TOP_LEFT = {0.0, 0.0};
const kn::Vec2 kn::Anchor::TOP_MID = {0.5, 0.0};
const kn::Vec2 kn::Anchor::TOP_RIGHT = {1.0, 0.0};
const kn::Vec2 kn::Anchor::MID_LEFT = {0.0, 0.5};
const kn::Vec2 kn::Anchor::CENTER = {0.5, 0.5};
const kn::Vec2 kn::Anchor::MID_RIGHT = {1.0, 0.5};
const kn::Vec2 kn::Anchor::BOTTOM_LEFT = {0.0, 1.0};
const kn::Vec2 kn::Anchor::BOTTOM_MID = {0.5, 1.0};
const kn::Vec2 kn::Anchor::BOTTOM_RIGHT = {1.0, 1.0};

#pragma once

#include "meta_program.hpp"

// types.hpp: This is the file you edit when adding new component or new tag

#include "../components/render_component.hpp"
#include "../components/physics_component.hpp"

// Components here
using GameCMPs = MP::TypeList<
    RenderComponent,
    PhysicsComponent
>;

// Only bitmask, no data
struct TagPlayer  {};
struct TagEnemy   {};

// Tags here
using GameTAGs = MP::TypeList<
    TagPlayer,
    TagEnemy
>;
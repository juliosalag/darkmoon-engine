#pragma once

// component_list.hpp: This is the file you edit when adding new component

#include "render_component.hpp"
#include "physics_component.hpp"

// A variadic type-list. No data, no vtable - exists only at
// compile time so the rest of the ECS can iterate over it.
template <typename... Ts>
struct ComponentList {};

// Components here
using GameComponents = ComponentList<
    RenderComponent,
    PhysicsComponent
>;
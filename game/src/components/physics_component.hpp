#pragma once

#include<darkmoon.hpp>

struct PhysicsComponent{

    Vector2D position {};
    Vector2D velocity {};

    PhysicsComponent() = default;

    PhysicsComponent(Vector2D pos, Vector2D vel) : position(pos), velocity(vel) {}
};
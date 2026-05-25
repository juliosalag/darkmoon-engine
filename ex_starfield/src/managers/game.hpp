#pragma once

#include <darkmoon/graphics.hpp>

#include "../utils/types.hpp"

#include "../systems/render_system.hpp"
#include "../systems/physics_system.hpp"
#include "../systems/debug_system.hpp"

struct Game{
public:
    Game() = default;
    ~Game() = default;

    void run();

    void CreateStar(Window& w);
    void CreateStars(int numStars, Window& w);
    void DeleteRandomStar(Window& w);

private:
    DarkMoonEngine m_darkmoon {};

    EM m_em {};
    RenderSystem m_renderSystem {};
    PhysicsSystem m_physicsSystem {};
};
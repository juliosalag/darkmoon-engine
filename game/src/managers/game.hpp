#pragma once

#include <darkmoon.hpp>

#include "./managers/entity_manager.hpp"
#include "./systems/render_system.hpp"

struct Game{
public:
    Game() = default;
    ~Game() = default;

    void run();

private:
    DarkMoonEngine m_darkmoon {};

    EntityManager m_entityManager {};
    RenderSystem m_renderSystem {};
};

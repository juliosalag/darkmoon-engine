#pragma once

#include <darkmoon.hpp>

#include "./managers/entity_manager.hpp"
#include "./systems/render_system.hpp"

struct Game{
public:
    Game() = default;
    ~Game() = default;

    using EM = EntityManager<GameCMPs, GameTAGs>;
    using Entity = EM::Entity;

    void run();

private:
    DarkMoonEngine m_darkmoon {};

    EM m_em {};
    RenderSystem m_renSys {};
};

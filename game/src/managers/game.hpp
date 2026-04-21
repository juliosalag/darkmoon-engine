#pragma once

#include <darkmoon.hpp>

#include "./utils/types.hpp"

#include "./managers/entity_manager.hpp"
#include "./systems/render_system.hpp"

struct Game{
public:
    Game() = default;
    ~Game() = default;

    void run();

private:
    DarkMoonEngine m_darkmoon {};

    EM m_em {};
    RenderSystem m_renderSystem {};
};

#pragma once

#include "darkmoon.hpp"

struct Game{
public:
    Game() = default;
    ~Game() = default;

    void run();

private:
    DarkMoonEngine m_dm{};
};

#pragma once

#include <darkmoon.hpp>
#include "../managers/entity_manager.hpp"

struct RenderComponent {
    Vector2D position;
    std::unique_ptr<Text> text;
    std::string font { "./assets/roboto.ttf" };
    Window* window;

    RenderComponent(Vector2D pos, const char* texto, Window* w) 
        : position(pos), window(w) 
    {
        text = std::make_unique<Text>(position, texto, font.c_str(), window);
    };
};
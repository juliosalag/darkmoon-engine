#pragma once

#include <darkmoon.hpp>

struct RenderComponent {

    Vector2D position {};
    Window* window {};
    std::unique_ptr<Text> text {};
    std::string font { "./assets/roboto.ttf" };

    RenderComponent() = default;

    RenderComponent(Vector2D pos, const char* texto, Window* w) 
        : position(pos), window(w)
    {
        text = std::make_unique<Text>(position, texto, font.c_str(), window);
        
        //text.SetText("Prueba");
        text->SetColor(WHITE);
    };
};
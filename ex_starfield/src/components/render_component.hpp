#pragma once

#include <darkmoon/darkmoon.hpp>

struct RenderComponent {

    Color color {WHITE};
    std::unique_ptr<Pixel> pixel {};

    uint8_t size { 1 };

    RenderComponent() = default;

    RenderComponent(Color col, int siz, Window& w) 
        : color(col), size(static_cast<uint8_t>(siz))
    {
        pixel = std::make_unique<Pixel>(Vector2D{}, color, size, &w);
        pixel->SetSize(size);
    };
};
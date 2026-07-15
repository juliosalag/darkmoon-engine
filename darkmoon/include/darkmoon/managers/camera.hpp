#pragma once
#include <darkmoon/utils/math.hpp>
#include <cmath>

struct Camera2D {
    Vector2Df target  {};        // World Point Looking at Camera
    Vector2Df offset  {};        // Where the target appears on screen (px)
    float     zoom     { 1.0f };
    float     rotation { 0.0f }; // In degrees

    // World to Screen
    Vector2Df WorldToScreen(Vector2Df worldPos) const {
        Vector2Df v = { worldPos.x - target.x,
                        worldPos.y - target.y };

        if (rotation != 0.0f) {
            float rad = rotation * (3.14159265f / 180.0f);
            float cosR = std::cos(rad), sinR = std::sin(rad);
            v = { v.x * cosR - v.y * sinR,
                  v.x * sinR + v.y * cosR };
        }

        return { v.x * zoom + offset.x,
                 v.y * zoom + offset.y };
    }

    // Screen to World
    Vector2Df ScreenToWorld(Vector2Df screenPos) const {
        Vector2Df v = { (screenPos.x - offset.x) / zoom,
                        (screenPos.y - offset.y) / zoom };

        if (rotation != 0.0f) {
            float rad = -rotation * (3.14159265f / 180.0f);
            float cosR = std::cos(rad), sinR = std::sin(rad);
            v = { v.x * cosR - v.y * sinR,
                  v.x * sinR + v.y * cosR };
        }

        return { v.x + target.x, v.y + target.y };
    }
};
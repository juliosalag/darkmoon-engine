#pragma once

#include <cstddef>
#include <iostream>
#include <vector>
#include <map>
#include <optional>

#include "./managers/resource_manager.hpp"
#include "./managers/window.hpp"
#include "./managers/monitor.hpp"

#include "./2D/2D.hpp"

#include "utils/keys.hpp"
#include "utils/math.hpp"

struct DarkMoonEngine{
public:
    DarkMoonEngine();
    ~DarkMoonEngine() { 
        m_resourceManager.unloadAllResources();
        glfwTerminate(); 
    };


    // Pixel //
    // Line //
    // Triangle //
    // Rectangle //


    // - Rectangle / Rounded
    // - Circle / Sector
    // - Ellipse
    // - Ring
    // - Regular Polygon
    // - Poly Lines
    // - Splines
    // - Bezier

    // Texture //


    // Textures / AnimatedTextures / Gifs
    // Text

    // TextBox / Aligned
    // Button
    // Checkbox
    // Slider
    // OptionSlider
    // Interactive TextBox 

    // Trans Matrix (Traslacion, Rotacion y Escalado)
    
    // MENU DEBUG // 

    // TODO, Comment code

    // ECS //

    // SoundManager //

    // To linux, To Web //

    // ---------------- //

    // Normalize coords in window (0/1 to width/height)
    //float normalizeX(float x) { return (x / static_cast<float>(m_activeWindow->GetWidth())) * 2 - 1; };
    //float normalizeY(float y) { return -((y / static_cast<float>(m_activeWindow->GetHeight())) * 2 - 1); };

private:
    ResourceManager& m_resourceManager = ResourceManager::getInstance();
};
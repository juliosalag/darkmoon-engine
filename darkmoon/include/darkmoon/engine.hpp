#pragma once

#include <cstddef>
#include <iostream>
#include <vector>
#include <map>
#include <optional>

#include <darkmoon/managers/resource_manager.hpp>
#include <darkmoon/managers/window.hpp>
#include <darkmoon/managers/monitor.hpp>

#include <darkmoon/2D/2D.hpp>

#include <darkmoon/utils/keys.hpp>
#include <darkmoon/utils/math.hpp>

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
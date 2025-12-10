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

    // ---------------- //
    // Window Functions //
    // ---------------- //

    //Window* CreateInitWindow(int width, int height, const char* title);
    //Window* CreateSharedWindow(int width, int height, const char* title);

    Window* GetActiveWindow() { return m_activeWindow; };

    void FocusWindow(Window& win);

    // ----------------- //
    // Monitor Functions //
    // ----------------- //

    /*
    // Set the active monitor for the window
    void SetActiveMonitor(Monitor monitor);
    // Get the currently active monitor
    Monitor GetActiveMonitor() { return m_activeMonitor; };
    // Get a list of all available monitors
    std::vector<Monitor> GetAllAvailableMonitors();
    */

    // ---------------- //
    // Render Functions //
    // ---------------- //

    void BeginDrawing(Window& window);
    void BeginDrawing(Window& window, Color color);
    void EndDrawing(Window& window);
    void ClearBackground(Window& window, Color color);

    // Pixel //

    void DrawPixel(Vector2D position, Color color, Window* window, int size = 1, Shader* shader = nullptr);

    // Line //

    void DrawLine(Vector2D startPosition, Vector2D endPosition, Color color, int width = 1, Shader* shader = nullptr);

    // Triangle //

    void DrawTriangle(Vector2D vertexA, Vector2D vertexB, Vector2D vertexC, Color color, Shader* shader = nullptr);
    void DrawTriangleLines(Vector2D vertexA, Vector2D vertexB, Vector2D vertexC, Color color, int width = 1, Shader* shader = nullptr);

    // Rectangle //

    void DrawRectangle(Vector2D vertexA, Vector2D vertexB, Vector2D vertexC, Vector2D vertexD, Color color, Shader* shader = nullptr);
    void DrawRectangleLines(Vector2D vertexA, Vector2D vertexB, Vector2D vertexC, Vector2D vertexD, Color color, int width = 1, Shader* shader = nullptr);

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

    // -------------------------- //
    // Resource Manager Functions //
    // -------------------------- //

    // Shader //

    Shader* CreateShader(const char* vertexPath = "", const char* fragmentPath = "", const char* geometryPath = ""){
        return m_resourceManager.loadResource<Shader>(vertexPath, fragmentPath, geometryPath);
    }

    Shader* CreateShader(const std::string& vertexCode = "", const std::string& fragmentCode  = "", const std::string& geometryCode = ""){
        return m_resourceManager.loadResource<Shader>((vertexCode + fragmentCode + geometryCode).c_str(), vertexCode, fragmentCode, geometryCode);
    }

    void LoadBasicShaders();

    Shader* GetBasicShader2D(){ return m_shaders["basic2D"]; };

    // Texture //

    //Texture* LoadTexture(const char* texturePath){
    //    return m_resourceManager.loadResource<Texture>(texturePath);
    //}

    // Font //

    // Normalize coords in window (0/1 to width/height)
    float normalizeX(float x) { return (x / static_cast<float>(m_activeWindow->GetWidth())) * 2 - 1; };
    float normalizeY(float y) { return -((y / static_cast<float>(m_activeWindow->GetHeight())) * 2 - 1); };

private:
    Window* m_activeWindow {};

    Monitor m_activeMonitor {};
    ResourceManager& m_resourceManager = ResourceManager::getInstance();
    std::map<std::string, Shader*> m_shaders;
};
#pragma once

#include <iostream>

#include "./managers/resource_manager.hpp"
#include "./managers/window.hpp"
#include "./utils/color.hpp"

struct DarkMoonEngine{
public:
    DarkMoonEngine() {};
    ~DarkMoonEngine() { glfwTerminate(); };

    // ---------------- //
    // Window Functions //
    // ---------------- //

    // Init GLFW, create window, load OpenGL functions pointers and configure OpenGL
    bool InitWindow(int width, int height, const char* title);
    // Destroy and close window
    void CloseWindow();
    // Return if window should close
    bool WindowShouldClose();

    // Create Window
    // Get/Set Active Window

    void SetFullscreen();
    void SetBorderless();
    void SetWindowed();

    WindowMode GetWindowMode(){ return m_activeWindow.mode; };

    void BeginDrawing();
    void EndDrawing();
    void ClearBackground(Color color);

    int IsKeyPressed(int key){ return glfwGetKey(m_activeWindow.window, key) == GLFW_PRESS; };

private:
    Window m_activeWindow {};
    ResourceManager& m_resourceManager = ResourceManager::getInstance();
};
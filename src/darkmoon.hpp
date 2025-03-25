#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "./managers/resource_manager.hpp"
#include "./utils/color.hpp"

struct DarkMoonEngine{
public:
    DarkMoonEngine() {};
    ~DarkMoonEngine() { glfwTerminate(); };

    bool InitWindow(int width, int height, const char* title);
    void CloseWindow();
    bool WindowShouldClose();

    // Create Window
    // Get/Set Active Window

    // Functions of Window

    void BeginDrawing();
    void EndDrawing();
    void ClearBackground(Color color);

private:
    GLFWwindow* m_activeWindow {};
    ResourceManager& m_resourceManager = ResourceManager::getInstance();
};
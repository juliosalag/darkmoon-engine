#pragma once

#include "./managers/window_manager.hpp"
#include "./managers/resource_manager.hpp"

struct DarkMoonEngine{
public:
    DarkMoonEngine() {};
    ~DarkMoonEngine() {};

    // Initializes the window, sets up GLFW, GLAD, GLEW, and OpenGL context // TODO commentary
    bool initWindow(int width, int height, const char* title);
    // Closes the window and terminates GLFW
    void closeWindow();
    // Returns whether the window should close
    bool windowShouldClose();

    // Sets up the projection matrix for 2D drawing
    void beginDrawing();
    // Swaps buffers and polls window events
    void endDrawing();

    // Set background color
    void clearBackground(Color color);

private:
    WindowManager& m_windowManager = WindowManager::getInstance();
    ResourceManager& m_resourceManager = ResourceManager::getInstance();
};
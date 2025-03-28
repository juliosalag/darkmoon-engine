#pragma once

#include <iostream>
#include <vector>

#include "./managers/resource_manager.hpp"
#include "./managers/window.hpp"
#include "./managers/monitor.hpp"
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
    void SetWindowMode(WindowMode mode);
    WindowMode GetWindowMode(){ return m_activeWindow.mode; };

    void SetWindowSize(int width, int height);
    int GetWindowWidth();
    int GetWindowHeight();

    void SetWindowPosition(int x, int y);
    int GetWindowPositionX();
    int GetWindowPositionY();

    void SetWindowTitle(const char* title);
    // const char* GetWindowTitle(); // ADDED IN 3.4, I ALREADY USE 3.3

    void SetWindowIcon(const char* iconPath);
    
    // Monitor --> monitor_guide.html  (SET | GET | GETS)
    void SetActiveMonitor(Monitor monitor);
    Monitor GetActiveMonitor() { return m_activeMonitor; };
    std::vector<Monitor> GetAllAvailableMonitors();

    // Iconification  (ICONIFY | MAXIMIZE | RESTORE)
    void IconifyWindow();
    void MaximizeWindow();
    void RestoreWindow();
    //bool IsWindowIconify();

    // Visibility (HIDE | SHOW)
    void HideWindow();
    void ShowWindow();

    // Input Focus (FOCUS)
    void FocusWindow();
    // Transparency ?
    void SetWindowOpacity(float opacity);
    float GetWindowOpacity();

    // ATTRIBUTTES

    // Buffer Swapping --> Render


    // ---------------- //
    // Render Functions //
    // ---------------- //

    void BeginDrawing();
    void EndDrawing();
    void ClearBackground(Color color);

    // --------------- //
    // Input Functions // --> input_guide.html
    // --------------- //

    int IsKeyPressed(int key){ return glfwGetKey(m_activeWindow.window, key) == GLFW_PRESS; };

private:
    Window m_activeWindow {};
    Monitor m_activeMonitor {};
    ResourceManager& m_resourceManager = ResourceManager::getInstance();
};
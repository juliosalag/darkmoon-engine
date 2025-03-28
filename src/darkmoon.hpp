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
    // Destroy the window and close it
    void CloseWindow();
    // Return whether the window should close
    bool WindowShouldClose();

    // Create Window
    // Get/Set Active Window

    // Set the window to fullscreen mode
    void SetFullscreen();
    // Set the window to borderless mode
    void SetBorderless();
    // Set the window to windowed mode
    void SetWindowed();
    // Set the window mode (Fullscreen, Borderless and Windowed)
    void SetWindowMode(WindowMode mode);
    // Get the current window mode
    WindowMode GetWindowMode(){ return m_activeWindow.mode; };

    // Set the window size
    void SetWindowSize(int width, int height);
    // Get the window width
    int GetWindowWidth();
    // Get the window height
    int GetWindowHeight();

    // Set the window position on the screen
    void SetWindowPosition(int x, int y);
    // Get the window's X position
    int GetWindowPositionX();
    // Get the window's Y position
    int GetWindowPositionY();

    // Set the window title
    void SetWindowTitle(const char* title);
    // const char* GetWindowTitle(); // ADDED IN 3.4, I ALREADY USE 3.3

    // Set the window icon from the specified file path
    void SetWindowIcon(const char* iconPath);
    
    // Set the active monitor for the window
    void SetActiveMonitor(Monitor monitor);
    // Get the currently active monitor
    Monitor GetActiveMonitor() { return m_activeMonitor; };
    // Get a list of all available monitors
    std::vector<Monitor> GetAllAvailableMonitors();

    // Iconify the window (minimize it)
    void IconifyWindow();
    // Maximife the window
    void MaximizeWindow();
    // Restore the window to its previous size and state
    void RestoreWindow();

    // Hide the window from view
    void HideWindow();
    // Show the window
    void ShowWindow();

    // Focus on the window (bring it to the front)
    void FocusWindow();

    // Set the window opacity (0.0 to 1.0)
    void SetWindowOpacity(float opacity);
    // Get the current window opacity level
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
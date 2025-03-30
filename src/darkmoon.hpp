#pragma once

#include <iostream>
#include <vector>

#include "./managers/resource_manager.hpp"
#include "./managers/window.hpp"
#include "./managers/monitor.hpp"
#include "./utils/color.hpp"

struct DarkMoonEngine{
public:
    DarkMoonEngine();
    ~DarkMoonEngine() { glfwTerminate(); };

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

    void BeginDrawing();
    void EndDrawing();
    void ClearBackground(Color color);

    // --------------- //
    // Input Functions // --> input_guide.html
    // --------------- //

    // int IsKeyPressed(int key){ return glfwGetKey(m_activeWindow.window, key) == GLFW_PRESS; };

private:
    Monitor m_activeMonitor {};
    ResourceManager& m_resourceManager = ResourceManager::getInstance();
};
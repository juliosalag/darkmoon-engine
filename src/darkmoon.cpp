#include "darkmoon.hpp"
#include "glad/glad.h"
#include "managers/window.hpp"

DarkMoonEngine::DarkMoonEngine(){
    // ----------------------------- //
    // Initialize and configure GLFW //
    // ----------------------------- //

std::cout << "---------------------------------\n";
std::cout << "| DarkMoon Engine - Version 2.0 |\n";
std::cout << "---------------------------------\n";

    if(!glfwInit()){
        std::cerr << "[ERROR] Failed to initialize GLFW\n";

        // throw std::runtime_error("Error en el constructor");
    }
    
    std::cout << "[OK] GLFW initialization successful\n";
}

// ---------------- //
// Window Functions //
// ---------------- //

/*
Window* DarkMoonEngine::CreateInitWindow(int width, int height, const char* title){
    if(!m_initWindow){
        m_initWindow = Window(width, height, title);
        LoadBasicShaders();
    }

    FocusWindow(*m_initWindow);

    return m_initWindow;
}
*/

/*
Window* DarkMoonEngine::CreateSharedWindow(int width, int height, const char* title){
    m_sharedWindows.push_back(std::make_unique<Window>(
        width, height, title, GetInitWindow().GetWindow()
    ));

    FocusWindow(*m_sharedWindows.back());

    return *m_sharedWindows.back();
}
*/

void DarkMoonEngine::FocusWindow(Window& win){
    m_activeWindow = &win;
    win.Focus();
}

// ---------------- //
// Render Functions //
// ---------------- //

void DarkMoonEngine::BeginDrawing(Window& window){
    window.BeginDrawing();
    m_activeWindow = &window;
}

void DarkMoonEngine::BeginDrawing(Window& window, Color color){
    window.BeginDrawing(color);
    m_activeWindow = &window;
}

void DarkMoonEngine::EndDrawing(Window& window){
    window.EndDrawing();
}

void DarkMoonEngine::ClearBackground(Window& window, Color color){
    window.ClearBackground(color);
}

// Pixel //

Pixel DarkMoonEngine::CreatePixel(Vector2D position, Color color, int size, ResourceShader* shader){
    return Pixel(position, color, size, m_activeWindow->GetWidth(), m_activeWindow->GetHeight(), shader != nullptr ? shader : m_shaders["basic2D"]);
}

void DarkMoonEngine::DrawPixel(Vector2D position, Color color, int size, ResourceShader* shader){
    auto pixel = CreatePixel(position, color, size, shader);
    pixel.Draw();
    pixel.Delete();
}

// Line //

Line DarkMoonEngine::CreateLine(Vector2D startPosition, Vector2D endPosition, Color color, int width, ResourceShader* shader){
    return Line(startPosition, endPosition, color, width, m_activeWindow->GetWidth(), m_activeWindow->GetHeight(), shader != nullptr ? shader : m_shaders["basic2D"]);
}

void DarkMoonEngine::DrawLine(Vector2D startPosition, Vector2D endPosition, Color color, int width, ResourceShader* shader){
    auto line = CreateLine(startPosition, endPosition, color, width, shader);
    line.Draw();
    line.Delete();
}

// -------------------------- //
// Resource Manager Functions //
// -------------------------- //

void DarkMoonEngine::LoadBasicShaders(){

    // Basic Shader 2D //

    const std::string basicVertex = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;

        void main(){
            gl_Position = vec4(aPos, 1.0);
        }
    )";

    const std::string basicFragment = R"(
        #version 330 core
        out vec4 FragColor;
        uniform vec4 customColor;

        void main(){
            FragColor = customColor;
        }
    )";

    m_shaders["basic2D"] = CreateShader(basicVertex, basicFragment);
    

}

/*
// --- //

void DarkMoonEngine::SetActiveMonitor(Monitor monitor){
    m_activeMonitor = monitor;

    if(m_activeWindow.mode == WindowMode::Fullscreen)
        SetFullscreen();
    else if(m_activeWindow.mode == WindowMode::Borderless)
        SetBorderless();
}

std::vector<Monitor> DarkMoonEngine::GetAllAvailableMonitors(){
    int count = 0;
    GLFWmonitor** monitors = glfwGetMonitors(&count);

    std::vector<Monitor> monitorList;
    for (int i = 0; i < count; i++)
        monitorList.push_back(Monitor(monitors[i]));

    return monitorList;
}

// --- //
*/
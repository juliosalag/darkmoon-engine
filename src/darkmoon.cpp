#include "darkmoon.hpp"
#include "2D/rectangle.hpp"
#include "2D/triangle.hpp"
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

void DarkMoonEngine::DrawPixel(Vector2D position, Color color, int size, Shader* shader){
    auto pixel = Pixel(position, color, size, m_activeWindow, shader != nullptr ? shader : m_shaders["basic2D"]);
    pixel.Draw();
    pixel.Delete();
}

// Line //

void DarkMoonEngine::DrawLine(Vector2D startPosition, Vector2D endPosition, Color color, int width, Shader* shader){
    auto line = Line(startPosition, endPosition, color, width, m_activeWindow, shader != nullptr ? shader : m_shaders["basic2D"]);
    line.Draw();
    line.Delete();
}

// Triangle //

void DarkMoonEngine::DrawTriangle(Vector2D vertexA, Vector2D vertexB, Vector2D vertexC, Color color, Shader* shader){
    auto triangle = Triangle(vertexA, vertexB, vertexC, color, m_activeWindow, shader != nullptr ? shader : m_shaders["basic2D"]);
    triangle.Draw();
    triangle.Delete();
}

void DarkMoonEngine::DrawTriangleLines(Vector2D vertexA, Vector2D vertexB, Vector2D vertexC, Color color, int width, Shader* shader){
    auto triangle_lines = TriangleLines(vertexA, vertexB, vertexC, color, width, m_activeWindow, shader != nullptr ? shader : m_shaders["basic2D"]);
    triangle_lines.Draw();
    triangle_lines.Delete();
}

// Rectangle //

void DarkMoonEngine::DrawRectangle(Vector2D vertexA, Vector2D vertexB, Vector2D vertexC, Vector2D vertexD, Color color, Shader* shader){
    auto rectangle = Rectangle(vertexA, vertexB, vertexC, vertexD, color, m_activeWindow, shader != nullptr ? shader : m_shaders["basic2D"]);
    rectangle.Draw();
    rectangle.Delete();
}

void DarkMoonEngine::DrawRectangleLines(Vector2D vertexA, Vector2D vertexB, Vector2D vertexC, Vector2D vertexD, Color color, int width, Shader* shader){
    auto rectangle_lines = RectangleLines(vertexA, vertexB, vertexC, vertexD, color, width, m_activeWindow, shader != nullptr ? shader : m_shaders["basic2D"]);
    rectangle_lines.Draw();
    rectangle_lines.Delete();
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
#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

enum struct WindowMode{
    Windowed,
    Borderless,
    Fullscreen
};

struct Window{
    Window() {};
    ~Window() { if(window) glfwDestroyWindow(window); };

    GLFWwindow* window {};
    WindowMode mode {};
    int windowedX {}, windowedY {}, windowedWidth {}, windowedHeight {};
};
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

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height) { 
        Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if (win)
            glViewport(0, 0, width, height);
    }

    GLFWwindow* window {};
    WindowMode mode {};
    int windowedX {}, windowedY {}, windowedWidth {}, windowedHeight {};
};
#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "monitor.hpp"

enum struct WindowMode{
    Windowed,
    Borderless,
    Fullscreen
};

struct Vector2D{
    int x, y;
};

struct Window{
    // Create window, load OpenGL functions pointers and configure OpenGL
    Window(int width, int height, const char* title);
    ~Window() { Close(); };

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height) { 
        Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if (win)
            glViewport(0, 0, width, height);
    }

    // --------------- //
    // Basic Functions //
    // --------------- //

    // Destroy the window and close it
    void Close(){ if(window) glfwDestroyWindow(window); };
    // Return whether the window should close
    bool ShouldClose(){ return window && glfwWindowShouldClose(window); };
    // Iconify the window (minimize it)
    void Iconify(){ glfwIconifyWindow(window); };
    // Maximife the window
    void Maximize(){ glfwMaximizeWindow(window); };
    // Restore the window to its previous size and state
    void Restore(){ glfwRestoreWindow(window); };
    // Hide the window from view
    void Hide(){ glfwHideWindow(window); };
    // Show the window
    void Show(){ glfwShowWindow(window); };
    // Focus on the window (bring it to the front)
    void Focus(){ glfwFocusWindow(window); };

    // ------- //
    // Drawing //
    // ------- //

    // Prepares the OpenGL context for rendering by setting up the projection matrix and defining the viewport size
    void BeginDrawing();
    // Swaps the buffers to display the rendered content and processes any pending events
    void EndDrawing();

    // ------- //
    // Setters //
    // ------- //

    // Set the window to fullscreen mode
    void SetFullscreen(Monitor monitor = glfwGetPrimaryMonitor());
    // Set the window to borderless mode
    void SetBorderless(Monitor monitor = glfwGetPrimaryMonitor());
    // Set the window to windowed mode
    void SetWindowed();
    // Set the window mode (Fullscreen, Borderless and Windowed)
    void SetWindowMode(WindowMode mode, Monitor monitor = glfwGetPrimaryMonitor());
    // Set the window size
    void SetSize(int width, int height);
    // Set the window size (vector)
    void SetSize(Vector2D size);
    // Set the window position on the screen
    void SetPosition(int x, int y);
    // Set the window position on the screen (vector)
    void SetPosition(Vector2D position);
    // Set the window title
    void SetTitle(const char* title);
    // Set the window icon from the specified file path
    void SetIcon(const char* iconPath);
    // Set the window opacity (0.0 to 1.0)
    void SetOpacity(float opacity);

    // ------- //
    // Getters //
    // ------- //

    WindowMode GetWindowMode(){ return mode; };
    GLFWwindow* GetWindow(){ return window; };

    int GetWidth();
    int GetHeight();
    Vector2D GetSize();

    int GetPositionX();
    int GetPositionY();
    Vector2D GetPosition();

    float GetOpacity(){ return glfwGetWindowOpacity(window); };

private:
    WindowMode mode {};
    GLFWwindow* window {};
    int windowedX {}, windowedY {}, windowedWidth {}, windowedHeight {};
};
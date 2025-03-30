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

    // --------------- //
    // Basic Functions //
    // --------------- //

    // Destroy the window and close it
    void Close(){ if(m_window) glfwDestroyWindow(m_window); };
    // Return whether the window should close
    bool ShouldClose(){ return m_window && glfwWindowShouldClose(m_window); };
    // Iconify the window (minimize it)
    void Iconify(){ glfwIconifyWindow(m_window); };
    // Maximife the window
    void Maximize(){ glfwMaximizeWindow(m_window); };
    // Restore the window to its previous size and state
    void Restore(){ glfwRestoreWindow(m_window); };
    // Hide the window from view
    void Hide(){ glfwHideWindow(m_window); };
    // Show the window
    void Show(){ 
        SetSize({m_windowedWidth, m_windowedHeight});
        SetPosition({m_windowedX, m_windowedY});
        glfwShowWindow(m_window); 
    };
    // Focus on the window (bring it to the front)
    void Focus(){ glfwFocusWindow(m_window); };

    // ------- //
    // Drawing //
    // ------- //

    // Prepares the OpenGL context for rendering by setting up the projection matrix and defining the viewport size
    void BeginDrawing();
    // Swaps the buffers to display the rendered content and processes any pending events
    void EndDrawing();

    // --------------- //
    // Input Functions //
    // --------------- //

    // Keyboard input
    /*
        2 modes: key events and character events

        glfwSetKeyCallback();
        glfwSetCharCallback();

        glfwGetKey();

        SetExitKey();
    */
    
    // Mouse input
    /*
        glfwSetCursorCallback();
        glfwCreateCursor();
        
        glfwSetCursorEnterCallback();

        glfwSetMouseButtonCallback();
        glfwGetMouseButton();

        glfwGetCursorPos();
            ...

        glfwSetScrollCallback();
    */
    
    // Joystick input
    /*
        glfwSetJoystickCallback();
           
    */

    // Gamepad mapping
    // Time input
    // Clipboard input and output
    // Path drop input

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

    WindowMode GetWindowMode(){ return m_mode; };
    GLFWwindow* GetWindow(){ return m_window; };

    int GetWidth();
    int GetHeight();
    Vector2D GetSize();

    int GetPositionX();
    int GetPositionY();
    Vector2D GetPosition();

    float GetOpacity(){ return glfwGetWindowOpacity(m_window); };

    int GetExitKey(){ return m_exitKey; };

private:
    // --------- //
    // Callbacks //
    // --------- //

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height) { 
        Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
        
        if(win) 
            glViewport(0, 0, width, height);
    }

    static void key_callback(GLFWwindow* window, int key, int, int action, int) {
        Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
        
        if(key == win->GetExitKey() && action == GLFW_RELEASE)
            glfwSetWindowShouldClose(win->GetWindow(), GLFW_TRUE);
    }

    // ---------- //
    // Properties //
    // ---------- //

    WindowMode m_mode {};
    GLFWwindow* m_window {};
    int m_windowedX {}, m_windowedY {}, m_windowedWidth {}, m_windowedHeight {};
    int m_exitKey { GLFW_KEY_ESCAPE };
};
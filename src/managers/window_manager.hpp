#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct WindowManager {
public:
    static WindowManager& getInstance() {
        static WindowManager instance;
        return instance;
    }

    ~WindowManager() { closeWindow(); };

    bool initWindow(int width, int height, const char* title);
    void closeWindow();
    bool windowShouldClose();

    void beginDrawing();
    void endDrawing();

    // SET WINDOW ICON

private:
    WindowManager() = default;

    GLFWwindow* m_window{};
    int m_width{}, m_height{};
};
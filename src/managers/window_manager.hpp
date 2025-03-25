#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../utils/color.hpp"

enum struct WindowMode {
    Windowed,
    Fullscreen,
    Borderless
};

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

    void clearBackground(Color color);

    void setWindowMode(WindowMode windowMode);

    void setWindowSize(int width, int height);
    void setWindowPosition(int x, int y);

    // Getters //

    WindowMode getWindowMode(){ return m_currentMode; };
    
    // void getWindowSize();
    // void getResolution();
    // void getWindowPosition();

    // set Monitor

    // SET WINDOW ICON

private:
    WindowManager() = default;

    bool initGLFW();
    bool createWindow(int width, int height, const char* title);
    bool loadGLAD();

    GLFWwindow* m_window{};
    const GLFWvidmode* m_mode;
    WindowMode m_currentMode;

    int m_width{}, m_height{};

    int windowedX{}, windowedY{}, windowedWidth{}, windowedHeight{};
};
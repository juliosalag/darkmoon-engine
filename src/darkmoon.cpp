#include "darkmoon.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"

#define STB_IMAGE_IMPLEMENTATION
#include "../libs/stb_image.h"

#pragma GCC diagnostic pop

// Window //

bool DarkMoonEngine::InitWindow(int width, int height, const char* title){
    // ----------------------------- //
    // Initialize and configure GLFW //
    // ----------------------------- //

    if(!glfwInit()){
        std::cerr << "[ERROR] Failed to initialize GLFW\n";

        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    std::cout << "[OK] GLFW initialization successful\n";

    // ------------------ // 
    // Create window GLFW //
    // ------------------ // 

    m_activeWindow.window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if(!m_activeWindow.window){
        std::cerr << "[ERROR] Failed to create GLFW window\n";
        glfwTerminate();

        return false;
    } 
    glfwMakeContextCurrent(m_activeWindow.window);
    glfwSetFramebufferSizeCallback(m_activeWindow.window, m_activeWindow.framebuffer_size_callback);

    glfwGetWindowPos(m_activeWindow.window, &m_activeWindow.windowedX, &m_activeWindow.windowedY);
    glfwGetWindowSize(m_activeWindow.window, &m_activeWindow.windowedWidth, &m_activeWindow.windowedHeight);

    std::cout << "[OK] GLFW window created successfully\n";

    // ------------------------------ //
    // Load OpenGL functions pointers //
    // ------------------------------ //

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "[ERROR] Failed to initialize GLAD\n";
        if(m_activeWindow.window)
            glfwDestroyWindow(m_activeWindow.window);
        glfwTerminate();

        return false;
    }
    std::cout << "[OK] GLAD initialization successful\n";

    // ---------------- //
    // Configure OpenGL //
    // ---------------- //

    glViewport(0, 0, width, height);
    m_activeMonitor.monitor = glfwGetPrimaryMonitor();
    m_activeMonitor.name = glfwGetMonitorName(m_activeMonitor.monitor);

    return true;
}

void DarkMoonEngine::CloseWindow(){
    if(m_activeWindow.window)
        glfwDestroyWindow(m_activeWindow.window);
}

bool DarkMoonEngine::WindowShouldClose(){
    return m_activeWindow.window && glfwWindowShouldClose(m_activeWindow.window);
}

void DarkMoonEngine::SetFullscreen(){
    const GLFWvidmode* mode = glfwGetVideoMode(m_activeMonitor.monitor);

    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwSetWindowMonitor(m_activeWindow.window, m_activeMonitor.monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    
    m_activeWindow.mode = WindowMode::Fullscreen;
}

void DarkMoonEngine::SetBorderless(){
    const GLFWvidmode* mode = glfwGetVideoMode(m_activeMonitor.monitor);

    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_FALSE);
    glfwSetWindowMonitor(m_activeWindow.window, m_activeMonitor.monitor, 0, 0, mode->width, mode->height, GLFW_DONT_CARE);

    m_activeWindow.mode = WindowMode::Borderless;
}

void DarkMoonEngine::SetWindowed(){
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwSetWindowMonitor(m_activeWindow.window, NULL, m_activeWindow.windowedX, m_activeWindow.windowedY, m_activeWindow.windowedWidth, m_activeWindow.windowedHeight, 0);

    m_activeWindow.mode = WindowMode::Windowed;
}

void DarkMoonEngine::SetWindowMode(WindowMode mode){
    switch (mode){
    case WindowMode::Borderless :
        SetBorderless();
        break;
    case WindowMode::Fullscreen :
        SetFullscreen();
        break;
    case WindowMode::Windowed :
        SetWindowed();
        break;

    default:
        break;
    }
}

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

void DarkMoonEngine::SetWindowSize(int width, int height){
    if(m_activeWindow.mode == WindowMode::Windowed)
        glfwSetWindowSize(m_activeWindow.window, width, height);
}

int DarkMoonEngine::GetWindowWidth(){
    int width, height;
    glfwGetWindowSize(m_activeWindow.window, &width, &height);

    return width;
}

int DarkMoonEngine::GetWindowHeight(){
    int width, height;
    glfwGetWindowSize(m_activeWindow.window, &width, &height);

    return height;
}

// --- //

void DarkMoonEngine::SetWindowPosition(int x, int y){
    if(m_activeWindow.mode == WindowMode::Windowed)
        glfwSetWindowPos(m_activeWindow.window, x, y);    
}

int DarkMoonEngine::GetWindowPositionX(){
    int xpos, ypos;
    glfwGetWindowPos(m_activeWindow.window, &xpos, &ypos);

    return xpos;
}

int DarkMoonEngine::GetWindowPositionY(){
    int xpos, ypos;
    glfwGetWindowPos(m_activeWindow.window, &xpos, &ypos);

    return ypos;
}

// --- //

void DarkMoonEngine::SetWindowTitle(const char* title){
    glfwSetWindowTitle(m_activeWindow.window, title);
}

/*
const char* DarkMoonEngine::GetWindowTitle(){
    return glfwGetWindowTitle(m_activeWindow.window);
}
*/

void DarkMoonEngine::SetWindowIcon(const char* iconPath){
    GLFWimage images[1];
    images[0].pixels = stbi_load(iconPath, &images[0].width, &images[0].height, 0, 4);

    glfwSetWindowIcon(m_activeWindow.window, 1, images),

    stbi_image_free(images[0].pixels);    
}

void DarkMoonEngine::IconifyWindow(){
    glfwIconifyWindow(m_activeWindow.window);
}

void DarkMoonEngine::MaximizeWindow(){
    glfwMaximizeWindow(m_activeWindow.window);
}

void DarkMoonEngine::RestoreWindow(){
    glfwRestoreWindow(m_activeWindow.window);
}

void DarkMoonEngine::HideWindow(){
    glfwHideWindow(m_activeWindow.window);
}

void DarkMoonEngine::ShowWindow(){
    glfwShowWindow(m_activeWindow.window);
}

void DarkMoonEngine::FocusWindow(){
    glfwFocusWindow(m_activeWindow.window);
}

void DarkMoonEngine::SetWindowOpacity(float opacity){
    if(opacity >= 0.0f && opacity <= 1.0f)
        glfwSetWindowOpacity(m_activeWindow.window, opacity);
}

float DarkMoonEngine::GetWindowOpacity(){
    return glfwGetWindowOpacity(m_activeWindow.window);
}


// Render System //

void DarkMoonEngine::BeginDrawing(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    int width, height;
    glfwGetWindowSize(m_activeWindow.window, &width, &height);

    glOrtho(0, width, 0, height, -1, 1);
}

void DarkMoonEngine::EndDrawing(){
    glfwSwapBuffers(m_activeWindow.window);
    glfwPollEvents();
}

void DarkMoonEngine::ClearBackground(Color color){
    glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
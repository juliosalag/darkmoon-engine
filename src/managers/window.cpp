#include "window.hpp"
#include <iostream>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"

#define STB_IMAGE_IMPLEMENTATION
#include "./../libs/stb_image.h"

#pragma GCC diagnostic pop

Window::Window(int width, int height, const char* title){
    // ------------------ // 
    // Create window GLFW //
    // ------------------ // 

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if(!window){
        std::cerr << "[ERROR] Failed to create GLFW window\n";
        glfwTerminate();

        // return false;
    } 
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwGetWindowPos(window, &windowedX, &windowedY);
    glfwGetWindowSize(window, &windowedWidth, &windowedHeight);

    std::cout << "[OK] GLFW window created successfully\n";

    // ------------------------------ //
    // Load OpenGL functions pointers //
    // ------------------------------ //

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "[ERROR] Failed to initialize GLAD\n";
        if(window)
            glfwDestroyWindow(window);
        glfwTerminate();

        //return false;
    }
    
    std::cout << "[OK] GLAD initialization successful\n";

    glViewport(0, 0, width, height);
}

// ------- //
// Drawing //
// ------- //

void Window::BeginDrawing(){
    glfwMakeContextCurrent(window);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    glOrtho(0, width, 0, height, -1, 1);
}

void Window::EndDrawing(){
    glfwSwapBuffers(window);
    glfwPollEvents();
}

// ------- //
// Setters //
// ------- //

void Window::SetFullscreen(Monitor monitor){
    const GLFWvidmode* mode = glfwGetVideoMode(monitor.monitor);

    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwSetWindowMonitor(window, monitor.monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    
    this->mode = WindowMode::Fullscreen;
}

void Window::SetBorderless(Monitor monitor){
    const GLFWvidmode* mode = glfwGetVideoMode(monitor.monitor);

    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_FALSE);
    glfwSetWindowMonitor(window, monitor.monitor, 0, 0, mode->width, mode->height, GLFW_DONT_CARE);

    this->mode = WindowMode::Borderless;
}

void Window::SetWindowed(){
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwSetWindowMonitor(window, NULL, windowedX, windowedY, windowedWidth, windowedHeight, 0);

    mode = WindowMode::Windowed;
}

void Window::SetWindowMode(WindowMode mode, Monitor monitor){
    switch (mode){
        case WindowMode::Borderless :
            SetBorderless(monitor);
            break;
        case WindowMode::Fullscreen :
            SetFullscreen(monitor);
            break;
        case WindowMode::Windowed :
            SetWindowed();
            break;

        default:
            break;
    }
}

void Window::SetSize(int width, int height){
    if(mode == WindowMode::Windowed)
        glfwSetWindowSize(window, width, height);
}

void Window::SetSize(Vector2D size){
    SetSize(size.x, size.y);
}

void Window::SetPosition(int x, int y){
    if(mode == WindowMode::Windowed)
        glfwSetWindowPos(window, x, y);  
}

void Window::SetPosition(Vector2D position){
    SetPosition(position.x, position.y);
}

void Window::SetTitle(const char* title){
    glfwSetWindowTitle(window, title);
}

void Window::SetIcon(const char* iconPath){
    GLFWimage images[1];
    images[0].pixels = stbi_load(iconPath, &images[0].width, &images[0].height, 0, 4);

    glfwSetWindowIcon(window, 1, images),

    stbi_image_free(images[0].pixels); 
}

void Window::SetOpacity(float opacity){
    if(opacity >= 0.0f && opacity <= 1.0f)
        glfwSetWindowOpacity(window, opacity);
}

// ------- //
// Getters //
// ------- //

int Window::GetWidth(){
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    
    return width;
}

int Window::GetHeight(){
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    
    return height;
}

Vector2D Window::GetSize(){
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    
    return Vector2D{width, height};
}

int Window::GetPositionX(){
    int xpos, ypos;
    glfwGetWindowPos(window, &xpos, &ypos);

    return xpos;
}

int Window::GetPositionY(){
    int xpos, ypos;
    glfwGetWindowPos(window, &xpos, &ypos);

    return ypos;
}

Vector2D Window::GetPosition(){
    int xpos, ypos;
    glfwGetWindowPos(window, &xpos, &ypos);

    return Vector2D{xpos, ypos};
}
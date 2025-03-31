#include "window.hpp"

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

    m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if(!m_window){
        std::cerr << "[ERROR] Failed to create GLFW window\n";
        glfwTerminate();

        // return false;
    } 
    glfwMakeContextCurrent(m_window);
    glfwSetWindowUserPointer(m_window, this);

    // Callbacks
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
    glfwSetKeyCallback(m_window, key_callback);
    glfwSetCharCallback(m_window, char_callback);

    glfwGetWindowPos(m_window, &m_windowedX, &m_windowedY);
    glfwGetWindowSize(m_window, &m_windowedWidth, &m_windowedHeight);

    std::cout << "[OK] GLFW window created successfully\n";

    // ------------------------------ //
    // Load OpenGL functions pointers //
    // ------------------------------ //

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "[ERROR] Failed to initialize GLAD\n";
        if(m_window)
            glfwDestroyWindow(m_window);
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
    glfwMakeContextCurrent(m_window);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    int width, height;
    glfwGetWindowSize(m_window, &width, &height);
    glOrtho(0, width, 0, height, -1, 1);
}

void Window::EndDrawing(){
    UpdateInput();

    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

// ------- //
// Setters //
// ------- //

void Window::SetFullscreen(Monitor monitor){

    if(m_mode == WindowMode::Windowed){
        glfwGetWindowPos(m_window, &m_windowedX, &m_windowedY);
        glfwGetWindowSize(m_window, &m_windowedWidth, &m_windowedHeight);
    }

    const GLFWvidmode* mode = glfwGetVideoMode(monitor.monitor);

    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwSetWindowMonitor(m_window, monitor.monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    
    m_mode = WindowMode::Fullscreen;
}

void Window::SetBorderless(Monitor monitor){

    if(m_mode == WindowMode::Windowed){
        glfwGetWindowPos(m_window, &m_windowedX, &m_windowedY);
        glfwGetWindowSize(m_window, &m_windowedWidth, &m_windowedHeight);
    }

    const GLFWvidmode* mode = glfwGetVideoMode(monitor.monitor);

    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_FALSE);
    glfwSetWindowMonitor(m_window, monitor.monitor, 0, 0, mode->width, mode->height, GLFW_DONT_CARE);

    m_mode = WindowMode::Borderless;
}

void Window::SetWindowed(){
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwSetWindowMonitor(m_window, NULL, m_windowedX, m_windowedY, m_windowedWidth, m_windowedHeight, 0);

    m_mode = WindowMode::Windowed;
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
    if(m_mode == WindowMode::Windowed){
        m_windowedWidth = width;
        m_windowedHeight = height;
        glfwSetWindowSize(m_window, width, height);
    }
}

void Window::SetSize(Vector2D size){
    SetSize(size.x, size.y);
}

void Window::SetPosition(int x, int y){
    if(m_mode == WindowMode::Windowed){
        m_windowedX = x;
        m_windowedY = y;
        glfwSetWindowPos(m_window, x, y);  
    }
}

void Window::SetPosition(Vector2D position){
    SetPosition(position.x, position.y);
}

void Window::SetTitle(const char* title){
    glfwSetWindowTitle(m_window, title);
}

void Window::SetIcon(const char* iconPath){
    GLFWimage images[1];
    images[0].pixels = stbi_load(iconPath, &images[0].width, &images[0].height, 0, 4);

    glfwSetWindowIcon(m_window, 1, images),

    stbi_image_free(images[0].pixels); 
}

void Window::SetOpacity(float opacity){
    if(opacity >= 0.0f && opacity <= 1.0f)
        glfwSetWindowOpacity(m_window, opacity);
}

// ------- //
// Getters //
// ------- //

int Window::GetWidth(){
    int width, height;
    glfwGetWindowSize(m_window, &width, &height);
    
    return width;
}

int Window::GetHeight(){
    int width, height;
    glfwGetWindowSize(m_window, &width, &height);
    
    return height;
}

Vector2D Window::GetSize(){
    int width, height;
    glfwGetWindowSize(m_window, &width, &height);
    
    return Vector2D{width, height};
}

int Window::GetPositionX(){
    int xpos, ypos;
    glfwGetWindowPos(m_window, &xpos, &ypos);

    return xpos;
}

int Window::GetPositionY(){
    int xpos, ypos;
    glfwGetWindowPos(m_window, &xpos, &ypos);

    return ypos;
}

Vector2D Window::GetPosition(){
    int xpos, ypos;
    glfwGetWindowPos(m_window, &xpos, &ypos);

    return Vector2D{xpos, ypos};
}
#include "darkmoon.hpp"

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
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwSetWindowMonitor(m_activeWindow.window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
    
    m_activeWindow.mode = WindowMode::Fullscreen;
}

void DarkMoonEngine::SetBorderless(){
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_FALSE);
    glfwSetWindowMonitor(m_activeWindow.window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, GLFW_DONT_CARE);

    m_activeWindow.mode = WindowMode::Borderless;
}

void DarkMoonEngine::SetWindowed(){
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwSetWindowMonitor(m_activeWindow.window, NULL, m_activeWindow.windowedX, m_activeWindow.windowedY, m_activeWindow.windowedWidth, m_activeWindow.windowedHeight, 0);

    m_activeWindow.mode = WindowMode::Windowed;
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
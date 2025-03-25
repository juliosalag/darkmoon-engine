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

    m_activeWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if(!m_activeWindow){
        std::cerr << "[ERROR] Failed to create GLFW window\n";
        glfwTerminate();

        return false;
    } 
    glfwMakeContextCurrent(m_activeWindow);

    std::cout << "[OK] GLFW window created successfully\n";

    // ------------------------------ //
    // Load OpenGL functions pointers //
    // ------------------------------ //

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "[ERROR] Failed to initialize GLAD\n";
        if(m_activeWindow)
            glfwDestroyWindow(m_activeWindow);
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
    if(m_activeWindow)
        glfwDestroyWindow(m_activeWindow);
}

bool DarkMoonEngine::WindowShouldClose(){
    return m_activeWindow && glfwWindowShouldClose(m_activeWindow);
}

// Render System //

void DarkMoonEngine::BeginDrawing(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    int width, height;
    glfwGetWindowSize(m_activeWindow, &width, &height);

    glOrtho(0, width, 0, height, -1, 1);
}

void DarkMoonEngine::EndDrawing(){
    glfwSwapBuffers(m_activeWindow);
    glfwPollEvents();
}

void DarkMoonEngine::ClearBackground(Color color){
    glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
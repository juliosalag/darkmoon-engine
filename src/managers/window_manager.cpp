#include "window_manager.hpp"

bool WindowManager::initWindow(int width, int height, const char* title) {
    this->m_width = width;
    this->m_height = height;

    // glfw: initialize and configure
    // -----------------------------
    if(!glfwInit()){
        std::cerr << "[ERROR] Failed to initialize GLFW\n";

        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw: window creation
    // ---------------------
    m_window = glfwCreateWindow(m_width, m_height, title, nullptr, nullptr); // glfwGetPrimaryMonitor()
    if(!m_window){
        std::cerr << "[ERROR] Failed to create GLFW window\n";
        glfwTerminate();

        return false;
    } 

    glfwMakeContextCurrent(m_window);

    // glad: load OpenGL functions pointers
    // ------------------------------------
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "[ERROR] Failed to initialize GLAD\n";
        glfwDestroyWindow(m_window);
        glfwTerminate();

        return false;
    }

    // Configure OpenGL
    glViewport(0, 0, m_width, m_height);

    return true;
}

void WindowManager::closeWindow() {
    if(m_window)
        glfwDestroyWindow(m_window);

    glfwTerminate();
}

bool WindowManager::windowShouldClose() {
    return m_window && glfwWindowShouldClose(m_window);
}

void WindowManager::beginDrawing() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, m_width, 0, m_height, -1, 1);
}

void WindowManager::endDrawing() {
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}
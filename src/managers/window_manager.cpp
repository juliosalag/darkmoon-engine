#include "window_manager.hpp"

bool WindowManager::initWindow(int width, int height, const char* title) {
    
    if(!initGLFW() || !createWindow(width, height, title) || !loadGLAD())
        return false;    

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

void WindowManager::clearBackground(Color color){
    glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void WindowManager::setWindowMode(WindowMode windowMode){
    /*
    switch (windowMode){
        case WindowMode::Windowed:
            glfwSetWindowMonitor(m_window, NULL, )
            break;

        case WindowMode::Fullscreen:
            
            break;

        case WindowMode::Borderless:
            
            break;
    
        default:
            break;
    }
    */
}

void WindowManager::setWindowSize(int width, int height){

}

void WindowManager::setWindowPosition(int x, int y){

}

// ------------- //
//    PRIVATE    // 
// ------------- //

// Initialize and configure GLFW
bool WindowManager::initGLFW(){
    if(!glfwInit()){
        std::cerr << "[ERROR] Failed to initialize GLFW\n";

        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    std::cout << "[OK] GLFW initialization successful\n";

    return true;
}

// Create window GLFW
bool WindowManager::createWindow(int width, int height, const char* title){
    m_width = width;
    m_height = height;
    m_currentMode = WindowMode::Windowed;

    m_window = glfwCreateWindow(m_width, m_height, title, nullptr, nullptr); // glfwGetPrimaryMonitor()
    if(!m_window){
        std::cerr << "[ERROR] Failed to create GLFW window\n";
        glfwTerminate();

        return false;
    } 

    glfwMakeContextCurrent(m_window);

    //glfwGetWindowPos(m_window, )

    std::cout << "[OK] GLFW window created successfully\n";

    return true;
}

// Load OpenGL functions pointers
bool WindowManager::loadGLAD(){
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "[ERROR] Failed to initialize GLAD\n";
        if(m_window)
            glfwDestroyWindow(m_window);
        glfwTerminate();

        return false;
    }

    std::cout << "[OK] GLAD initialization successful\n";

    return true;
}
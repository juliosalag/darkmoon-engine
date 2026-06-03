#include <darkmoon/engine.hpp>
#include <glad/glad.h>

DarkMoonEngine::DarkMoonEngine(){
    // ----------------------------- //
    // Initialize and configure GLFW //
    // ----------------------------- //

    std::cout << "---------------------------------\n";
    std::cout << "| DarkMoon Engine - Version 2.0 |\n";
    std::cout << "---------------------------------\n";

    if(!glfwInit()){
        std::cerr << "[ERROR] Failed to initialize GLFW\n";

        // throw std::runtime_error("Error en el constructor");
    }
    
    std::cout << "[OK] GLFW initialization successful\n";

    Monitor::initCallback();
}
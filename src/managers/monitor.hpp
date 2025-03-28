#pragma once

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct Monitor{
    Monitor(GLFWmonitor* mon) {
        monitor = mon;
        name = glfwGetMonitorName(monitor);
    };
    Monitor() {};
    ~Monitor() {};
    
    GLFWmonitor* monitor;
    std::string name;
};

#include "darkmoon.hpp"
#include "managers/window.hpp"

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
}

// ---------------- //
// Window Functions //
// ---------------- //

Window& DarkMoonEngine::CreateInitWindow(int width, int height, const char* title){
    if(!m_initWindow){
        m_initWindow.emplace(width, height, title);
        LoadBasicShaders();
    }

    return *m_initWindow;
}

Window& DarkMoonEngine::CreateSharedWindow(int width, int height, const char* title){
    m_sharedWindows.push_back(std::make_unique<Window>(
        width, height, title, GetInitWindow().GetWindow()
    ));
    return *m_sharedWindows.back();
}

// ---------------- //
// Render Functions //
// ---------------- //

void DarkMoonEngine::BeginDrawing(Window& window){
    window.BeginDrawing();
    m_activeWindow = &window;
}

void DarkMoonEngine::BeginDrawing(Window& window, Color color){
    window.BeginDrawing(color);
    m_activeWindow = &window;
}

void DarkMoonEngine::EndDrawing(Window& window){
    window.EndDrawing();
}

void DarkMoonEngine::ClearBackground(Window& window, Color color){
    window.ClearBackground(color);
}

void DarkMoonEngine::DrawPixel(Vector2D /*position*/, Color /*color*/, ResourceShader* /*shader*/){
    /*
    auto activeShader = shader;
    
    if(!shader){
        activeShader = m_shaders["basic2D"];
    }
    */
}

void DarkMoonEngine::DrawLine(Vector2D startPosition, Vector2D endPosition, Color color, ResourceShader* shader){
    auto activeShader = shader;
    
    if(!shader){
        activeShader = m_shaders["basic2D"];
    }

    // Construct Line

    GLuint VAO, VBO;
    
    float vertices[] = {
        normalizeX(static_cast<float>(startPosition.x)), 
        normalizeY(static_cast<float>(startPosition.y)), 
        normalizeX(static_cast<float>(endPosition.x)), 
        normalizeY(static_cast<float>(endPosition.y))
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // VAO

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // Set uniform color in the shader
    GLint colorUniform = glGetUniformLocation(activeShader->getIDShader(), "customColor");
    glUseProgram(activeShader->getIDShader());
    glUniform4fv(colorUniform, 1, glm::value_ptr(glm::vec4(color.r, color.g, color.b, color.a)));

    // Set line width
    glLineWidth(2);

    // Draw the line
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);

    // Reset line width
    glLineWidth(1);

    // Clean up resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

// -------------------------- //
// Resource Manager Functions //
// -------------------------- //

void DarkMoonEngine::LoadBasicShaders(){

    // Basic Shader 2D //

    const std::string basicVertex = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;

        void main(){
            gl_Position = vec4(aPos, 1.0);
        }
    )";

    const std::string basicFragment = R"(
        #version 330 core
        out vec4 FragColor;
        uniform vec4 customColor;

        void main(){
            FragColor = customColor;
        }
    )";

    m_shaders["basic2D"] = CreateShader(basicVertex, basicFragment);
    

}

/*
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
*/
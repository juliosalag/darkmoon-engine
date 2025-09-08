#include <darkmoon.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

void DrawLine(float vertices[], Color color, ResourceShader* shader){
    
    // Construct Line

    GLuint VAO, VBO;

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
    GLint colorUniform = glGetUniformLocation(shader->getIDShader(), "customColor");
    glUseProgram(shader->getIDShader());
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

int main() {
    DarkMoonEngine dm;

    Window w = Window(600, 450, "Game");
    w.SetPosition(10, 10);

    auto shader = dm.CreateShader("assets/shaders/basicshader.vs", "assets/shaders/basicshader.fs");
    
    Window w2 = Window(300, 250, "Debug", w.GetWindow());
    w2.SetPosition(10, 460);

    // Shader basico para las cosas 2D, con color basico y transformaciones
    // Posibilidad de pasar shader personalizado por parametro, si no usara uno por defecto

    float vertices[] = {0.5, -0.5, 0.5, 0.5};
    float vertices2[] = {1, 0, 0.5, 0.5};
    float vertices3[] = {0, -0.7, 0, 0.5};
    float vertices4[] = {0, 1, 0.9, 0.5};
    float vertices5[] = {-1, 1, 0, 0};

    while(!w.ShouldClose()){

        // Logic //

        // Render //

        w.BeginDrawing();
        dm.ClearBackground(GRAY); // TODO: Change to window //

        DrawLine(vertices, RED, shader);
        DrawLine(vertices5, BLACK, shader);
        
        w.EndDrawing();
        
        w2.BeginDrawing();
        dm.ClearBackground(BLACK);

        DrawLine(vertices2, GREEN, shader);
        DrawLine(vertices3, BLUE, shader);
        DrawLine(vertices4, WHITE, shader);

        w2.EndDrawing();

    }

    return 0;
}
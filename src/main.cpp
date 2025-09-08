#include <darkmoon.hpp>

#include <glm/glm.hpp>

int main() {
    DarkMoonEngine dm;

    Window w = Window(400, 300, "Game");
    w.SetPosition(10, 10);

    /*
    // Shader //



    // Line //

    // Construct Line

    GLuint VAO, VBO, EBO;
    glm::vec2 startPos, endPos;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // VAO

    float vertices[] = {startPos.x, startPos.y, endPos.x, endPos.y};

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    */

    while(!w.ShouldClose()){

        // Logic //

        // Render //

        w.BeginDrawing();
        dm.ClearBackground(GRAY); // Change to window

        /*
        // Draw Line



        // Reset line width
        glLineWidth(1);
        */

        w.EndDrawing();
    }

    return 0;
}
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../utils/math.hpp"
#include "../utils/color.hpp"
#include "../resources/resource_shader.hpp"

struct Pixel {
private:
    GLuint VAO {}, VBO {};
    Vector2D position {};
    Color color { BLACK };
    int size { 1 };
    ResourceShader* shader {};
    int windowWidth {}, windowHeight {};

public:
    Pixel(Vector2D pos, Color col, int s, int width, int height, ResourceShader* sha)
        : position(pos), color(col), size(s), shader(sha), windowWidth(width), windowHeight(height)
    {
        float vertex[] = {
            (position.x / static_cast<float>(windowWidth)) * 2 - 1,
            -((position.y / static_cast<float>(windowHeight)) * 2 - 1)
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
    }

    ~Pixel() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    void Draw() {
        glUseProgram(shader->getIDShader());

        GLint colorUniform = glGetUniformLocation(shader->getIDShader(), "customColor");
        glUniform4fv(colorUniform, 1, glm::value_ptr(glm::vec4(color.r, color.g, color.b, color.a)));

        glPointSize(static_cast<GLfloat>(size));
        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, 1);
        glBindVertexArray(0);
        glPointSize(1);
    }

    void SetPosition(Vector2D pos) {
        position = pos;
        float vertex[] = {
            (position.x / static_cast<float>(windowWidth)) * 2 - 1,
            -((position.y / static_cast<float>(windowHeight)) * 2 - 1)
        };

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex), vertex);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void SetColor(Color col) { color = col; }
    void SetSize(int s) { size = s; }
};


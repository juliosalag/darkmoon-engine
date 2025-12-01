#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../utils/math.hpp"
#include "../utils/color.hpp"
#include "../resources/resource_shader.hpp"

struct Pixel {
private:
    GLuint m_VAO {}, m_VBO {};
    Vector2D m_position {};
    Color m_color { BLACK };
    int m_size { 1 };
    ResourceShader* m_shader {};
    int m_windowWidth {}, m_windowHeight {};

public:
    Pixel(Vector2D position, Color color, int size, int windowWidth, int windowHeight, ResourceShader* shader)
        : m_position(position), m_color(color), m_size(size), m_shader(shader), m_windowWidth(windowWidth), m_windowHeight(windowHeight)
    {
        float vertex[] = {
            (static_cast<float>(m_position.x) / static_cast<float>(m_windowWidth)) * 2 - 1,
            -((static_cast<float>(m_position.y) / static_cast<float>(m_windowHeight)) * 2 - 1)
        };

        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
    }

    ~Pixel() { Delete(); }

    void Delete() {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
    }

    void Draw() {
        glUseProgram(m_shader->getIDShader());

        GLint colorUniform = glGetUniformLocation(m_shader->getIDShader(), "customColor");
        glUniform4fv(colorUniform, 1, glm::value_ptr(glm::vec4(m_color.r, m_color.g, m_color.b, m_color.a)));

        glPointSize(static_cast<GLfloat>(m_size));
        glBindVertexArray(m_VAO);
        glDrawArrays(GL_POINTS, 0, 1);
        glBindVertexArray(0);

        glPointSize(1);
    }

    // ------- //
    // Setters //
    // ------- //

    void SetPosition(Vector2D position, Vector2D windowSize) {
        m_windowWidth = windowSize.x;
        m_windowHeight = windowSize.y;
        m_position = position;

        float vertex[] = {
            (static_cast<float>(m_position.x) / static_cast<float>(m_windowWidth)) * 2 - 1,
            -((static_cast<float>(m_position.y) / static_cast<float>(m_windowHeight)) * 2 - 1)
        };

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex), vertex);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void SetColor(Color color) { m_color = color; }
    void SetSize(int size) { m_size = size; }

    // ------- //
    // Getters //
    // ------- //

    Vector2D GetPosition() { return m_position; }
    Color GetColor() { return m_color; }
    int GetSize() { return m_size; }
};


#include <darkmoon/2D/basic/pixel.hpp>

Pixel::Pixel(Vector2Df position, Color color, int size, Window* window, Shader* shader)
    : m_position(position), m_color(color), m_size(size), m_shader(shader == nullptr ? window->GetBasicShader2D() : shader), m_window(window)
{
    float vertex[] = {
        (m_position.x / static_cast<float>(m_window->GetWidth()))  * 2.0f - 1.0f,
        -((m_position.y / static_cast<float>(m_window->GetHeight())) * 2.0f - 1.0f)
    };

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

Pixel::~Pixel() { Delete(); }

void Pixel::Delete() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
}

void Pixel::Draw() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(m_shader->getIDShader());

    GLint colorUniform = glGetUniformLocation(m_shader->getIDShader(), "customColor");
    glUniform4fv(colorUniform, 1, glm::value_ptr(glm::vec4(m_color.r/255.f, m_color.g/255.f, m_color.b/255.f, m_color.a/255.f)));

    glPointSize(static_cast<GLfloat>(m_size));
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_POINTS, 0, 1);
    glBindVertexArray(0);

    glPointSize(1);
    glDisable(GL_BLEND);
}

void Pixel::SetPosition(Vector2Df position) {
    m_position = position;

    float vertex[] = {
        (m_position.x / static_cast<float>(m_window->GetWidth()))  * 2.0f - 1.0f,
        -((m_position.y / static_cast<float>(m_window->GetHeight())) * 2.0f - 1.0f)
    };

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex), vertex);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
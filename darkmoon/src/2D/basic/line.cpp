#include <darkmoon/2D/basic/line.hpp>

Line::Line(Vector2D startPosition, Vector2D endPosition, Color color, int width, Window* window, Shader* shader)
    : m_startPosition(startPosition), m_endPosition(endPosition), m_color(color), m_width(width), m_shader(shader == nullptr ? window->GetBasicShader2D() : shader), m_window(window)
{
    float vertex[] = {
        (static_cast<float>(m_startPosition.x) / static_cast<float>(m_window->GetWidth())) * 2 - 1,
        -((static_cast<float>(m_startPosition.y) / static_cast<float>(m_window->GetHeight())) * 2 - 1),
        (static_cast<float>(m_endPosition.x) / static_cast<float>(m_window->GetWidth())) * 2 - 1,
        -((static_cast<float>(m_endPosition.y) / static_cast<float>(m_window->GetHeight())) * 2 - 1)
    };

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

Line::~Line() { Delete(); }

void Line::Delete() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
}

void Line::Draw() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(m_shader->getIDShader());

    GLint colorUniform = glGetUniformLocation(m_shader->getIDShader(), "customColor");
    glUniform4fv(colorUniform, 1, glm::value_ptr(glm::vec4(m_color.r/255.f, m_color.g/255.f, m_color.b/255.f, m_color.a/255.f)));

    glLineWidth(static_cast<GLfloat>(m_width));
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);

    glLineWidth(1);
    glDisable(GL_BLEND);
}

void Line::SetPosition(Vector2D startPosition, Vector2D endPosition) {
    m_startPosition = startPosition;
    m_endPosition = endPosition;

    float vertex[] = {
        (static_cast<float>(m_startPosition.x) / static_cast<float>(m_window->GetWidth())) * 2 - 1,
        -((static_cast<float>(m_startPosition.y) / static_cast<float>(m_window->GetHeight())) * 2 - 1),
        (static_cast<float>(m_endPosition.x) / static_cast<float>(m_window->GetWidth())) * 2 - 1,
        -((static_cast<float>(m_endPosition.y) / static_cast<float>(m_window->GetHeight())) * 2 - 1)
    };

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex), vertex);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
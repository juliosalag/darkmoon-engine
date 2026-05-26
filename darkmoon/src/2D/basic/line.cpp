#include <darkmoon/2D/basic/line.hpp>

Line::Line(Vector2Df startPosition, Vector2Df endPosition, Color color, int width, Window* window, Shader* shader)
    : m_startPosition(startPosition), m_endPosition(endPosition), m_color(color), m_width(width), m_shader(shader == nullptr ? window->GetBasicShader2D() : shader), m_window(window)
{
    float vertex[] = {
        (m_startPosition.x / static_cast<float>(m_window->GetWidth()))  * 2.0f - 1.0f,
        -((m_startPosition.y / static_cast<float>(m_window->GetHeight())) * 2.0f - 1.0f),
        (m_endPosition.x / static_cast<float>(m_window->GetWidth()))    * 2.0f - 1.0f,
        -((m_endPosition.y / static_cast<float>(m_window->GetHeight())) * 2.0f - 1.0f)
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

void Line::Draw(const Camera2D& camera) {
    Vector2Df screenStart = camera.WorldToScreen(m_startPosition);
    Vector2Df screenEnd   = camera.WorldToScreen(m_endPosition);

    float w = static_cast<float>(m_window->GetWidth());
    float h = static_cast<float>(m_window->GetHeight());

    float vertex[] = {
        (screenStart.x / w) * 2.0f - 1.0f,
        -((screenStart.y / h) * 2.0f - 1.0f),
        (screenEnd.x / w)   * 2.0f - 1.0f,
        -((screenEnd.y / h) * 2.0f - 1.0f)
    };

    // Temporal upload
    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(m_shader->getIDShader());
    GLint colorUniform = glGetUniformLocation(m_shader->getIDShader(), "customColor");
    glUniform4fv(colorUniform, 1, glm::value_ptr(glm::vec4(
        m_color.r / 255.f, m_color.g / 255.f,
        m_color.b / 255.f, m_color.a / 255.f)));

    glLineWidth(static_cast<GLfloat>(m_width));
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);
    glLineWidth(1);
    glDisable(GL_BLEND);

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void Line::SetPosition(Vector2Df startPosition, Vector2Df endPosition) {
    m_startPosition = startPosition;
    m_endPosition   = endPosition;

    float vertex[] = {
        (m_startPosition.x / static_cast<float>(m_window->GetWidth()))  * 2.0f - 1.0f,
        -((m_startPosition.y / static_cast<float>(m_window->GetHeight())) * 2.0f - 1.0f),
        (m_endPosition.x / static_cast<float>(m_window->GetWidth()))    * 2.0f - 1.0f,
        -((m_endPosition.y / static_cast<float>(m_window->GetHeight())) * 2.0f - 1.0f)
    };

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex), vertex);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
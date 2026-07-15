#include <darkmoon/2D/triangle.hpp>

// -------- //
// Triangle //
// -------- //

Triangle::Triangle(Vector2Df vertexA, Vector2Df vertexB, Vector2Df vertexC,
                   Color color, Window* window, Shader* shader)
    : m_vertexA(vertexA), m_vertexB(vertexB), m_vertexC(vertexC),
      m_color(color),
      m_shader(shader == nullptr ? window->GetBasicShader2D() : shader),
      m_window(window)
{
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    Rebuild();
}

void Triangle::Rebuild(){
    float w = static_cast<float>(m_window->GetWidth());
    float h = static_cast<float>(m_window->GetHeight());

    auto [ax, ay] = ToNDC(m_vertexA, w, h);
    auto [bx, by] = ToNDC(m_vertexB, w, h);
    auto [cx, cy] = ToNDC(m_vertexC, w, h);

    float vertex[] = { ax, ay, bx, by, cx, cy };
    GLuint index[] = { 0, 1, 2 };

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// ------- //
// Setters //
// ------- //

void Triangle::SetPosition(Vector2Df position){
    Vector2Df delta = { position.x - m_vertexA.x,
                       position.y - m_vertexA.y };
    m_vertexA = position;
    m_vertexB = { m_vertexB.x + delta.x, m_vertexB.y + delta.y };
    m_vertexC = { m_vertexC.x + delta.x, m_vertexC.y + delta.y };
    Rebuild();
}

void Triangle::SetVertexA(Vector2Df v) { m_vertexA = v; Rebuild(); }
void Triangle::SetVertexB(Vector2Df v) { m_vertexB = v; Rebuild(); }
void Triangle::SetVertexC(Vector2Df v) { m_vertexC = v; Rebuild(); }

// -------- //
// Commands //
// -------- //

void Triangle::Delete(){
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
}

void Triangle::Draw(){
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(m_shader->getIDShader());

    GLint colorUniform = glGetUniformLocation(m_shader->getIDShader(), "customColor");
    glUniform4fv(colorUniform, 1, glm::value_ptr(glm::vec4(
        m_color.r / 255.f, m_color.g / 255.f,
        m_color.b / 255.f, m_color.a / 255.f)));

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glDisable(GL_BLEND);
}

void Triangle::Draw(const Camera2D& camera){
    float w = static_cast<float>(m_window->GetWidth());
    float h = static_cast<float>(m_window->GetHeight());

    auto [ax, ay] = ToNDC(camera.WorldToScreen(m_vertexA), w, h);
    auto [bx, by] = ToNDC(camera.WorldToScreen(m_vertexB), w, h);
    auto [cx, cy] = ToNDC(camera.WorldToScreen(m_vertexC), w, h);

    float vertex[] = { ax, ay, bx, by, cx, cy };
    GLuint index[] = { 0, 1, 2 };

    // Temporal Upload
    GLuint vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(m_shader->getIDShader());

    GLint colorUniform = glGetUniformLocation(m_shader->getIDShader(), "customColor");
    glUniform4fv(colorUniform, 1, glm::value_ptr(glm::vec4(
        m_color.r / 255.f, m_color.g / 255.f,
        m_color.b / 255.f, m_color.a / 255.f)));

    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glDisable(GL_BLEND);

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

// ------------- //
// TriangleLines //
// ------------- //

TriangleLines::TriangleLines(Vector2Df vertexA, Vector2Df vertexB, Vector2Df vertexC, Color color, int width, Window* window, Shader* shader)
    : m_vertexA(vertexA), m_vertexB(vertexB), m_vertexC(vertexC),
      m_color(color), m_lineWidth(width), m_window(window), m_shader(shader),
      m_edgeAB(vertexA, vertexB, color, width, window, shader),
      m_edgeBC(vertexB, vertexC, color, width, window, shader),
      m_edgeCA(vertexC, vertexA, color, width, window, shader)
{}

void TriangleLines::Rebuild(){
    m_edgeAB.Delete();
    m_edgeBC.Delete();
    m_edgeCA.Delete();

    m_edgeAB = Line(m_vertexA, m_vertexB, m_color, m_lineWidth, m_window, m_shader);
    m_edgeBC = Line(m_vertexB, m_vertexC, m_color, m_lineWidth, m_window, m_shader);
    m_edgeCA = Line(m_vertexC, m_vertexA, m_color, m_lineWidth, m_window, m_shader);
}

// ------- //
// Setters //
// ------- //

void TriangleLines::SetPosition(Vector2Df position){
    Vector2Df delta = { position.x - m_vertexA.x,
                       position.y - m_vertexA.y };
    m_vertexA = position;
    m_vertexB = { m_vertexB.x + delta.x, m_vertexB.y + delta.y };
    m_vertexC = { m_vertexC.x + delta.x, m_vertexC.y + delta.y };
    Rebuild();
}

void TriangleLines::SetVertexA(Vector2Df v) { m_vertexA = v; Rebuild(); }
void TriangleLines::SetVertexB(Vector2Df v) { m_vertexB = v; Rebuild(); }
void TriangleLines::SetVertexC(Vector2Df v) { m_vertexC = v; Rebuild(); }

// -------- //
// Commands //
// -------- //

void TriangleLines::Delete(){
    m_edgeAB.Delete();
    m_edgeBC.Delete();
    m_edgeCA.Delete();
}

void TriangleLines::Draw(){
    m_edgeAB.Draw();
    m_edgeBC.Draw();
    m_edgeCA.Draw();
}

void TriangleLines::Draw(const Camera2D& camera){
    m_edgeAB.Draw(camera);
    m_edgeBC.Draw(camera);
    m_edgeCA.Draw(camera);
}
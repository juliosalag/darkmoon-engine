#include <darkmoon/2D/rectangle.hpp>

// ================================================================== //
// Helpers                                                             //
// ================================================================== //

// Recalculates B, C, D from A + current width/height, keeping layout:
//   A --- B
//   |     |
//   C --- D
static void LayoutFromAandSize(Vector2Df a, float w, float h,  Vector2Df& b, Vector2Df& c, Vector2Df& d){
    b = { a.x + w, a.y     };
    c = { a.x,     a.y + h };
    d = { a.x + w, a.y + h };
}

// ================================================================== //
// Rectangle                                                           //
// ================================================================== //

Rectangle::Rectangle(Vector2Df vertexA, Vector2Df vertexB, Vector2Df vertexC, Vector2Df vertexD, Color color, Window* window, Shader* shader)
    : m_vertexA(vertexA), m_vertexB(vertexB), m_vertexC(vertexC), m_vertexD(vertexD),
      m_color(color),
      m_shader(shader == nullptr ? window->GetBasicShader2D() : shader),
      m_window(window)
{
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    Rebuild();
}

Rectangle::Rectangle(Vector2Df position, float width, float height, Color color, Window* window, Shader* shader)
    : Rectangle(
        position,
        { position.x + width, position.y          },
        { position.x,         position.y + height  },
        { position.x + width, position.y + height  },
        color, window, shader)
{}

void Rectangle::Rebuild(){
    float w = static_cast<float>(m_window->GetWidth());
    float h = static_cast<float>(m_window->GetHeight());

    // NDC conversion:  x_ndc = (px / w) * 2 - 1
    //                  y_ndc = -((py / h) * 2 - 1)   (flip Y)
    float vertex[] = {
        (m_vertexA.x / w) * 2.f - 1.f,  -((m_vertexA.y / h) * 2.f - 1.f),
        (m_vertexB.x / w) * 2.f - 1.f,  -((m_vertexB.y / h) * 2.f - 1.f),
        (m_vertexC.x / w) * 2.f - 1.f,  -((m_vertexC.y / h) * 2.f - 1.f),
        (m_vertexD.x / w) * 2.f - 1.f,  -((m_vertexD.y / h) * 2.f - 1.f),
    };
    GLuint index[] = { 0, 1, 2, 1, 2, 3 };

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

void Rectangle::SetPosition(Vector2Df position){
    float w = GetWidth();
    float h = GetHeight();
    m_vertexA = position;
    LayoutFromAandSize(m_vertexA, w, h, m_vertexB, m_vertexC, m_vertexD);
    Rebuild();
}

void Rectangle::SetVertexA(Vector2Df v){ m_vertexA = v; Rebuild(); }
void Rectangle::SetVertexB(Vector2Df v){ m_vertexB = v; Rebuild(); }
void Rectangle::SetVertexC(Vector2Df v){ m_vertexC = v; Rebuild(); }
void Rectangle::SetVertexD(Vector2Df v){ m_vertexD = v; Rebuild(); }

void Rectangle::SetWidth(float width){
    LayoutFromAandSize(m_vertexA, width, GetHeight(), m_vertexB, m_vertexC, m_vertexD);
    Rebuild();
}

void Rectangle::SetHeight(float height){
    LayoutFromAandSize(m_vertexA, GetWidth(), height, m_vertexB, m_vertexC, m_vertexD);
    Rebuild();
}

void Rectangle::SetSize(float width, float height){
    LayoutFromAandSize(m_vertexA, width, height, m_vertexB, m_vertexC, m_vertexD);
    Rebuild();
}

void Rectangle::SetSize(Vector2Df size){ SetSize(size.x, size.y); }

// -------- //
// Commands //
// -------- //

void Rectangle::Delete(){
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
}

void Rectangle::Draw(){
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(m_shader->getIDShader());

    GLint colorUniform = glGetUniformLocation(m_shader->getIDShader(), "customColor");
    glUniform4fv(colorUniform, 1, glm::value_ptr(glm::vec4(
        m_color.r / 255.f, m_color.g / 255.f,
        m_color.b / 255.f, m_color.a / 255.f)));

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glDisable(GL_BLEND);
}

// ================================================================== //
// RectangleLines                                                      //
// ================================================================== //

RectangleLines::RectangleLines(Vector2Df vertexA, Vector2Df vertexB, Vector2Df vertexC, Vector2Df vertexD, Color color, int width, Window* window, Shader* shader)
    : m_vertexA(vertexA), m_vertexB(vertexB), m_vertexC(vertexC), m_vertexD(vertexD),
      m_color(color), m_width(width), m_window(window), m_shader(shader),
      m_edgeAB(vertexA, vertexB, color, width, window, shader),
      m_edgeBD(vertexB, vertexD, color, width, window, shader),
      m_edgeCD(vertexC, vertexD, color, width, window, shader),
      m_edgeCA(vertexC, vertexA, color, width, window, shader)
{}

RectangleLines::RectangleLines(Vector2Df position, float width, float height, Color color, int lineWidth, Window* window, Shader* shader)
    : RectangleLines(
        position,
        { position.x + width, position.y          },
        { position.x,         position.y + height  },
        { position.x + width, position.y + height  },
        color, lineWidth, window, shader)
{}

void RectangleLines::Rebuild(){
    m_edgeAB.Delete();
    m_edgeBD.Delete();
    m_edgeCD.Delete();
    m_edgeCA.Delete();

    m_edgeAB = Line(m_vertexA, m_vertexB, m_color, m_width, m_window, m_shader);
    m_edgeBD = Line(m_vertexB, m_vertexD, m_color, m_width, m_window, m_shader);
    m_edgeCD = Line(m_vertexC, m_vertexD, m_color, m_width, m_window, m_shader);
    m_edgeCA = Line(m_vertexC, m_vertexA, m_color, m_width, m_window, m_shader);
}

// ------- //
// Setters //
// ------- //

void RectangleLines::SetPosition(Vector2Df position){
    float w = GetWidth();
    float h = GetHeight();
    m_vertexA = position;
    LayoutFromAandSize(m_vertexA, w, h, m_vertexB, m_vertexC, m_vertexD);
    Rebuild();
}

void RectangleLines::SetVertexA(Vector2Df v){ m_vertexA = v; Rebuild(); }
void RectangleLines::SetVertexB(Vector2Df v){ m_vertexB = v; Rebuild(); }
void RectangleLines::SetVertexC(Vector2Df v){ m_vertexC = v; Rebuild(); }
void RectangleLines::SetVertexD(Vector2Df v){ m_vertexD = v; Rebuild(); }

void RectangleLines::SetWidth(float width){
    LayoutFromAandSize(m_vertexA, width, GetHeight(), m_vertexB, m_vertexC, m_vertexD);
    Rebuild();
}

void RectangleLines::SetHeight(float height){
    LayoutFromAandSize(m_vertexA, GetWidth(), height, m_vertexB, m_vertexC, m_vertexD);
    Rebuild();
}

void RectangleLines::SetSize(float width, float height){
    LayoutFromAandSize(m_vertexA, width, height, m_vertexB, m_vertexC, m_vertexD);
    Rebuild();
}

void RectangleLines::SetSize(Vector2Df size){ SetSize(size.x, size.y); }

// -------- //
// Commands //
// -------- //

void RectangleLines::Delete(){
    m_edgeAB.Delete();
    m_edgeBD.Delete();
    m_edgeCD.Delete();
    m_edgeCA.Delete();
}

void RectangleLines::Draw(){
    m_edgeAB.Draw();
    m_edgeBD.Draw();
    m_edgeCD.Draw();
    m_edgeCA.Draw();
}
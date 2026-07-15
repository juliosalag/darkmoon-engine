#include <darkmoon/2D/circle.hpp>

static constexpr float PI = 3.14159265358979323846f;

static std::vector<float> buildFanNDC(Vector2Df center, float radius, int segments,
                                       float rotationDeg, float winW, float winH,
                                       float toNDC_X(float, float), float toNDC_Y(float, float))
{
    std::vector<float> verts;
    verts.reserve(2 * (segments + 2));

    verts.push_back(toNDC_X(center.x, winW));
    verts.push_back(toNDC_Y(center.y, winH));

    float rotRad = rotationDeg * (PI / 180.0f);

    for (int i = 0; i <= segments; ++i) {
        float angle = rotRad + (2.0f * PI * static_cast<float>(i) / static_cast<float>(segments));
        verts.push_back(toNDC_X(center.x + radius * std::cos(angle), winW));
        verts.push_back(toNDC_Y(center.y + radius * std::sin(angle), winH));
    }

    return verts;
}

static std::vector<float> buildLoopNDC(Vector2Df center, float radius, int segments,
                                        float rotationDeg, float winW, float winH,
                                        float toNDC_X(float, float), float toNDC_Y(float, float))
{
    std::vector<float> verts;
    verts.reserve(2 * segments);

    float rotRad = rotationDeg * (PI / 180.0f);

    for (int i = 0; i < segments; ++i) {
        float angle = rotRad + (2.0f * PI * static_cast<float>(i) / static_cast<float>(segments));
        verts.push_back(toNDC_X(center.x + radius * std::cos(angle), winW));
        verts.push_back(toNDC_Y(center.y + radius * std::sin(angle), winH));
    }

    return verts;
}

static void uploadVBO(GLuint vao, GLuint vbo, const std::vector<float>& verts) {
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(verts.size() * sizeof(float)), verts.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

static void drawSetup(Shader* shader, Color color) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glUseProgram(shader->getIDShader());
    GLint colorUniform = glGetUniformLocation(shader->getIDShader(), "customColor");
    glUniform4fv(colorUniform, 1, glm::value_ptr(glm::vec4(
        color.r / 255.f, color.g / 255.f,
        color.b / 255.f, color.a / 255.f)));
}

// ------ //
// Circle //
// ------ //

Circle::Circle(Vector2Df center, float radius, Color color, Window* window, int segments, Shader* shader)
    : m_center(center), m_radius(radius), m_segments(segments),
      m_color(color),
      m_shader(shader == nullptr ? window->GetBasicShader2D() : shader),
      m_window(window)
{
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    Rebuild();
}

void Circle::Rebuild() {
    float w = static_cast<float>(m_window->GetWidth());
    float h = static_cast<float>(m_window->GetHeight());
    auto verts = buildFanNDC(m_center, m_radius, m_segments, 0.0f, w, h, toNDC_X, toNDC_Y);
    uploadVBO(m_VAO, m_VBO, verts);
}

void Circle::SetCenter(Vector2Df center) { m_center   = center;   Rebuild(); }
void Circle::SetRadius(float radius)     { m_radius   = radius;   Rebuild(); }
void Circle::SetSegments(int segments)   { m_segments = segments; Rebuild(); }

void Circle::Delete() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
}

void Circle::Draw() {
    drawSetup(m_shader, m_color);
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, m_segments + 2);
    glBindVertexArray(0);
    glDisable(GL_BLEND);
}

void Circle::Draw(const Camera2D& camera) {
    float w = static_cast<float>(m_window->GetWidth());
    float h = static_cast<float>(m_window->GetHeight());

    Vector2Df screenCenter = camera.WorldToScreen(m_center);
    float screenRadius     = m_radius * camera.zoom;

    auto verts = buildFanNDC(screenCenter, screenRadius, m_segments, 0.0f, w, h, toNDC_X, toNDC_Y);

    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    uploadVBO(vao, vbo, verts);

    drawSetup(m_shader, m_color);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, m_segments + 2);
    glBindVertexArray(0);
    glDisable(GL_BLEND);

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

// ----------- //
// CircleLines //
// ----------- //

CircleLines::CircleLines(Vector2Df center, float radius, Color color, Window* window, int segments, float lineWidth, Shader* shader)
    : m_center(center), m_radius(radius), m_segments(segments),
      m_lineWidth(lineWidth), m_color(color),
      m_shader(shader == nullptr ? window->GetBasicShader2D() : shader),
      m_window(window)
{
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    Rebuild();
}

void CircleLines::Rebuild() {
    float w = static_cast<float>(m_window->GetWidth());
    float h = static_cast<float>(m_window->GetHeight());
    auto verts = buildLoopNDC(m_center, m_radius, m_segments, 0.0f, w, h, toNDC_X, toNDC_Y);
    uploadVBO(m_VAO, m_VBO, verts);
}

void CircleLines::SetCenter(Vector2Df center) { m_center   = center;   Rebuild(); }
void CircleLines::SetRadius(float radius)     { m_radius   = radius;   Rebuild(); }
void CircleLines::SetSegments(int segments)   { m_segments = segments; Rebuild(); }

void CircleLines::Delete() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
}

void CircleLines::Draw() {
    drawSetup(m_shader, m_color);
    glLineWidth(m_lineWidth);
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_LINE_LOOP, 0, m_segments);
    glBindVertexArray(0);
    glLineWidth(1.0f);
    glDisable(GL_BLEND);
}

void CircleLines::Draw(const Camera2D& camera) {
    float w = static_cast<float>(m_window->GetWidth());
    float h = static_cast<float>(m_window->GetHeight());

    Vector2Df screenCenter = camera.WorldToScreen(m_center);
    float screenRadius     = m_radius * camera.zoom;

    auto verts = buildLoopNDC(screenCenter, screenRadius, m_segments, 0.0f, w, h, toNDC_X, toNDC_Y);

    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    uploadVBO(vao, vbo, verts);

    drawSetup(m_shader, m_color);
    glLineWidth(m_lineWidth);
    glBindVertexArray(vao);
    glDrawArrays(GL_LINE_LOOP, 0, m_segments);
    glBindVertexArray(0);
    glLineWidth(1.0f);
    glDisable(GL_BLEND);

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}
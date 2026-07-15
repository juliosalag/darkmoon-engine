#include <darkmoon/2D/text.hpp>

static int nextCodepoint(const char*& s) {
    unsigned char c = (unsigned char)*s++;
    if (c < 0x80)  return c;
    if (c < 0xE0) { int cp = (c & 0x1F) << 6;  cp |= (*s++ & 0x3F); return cp; }
    if (c < 0xF0) { int cp = (c & 0x0F) << 12; cp |= (*s++ & 0x3F) << 6;  cp |= (*s++ & 0x3F); return cp; }
                   { int cp = (c & 0x07) << 18; cp |= (*s++ & 0x3F) << 12; cp |= (*s++ & 0x3F) << 6; cp |= (*s++ & 0x3F); return cp; }
}

Text::Text(Vector2Df position, const std::string& text, const char* fontPath, Window* window, float pixelHeight, Shader* shader)
    : m_position(position), m_text(text), m_pixelHeight(pixelHeight), m_window(window), m_shader(shader == nullptr ? window->GetBasicFontShader2D() : shader)
{
    ResourceManager& RM = ResourceManager::getInstance();

    m_resourceFont = RM.loadResource<ResourceFont>(fontPath, pixelHeight);

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

float Text::toNDC_X(float px) const {
    return (px / static_cast<float>(m_window->GetWidth())) * 2.f - 1.f;
}

float Text::toNDC_Y(float py) const {
    return -((py / static_cast<float>(m_window->GetHeight())) * 2.f - 1.f);
}

void Text::Draw() {
    if (!m_resourceFont || !m_resourceFont->isLoaded()) return;
    if (m_text.empty()) return;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(m_shader->getIDShader());

    GLint colorUniform = glGetUniformLocation(m_shader->getIDShader(), "customColor");
    glUniform4f(colorUniform, m_color.r / 255.f, m_color.g / 255.f, m_color.b / 255.f, m_color.a / 255.f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_resourceFont->atlasID);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    float cursorX = m_position.x;
    float cursorY = m_position.y + m_resourceFont->ascent * m_scale;

    const char* ptr = m_text.c_str();
    while (*ptr) {
        int codepoint = nextCodepoint(ptr);

        auto it = m_resourceFont->glyphs.find(codepoint);
        if (it == m_resourceFont->glyphs.end()) {
            cursorX += m_pixelHeight * 0.3f * m_scale;
            continue;
        }

        const GlyphInfo& g = it->second;

        float px0 = cursorX + g.bearingX * m_scale;
        float py0 = cursorY + g.bearingY * m_scale;
        float px1 = px0     + g.width    * m_scale;
        float py1 = py0     + g.height   * m_scale;

        float x0 = toNDC_X(px0), y0 = toNDC_Y(py0);
        float x1 = toNDC_X(px1), y1 = toNDC_Y(py1);

        float verts[6][4] = {
            { x0, y0,  g.u0, g.v0 },
            { x0, y1,  g.u0, g.v1 },
            { x1, y1,  g.u1, g.v1 },

            { x0, y0,  g.u0, g.v0 },
            { x1, y1,  g.u1, g.v1 },
            { x1, y0,  g.u1, g.v0 },
        };

        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        cursorX += g.advance * m_scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
    glDisable(GL_BLEND);
}

void Text::Draw(const Camera2D& camera) {
    Vector2Df screen = camera.WorldToScreen(m_position);

    Vector2Df savedPos   = m_position;
    float     savedScale = m_scale;

    m_position = screen;
    m_scale   *= camera.zoom;

    Draw();

    m_position = savedPos;
    m_scale    = savedScale;
}

float Text::MeasureText() const {
    float width = 0.f;
    const char* ptr = m_text.c_str();
    while (*ptr) {
        int codepoint = nextCodepoint(ptr);
        auto it = m_resourceFont->glyphs.find(codepoint);
        if (it == m_resourceFont->glyphs.end()) {
            width += m_pixelHeight * 0.3f * m_scale;
            continue;
        }
        width += it->second.advance * m_scale;
    }
    return width;
}

float Text::GetTextHeight() const {
    return (m_resourceFont->ascent - m_resourceFont->descent) * m_scale;
}

void Text::Unload() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    m_VAO = m_VBO = 0;
}
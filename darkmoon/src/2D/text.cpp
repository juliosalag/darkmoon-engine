#include "text.hpp"

Text::Text(Vector2D position, const char* fontPath, float pixelHeight, Window* window, Shader* shader)
    : m_position(position), m_pixelHeight(pixelHeight), m_window(window), m_shader(shader == nullptr ? window->GetBasicFontShader2D() : shader)
{
    m_resourceFont = m_RM.loadResource<ResourceFont>(fontPath, pixelHeight);

    // VAO + VBO dinamic - 6 vertex × 4 floats (xy + uv), un quad por carácter
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

    // xy
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // uv
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

void Text::Draw(const std::string& text) {
    if (!m_resourceFont || !m_resourceFont->isLoaded()) return;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(m_shader->getIDShader());

    // Color uniform
    GLint colorUniform = glGetUniformLocation(m_shader->getIDShader(), "customColor");
    glUniform4f(colorUniform, m_color.r / 255.f, m_color.g / 255.f, m_color.b / 255.f, m_color.a / 255.f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_resourceFont->atlasID);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    float cursorX = static_cast<float>(m_position.x);
    float cursorY = static_cast<float>(m_position.y);

    for (char c : text) {
        int idx = static_cast<int>(c) - ResourceFont::FIRST_CHAR;
        if (idx < 0 || idx >= ResourceFont::CHAR_COUNT) {
            cursorX += m_pixelHeight * 0.3f;
            continue;
        }

        const GlyphInfo& g = m_resourceFont->glyphs[idx];

        float px0 = cursorX + g.bearingX;
        float py0 = cursorY + g.bearingY;
        float px1 = px0     + g.width;
        float py1 = py0     + g.height;

        // Convert to NDC
        float x0 = toNDC_X(px0),  y0 = toNDC_Y(py0);
        float x1 = toNDC_X(px1),  y1 = toNDC_Y(py1);

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

        cursorX += g.advance;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
    glDisable(GL_BLEND);
}

float Text::MeasureText(const std::string& text) const {
    float width = 0.f;
    for (char c : text) {
        int idx = static_cast<int>(c) - ResourceFont::FIRST_CHAR;
        if (idx < 0 || idx >= ResourceFont::CHAR_COUNT){ 
            width += m_pixelHeight * 0.3f; continue; 
        }
        width += m_resourceFont->glyphs[idx].advance;
    }
    return width;
}

void Text::Unload() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    m_VAO = m_VBO = 0;
}
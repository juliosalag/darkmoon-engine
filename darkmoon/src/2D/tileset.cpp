#include <darkmoon/2D/tileset.hpp>
#include <glad/glad.h>

TileSet::TileSet(int tileW, int tileH, int cols, int rows, const char* texturePath, Window* window, Shader* shader)
    : m_tileW(tileW), m_tileH(tileH), m_cols(cols), m_rows(rows), m_window(window), 
      m_shader(shader == nullptr ? window->GetBasicTextureShader2D() : shader)    
{
    m_resourceTexture = m_RM.loadResource<ResourceTexture>(texturePath);
    InitGLBuffers();
}

TileSet::~TileSet() {
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_ebo) glDeleteBuffers(1, &m_ebo);
}

void TileSet::InitGLBuffers() {
    m_colorUniformLoc = glGetUniformLocation(m_shader->getIDShader(), "customColor");

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, 4 * 7 * sizeof(float), nullptr, GL_STREAM_DRAW);

    unsigned int indices[] = { 0, 1, 2, 1, 2, 3 };
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void TileSet::IndexToXY(int index, int& outCol, int& outRow) const {
    const int count = m_cols * m_rows;
    if (count > 0) 
        index = ((index % count) + count) % count;
    outCol = index % m_cols;
    outRow = index / m_cols;
}

void TileSet::DrawQuad(Vector2Df screenTL, Vector2Df screenBR, float uvX0, float uvY0, float uvX1, float uvY1, float winW, float winH) const {
    auto [x0, y0] = ToNDC(screenTL, winW, winH);
    auto [x1, y1] = ToNDC(screenBR, winW, winH);

    float r = m_color.r / 255.f;
    float g = m_color.g / 255.f;
    float b = m_color.b / 255.f;

    float vertices[] = {
        x0, y0,   r, g, b,   uvX0, uvY0,
        x1, y0,   r, g, b,   uvX1, uvY0,
        x0, y1,   r, g, b,   uvX0, uvY1,
        x1, y1,   r, g, b,   uvX1, uvY1,
    };

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(m_shader->getIDShader());
    
    if (m_colorUniformLoc != -1) {
        glUniform4fv(m_colorUniformLoc, 1, glm::value_ptr(glm::vec4(r, g, b, m_color.a / 255.f)));
    }

    glBindTexture(GL_TEXTURE_2D, m_resourceTexture->getIDTexture());
    
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
}

void TileSet::SetTexture(const char* path) {
    m_resourceTexture = m_RM.loadResource<ResourceTexture>(path);
}

void TileSet::Draw(int col, int row, Vector2Df pos) const {
    float winW = static_cast<float>(m_window->GetWidth());
    float winH = static_cast<float>(m_window->GetHeight());

    float sheetW = static_cast<float>(m_resourceTexture->getWitdh());
    float sheetH = static_cast<float>(m_resourceTexture->getHeight());

    float tw = static_cast<float>(m_tileW);
    float th = static_cast<float>(m_tileH);

    float uvX0 = (col * tw) / sheetW;
    float uvY0 = (row * th) / sheetH;
    float uvX1 = uvX0 + (tw / sheetW);
    float uvY1 = uvY0 + (th / sheetH); 

    float scaledW = tw * m_scale.x;
    float scaledH = th * m_scale.y;

    DrawQuad(pos, { pos.x + scaledW, pos.y + scaledH }, uvX0, uvY0, uvX1, uvY1, winW, winH);
}

void TileSet::Draw(int index, Vector2Df pos) const {
    int col, row;
    IndexToXY(index, col, row);
    Draw(col, row, pos);
}

void TileSet::Draw(int col, int row, Vector2Df pos, const Camera2D& camera) const {
    float winW = static_cast<float>(m_window->GetWidth());
    float winH = static_cast<float>(m_window->GetHeight());

    float sheetW = static_cast<float>(m_resourceTexture->getWitdh());
    float sheetH = static_cast<float>(m_resourceTexture->getHeight());

    float tw = static_cast<float>(m_tileW);
    float th = static_cast<float>(m_tileH);

    float uvX0 = (col * tw) / sheetW;
    float uvY0 = (row * th) / sheetH;
    float uvX1 = uvX0 + (tw / sheetW);
    float uvY1 = uvY0 + (th / sheetH);

    float scaledW = tw * m_scale.x;
    float scaledH = th * m_scale.y;

    Vector2Df screenTL = camera.WorldToScreen(pos);
    Vector2Df screenBR = camera.WorldToScreen({ pos.x + scaledW, pos.y + scaledH });

    DrawQuad(screenTL, screenBR, uvX0, uvY0, uvX1, uvY1, winW, winH);
}

void TileSet::Draw(int index, Vector2Df pos, const Camera2D& camera) const {
    int col, row;
    IndexToXY(index, col, row);
    Draw(col, row, pos, camera);
}
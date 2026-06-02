#include <darkmoon/2D/texture.hpp>

// ------- //
// Texture //
// ------- //

Texture::Texture(Vector2Df position, Vector2Df scale, float rotation,
                 const char* texturePath, Window* window, Shader* shader)
    : m_position(position), m_scale(scale), m_rotation(rotation),
      m_shader(shader == nullptr ? window->GetBasicTextureShader2D() : shader),
      m_window(window)
{
    m_resourceTexture = m_RM.loadResource<ResourceTexture>(texturePath);

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    Rebuild();
}

// ------- //
// Private //
// ------- //

void Texture::Rebuild() {
    float winW = static_cast<float>(m_window->GetWidth());
    float winH = static_cast<float>(m_window->GetHeight());

    float posX = m_position.x;
    float posY = m_position.y;

    float texW = static_cast<float>(m_resourceTexture->getWitdh())  * m_scale.x;
    float texH = static_cast<float>(m_resourceTexture->getHeight()) * m_scale.y;

    float r = m_color.r / 255.f;
    float g = m_color.g / 255.f;
    float b = m_color.b / 255.f;

    // NDC corners
    auto [x0, y0] = ToNDC({ posX,        posY        }, winW, winH);
    auto [x1, y1] = ToNDC({ posX + texW, posY + texH }, winW, winH);

    //  A(x0,y0) --- B(x1,y0)
    //  |            |
    //  C(x0,y1) --- D(x1,y1)

    float vertex[] = {
        // pos         color      uv
        x0, y0,   r, g, b,   0.f, 0.f,   // A  top-left
        x1, y0,   r, g, b,   1.f, 0.f,   // B  top-right
        x0, y1,   r, g, b,   0.f, 1.f,   // C  bottom-left
        x1, y1,   r, g, b,   1.f, 1.f,   // D  bottom-right
    };

    GLuint index[] = { 0, 1, 2, 1, 2, 3 };

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_DYNAMIC_DRAW);

    // position  (location 0)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color     (location 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // uv        (location 2)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// ------- //
// Getters //
// ------- //

float Texture::GetWidth()  const { return static_cast<float>(m_resourceTexture->getWitdh())  * m_scale.x; }
float Texture::GetHeight() const { return static_cast<float>(m_resourceTexture->getHeight()) * m_scale.y; }

int Texture::GetTextureWidth()  const { return m_resourceTexture->getWitdh();  }
int Texture::GetTextureHeight() const { return m_resourceTexture->getHeight(); }

// ------- //
// Setters //
// ------- //

void Texture::SetPosition(Vector2Df position) {
    m_position = position;
    Rebuild();
}

void Texture::SetScale(Vector2Df scale) {
    m_scale = scale;
    Rebuild();
}

void Texture::SetRotation(float rotation) {
    // Stored for future transform-matrix support
    m_rotation = rotation;
    // Rebuild() not needed until rotation is applied in vertex data
}

void Texture::SetColor(Color color) {
    m_color = color;
    Rebuild();
}

void Texture::SetTexture(const char* texturePath) {
    m_resourceTexture = m_RM.loadResource<ResourceTexture>(texturePath);
    Rebuild();
}

// -------- //
// Commands //
// -------- //

void Texture::Delete() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
}

void Texture::Draw() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(m_shader->getIDShader());

    GLint colorUniform = glGetUniformLocation(m_shader->getIDShader(), "customColor");
    glUniform4fv(colorUniform, 1, glm::value_ptr(glm::vec4(
        m_color.r / 255.f, m_color.g / 255.f,
        m_color.b / 255.f, m_color.a / 255.f)));

    glBindTexture(GL_TEXTURE_2D, m_resourceTexture->getIDTexture());
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisable(GL_BLEND);
}

void Texture::Draw(const Camera2D& camera) {
    float winW = static_cast<float>(m_window->GetWidth());
    float winH = static_cast<float>(m_window->GetHeight());

    float texW = static_cast<float>(m_resourceTexture->getWitdh())  * m_scale.x;
    float texH = static_cast<float>(m_resourceTexture->getHeight()) * m_scale.y;

    // Transform the two defining corners through the camera
    Vector2Df worldTL = m_position;
    Vector2Df worldBR = { m_position.x + texW, m_position.y + texH };

    auto [x0, y0] = ToNDC(camera.WorldToScreen(worldTL), winW, winH);
    auto [x1, y1] = ToNDC(camera.WorldToScreen(worldBR), winW, winH);

    float r = m_color.r / 255.f;
    float g = m_color.g / 255.f;
    float b = m_color.b / 255.f;

    float vertex[] = {
        x0, y0,   r, g, b,   0.f, 0.f,
        x1, y0,   r, g, b,   1.f, 0.f,
        x0, y1,   r, g, b,   0.f, 1.f,
        x1, y1,   r, g, b,   1.f, 1.f,
    };
    GLuint index[] = { 0, 1, 2, 1, 2, 3 };

    // Temporal VAO/VBO/EBO (same approach as Rectangle::Draw(camera))
    GLuint vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(m_shader->getIDShader());
    GLint colorUniform = glGetUniformLocation(m_shader->getIDShader(), "customColor");
    glUniform4fv(colorUniform, 1, glm::value_ptr(glm::vec4(
        m_color.r / 255.f, m_color.g / 255.f,
        m_color.b / 255.f, m_color.a / 255.f)));

    glBindTexture(GL_TEXTURE_2D, m_resourceTexture->getIDTexture());
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);

    glDisable(GL_BLEND);

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}
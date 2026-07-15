#include <darkmoon/2D/animated_texture.hpp>

#include <utility>
#include <algorithm>

// ----------- //
// Constructor //
// ----------- //

AnimatedTexture::AnimatedTexture(Vector2Df position, Vector2Df scale, float rotation, const char* texturePath, Window* window, Shader* shader)
    : m_position(position), m_scale(scale), m_rotation(rotation), m_shader(shader == nullptr ? window->GetBasicTextureShader2D() : shader), m_window(window)
{
    m_resource = m_RM.loadResource<ResourceAnimatedTexture>(texturePath);

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    Rebuild();
}

// -------------- //
// Move semantics //
// -------------- //

AnimatedTexture::AnimatedTexture(AnimatedTexture&& o) noexcept
    : m_VAO(o.m_VAO), m_VBO(o.m_VBO), m_EBO(o.m_EBO),
      m_currentFrame(o.m_currentFrame), m_elapsed(o.m_elapsed),
      m_playing(o.m_playing), m_looping(o.m_looping), m_speed(o.m_speed),
      m_position(o.m_position), m_scale(o.m_scale), m_rotation(o.m_rotation),
      m_color(o.m_color),
      m_shader(o.m_shader), m_window(o.m_window), m_resource(o.m_resource)
{
    o.m_VAO = o.m_VBO = o.m_EBO = 0;
    o.m_resource = nullptr;
}

AnimatedTexture& AnimatedTexture::operator=(AnimatedTexture&& o) noexcept {
    if (this != &o) {
        Delete();
        new (this) AnimatedTexture(std::move(o));
    }
    return *this;
}

// ------- //
// Private //
// ------- //

void AnimatedTexture::Rebuild() {
    float winW = static_cast<float>(m_window->GetWidth());
    float winH = static_cast<float>(m_window->GetHeight());

    float texW = static_cast<float>(m_resource->getWidth())  * m_scale.x;
    float texH = static_cast<float>(m_resource->getHeight()) * m_scale.y;

    float r = m_color.r / 255.f;
    float g = m_color.g / 255.f;
    float b = m_color.b / 255.f;

    auto [x0, y0] = ToNDC(m_position,                          winW, winH);
    auto [x1, y1] = ToNDC({ m_position.x + texW, m_position.y + texH }, winW, winH);

    float vertex[] = {
        x0, y0,   r, g, b,   0.f, 0.f,
        x1, y0,   r, g, b,   1.f, 0.f,
        x0, y1,   r, g, b,   0.f, 1.f,
        x1, y1,   r, g, b,   1.f, 1.f,
    };
    GLuint index[] = { 0, 1, 2, 1, 2, 3 };

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// --------- //
// Animation //
// --------- //

void AnimatedTexture::Update(float dt) {
    if (!m_playing || !m_resource || m_resource->getFrameCount() == 0) return;

    m_elapsed += dt * m_speed;

    float frameDur = m_resource->getFrameDelay(m_currentFrame);

    while (m_elapsed >= frameDur) {
        m_elapsed -= frameDur;

        int next = m_currentFrame + 1;

        if (next >= m_resource->getFrameCount()) {
            if (m_looping) {
                next = 0;
            } else {
                m_currentFrame = m_resource->getFrameCount() - 1;
                m_elapsed      = 0.f;
                m_playing      = false;
                return;
            }
        }

        m_currentFrame = next;
        frameDur = m_resource->getFrameDelay(m_currentFrame);
    }
}

void AnimatedTexture::SetFrame(int index) {
    if (!m_resource) return;
    m_currentFrame = std::max(0, std::min(index, m_resource->getFrameCount() - 1));
    m_elapsed = 0.f;
}

// ------- //
// Setters //
// ------- //

void AnimatedTexture::SetPosition(Vector2Df position) { m_position = position; Rebuild(); }
void AnimatedTexture::SetScale(Vector2Df scale)        { m_scale    = scale;    Rebuild(); }
void AnimatedTexture::SetColor(Color color)            { m_color    = color;    Rebuild(); }

void AnimatedTexture::SetRotation(float rotation) {
    m_rotation = rotation;
}

void AnimatedTexture::SetTexture(const char* texturePath) {
    m_resource = m_RM.loadResource<ResourceAnimatedTexture>(texturePath);
    m_currentFrame = 0;
    m_elapsed      = 0.f;
    Rebuild();
}

// ------------ //
// Draw helpers //
// ------------ //

void AnimatedTexture::DrawWith(GLuint texID) const {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(m_shader->getIDShader());

    GLint loc = glGetUniformLocation(m_shader->getIDShader(), "customColor");
    glUniform4fv(loc, 1, glm::value_ptr(glm::vec4(m_color.r / 255.f, m_color.g / 255.f, m_color.b / 255.f, m_color.a / 255.f)));

    glBindTexture(GL_TEXTURE_2D, texID);
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisable(GL_BLEND);
}

void AnimatedTexture::DrawWith(GLuint texID, const Camera2D& camera) const {
    float winW = static_cast<float>(m_window->GetWidth());
    float winH = static_cast<float>(m_window->GetHeight());

    float texW = static_cast<float>(m_resource->getWidth())  * m_scale.x;
    float texH = static_cast<float>(m_resource->getHeight()) * m_scale.y;

    auto [x0, y0] = ToNDC(camera.WorldToScreen(m_position), winW, winH);
    auto [x1, y1] = ToNDC(camera.WorldToScreen({ m_position.x + texW, m_position.y + texH }), winW, winH);

    float r = m_color.r / 255.f;
    float g = m_color.g / 255.f;
    float b = m_color.b / 255.f;

    float vertex[] = {
        x0, y0,   r, g, b,   0.f, 0.f,
        x1, y0,   r, g, b,   1.f, 0.f,
        x0, y1,   r, g, b,   0.f, 1.f,
        x1, y1,   r, g, b,   1.f, 1.f,
    };
    GLuint idx[] = { 0, 1, 2, 1, 2, 3 };

    GLuint vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(m_shader->getIDShader());
    GLint loc = glGetUniformLocation(m_shader->getIDShader(), "customColor");
    glUniform4fv(loc, 1, glm::value_ptr(glm::vec4( m_color.r / 255.f, m_color.g / 255.f, m_color.b / 255.f, m_color.a / 255.f)));

    glBindTexture(GL_TEXTURE_2D, texID);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glDisable(GL_BLEND);

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

// -------------------- //
// Public Draw / Delete //
// -------------------- //

void AnimatedTexture::Draw() {
    if (!m_resource || m_resource->getFrameCount() == 0) return;
    DrawWith(m_resource->getFrameTexture(m_currentFrame));
}

void AnimatedTexture::Draw(const Camera2D& camera) {
    if (!m_resource || m_resource->getFrameCount() == 0) return;
    DrawWith(m_resource->getFrameTexture(m_currentFrame), camera);
}

void AnimatedTexture::Delete() {
    if (m_VAO) { glDeleteVertexArrays(1, &m_VAO); m_VAO = 0; }
    if (m_VBO) { glDeleteBuffers(1, &m_VBO);      m_VBO = 0; }
    if (m_EBO) { glDeleteBuffers(1, &m_EBO);      m_EBO = 0; }
}
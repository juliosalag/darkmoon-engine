#include <darkmoon/2D/animated_texture.hpp>

#include "../libs/stb_image.h"

#include <cstring>
#include <cassert>
#include <utility>

// ---------------- //
// Internal helpers //
// ---------------- //

namespace {

// Centiseconds → seconds.  GIF spec stores delays in 1/100 s units.
constexpr float CentiToSec(int cs) {
    // Browsers treat 0 (or ≤1) as 10 cs (100 ms) — mirror that behaviour.
    return (cs <= 1 ? 10 : cs) / 100.f;
}

} // anonymous namespace

// -------------------------------- //
// Constructor / destructor helpers //
// -------------------------------- //

AnimatedTexture::AnimatedTexture(Vector2Df position, Vector2Df scale, float rotation, const char* texturePath, Window* window, Shader* shader)
    : m_position(position), m_scale(scale), m_rotation(rotation), m_shader(shader == nullptr ? window->GetBasicTextureShader2D() : shader), m_window(window)
{
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    LoadGif(texturePath);
    Rebuild();
}

// -------------- //
// Move semantics //
// -------------- //

AnimatedTexture::AnimatedTexture(AnimatedTexture&& o) noexcept
    : m_frames(std::move(o.m_frames)), m_currentFrame(o.m_currentFrame), m_elapsed(o.m_elapsed), m_playing(o.m_playing), m_looping(o.m_looping), m_speed(o.m_speed),
      m_VAO(o.m_VAO), m_VBO(o.m_VBO), m_EBO(o.m_EBO), m_position(o.m_position), m_scale(o.m_scale), m_rotation(o.m_rotation),
      m_color(o.m_color), m_frameWidth(o.m_frameWidth), m_frameHeight(o.m_frameHeight), m_shader(o.m_shader), m_window(o.m_window), m_path(std::move(o.m_path))
{
    o.m_VAO = o.m_VBO = o.m_EBO = 0;
    o.m_frames.clear();
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

void AnimatedTexture::LoadGif(const char* path) {
    FreeFrames();
    m_path = path;

    // ---------------------------------------------------------------- //
    // 1. Read file into memory so we can use stbi_load_gif_from_memory //
    //    (the public, stable API that replaced the private internals). //
    // ---------------------------------------------------------------- //
    FILE* f = std::fopen(path, "rb");
    assert(f && "AnimatedTexture: cannot open file");

    std::fseek(f, 0, SEEK_END);
    long fileSize = std::ftell(f);
    std::rewind(f);

    std::vector<stbi_uc> fileData(static_cast<size_t>(fileSize));
    std::fread(fileData.data(), 1, static_cast<size_t>(fileSize), f);
    std::fclose(f);

    // ---------------------- //
    // 2. Try GIF path first. //
    // ---------------------- //
    int w = 0, h = 0, nFrames = 0, comp = 0;
    int* delays = nullptr;

    // stbi_load_gif_from_memory returns a contiguous buffer:
    //   [frame0 RGBA][frame1 RGBA]...[frameN RGBA]
    // with a separate int* delays array (centiseconds per frame).
    stbi_uc* pixels = stbi_load_gif_from_memory(
        fileData.data(), static_cast<int>(fileData.size()),
        &delays, &w, &h, &nFrames, &comp, 4);

    if (pixels && nFrames > 0) {
        m_frameWidth  = w;
        m_frameHeight = h;
        m_frames.reserve(static_cast<size_t>(nFrames));

        const size_t frameBytes = static_cast<size_t>(w * h * 4);

        for (int i = 0; i < nFrames; ++i) {
            Frame frame;
            frame.delay     = (delays != nullptr) ? CentiToSec(delays[i]) : 0.1f;
            frame.textureID = UploadTexture(pixels + i * frameBytes, w, h);
            m_frames.push_back(frame);
        }

        stbi_image_free(pixels);
        if (delays) stbi_image_free(delays);
        return;
    }

    // ------------------------------------------- //
    // 3. Fall back to regular single-frame image. //
    // ------------------------------------------- //
    if (pixels) { stbi_image_free(pixels); pixels = nullptr; }
    if (delays) { stbi_image_free(delays); delays = nullptr; }

    stbi_uc* img = stbi_load(path, &w, &h, &comp, 4);
    assert(img && "AnimatedTexture: stbi_load failed");

    m_frameWidth  = w;
    m_frameHeight = h;

    Frame frame;
    frame.delay     = 0.1f;
    frame.textureID = UploadTexture(img, w, h);
    m_frames.push_back(frame);

    stbi_image_free(img);
}

void AnimatedTexture::FreeFrames() {
    for (auto& f : m_frames)
        if (f.textureID) glDeleteTextures(1, &f.textureID);
    m_frames.clear();
    m_currentFrame = 0;
    m_elapsed      = 0.f;
}

GLuint AnimatedTexture::UploadTexture(const unsigned char* pixels, int w, int h) {
    GLuint id = 0;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    glBindTexture(GL_TEXTURE_2D, 0);
    return id;
}

// ------------------------------------------------------------ //
// Rebuild: upload geometry to GPU from current transform state //
// ------------------------------------------------------------ //

void AnimatedTexture::Rebuild() {
    float winW = static_cast<float>(m_window->GetWidth());
    float winH = static_cast<float>(m_window->GetHeight());

    float posX = m_position.x;
    float posY = m_position.y;

    float texW = static_cast<float>(m_frameWidth)  * m_scale.x;
    float texH = static_cast<float>(m_frameHeight) * m_scale.y;

    float r = m_color.r / 255.f;
    float g = m_color.g / 255.f;
    float b = m_color.b / 255.f;

    auto [x0, y0] = ToNDC({ posX,        posY        }, winW, winH);
    auto [x1, y1] = ToNDC({ posX + texW, posY + texH }, winW, winH);

    //  A(x0,y0) --- B(x1,y0)
    //  |                    |
    //  C(x0,y1) --- D(x1,y1)

    float vertex[] = {
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

// --------- //
// Animation //
// --------- //

void AnimatedTexture::Update(float dt) {
    if (!m_playing || m_frames.empty()) return;

    m_elapsed += dt * m_speed;

    float frameDur = m_frames[static_cast<size_t>(m_currentFrame)].delay;

    while (m_elapsed >= frameDur) {
        m_elapsed -= frameDur;

        int next = m_currentFrame + 1;

        if (next >= static_cast<int>(m_frames.size())) {
            if (m_looping) {
                next = 0;
            } else {
                m_currentFrame = static_cast<int>(m_frames.size()) - 1;
                m_elapsed      = 0.f;
                m_playing      = false;
                return;
            }
        }

        m_currentFrame = next;
        frameDur = m_frames[static_cast<size_t>(m_currentFrame)].delay;
    }
}

void AnimatedTexture::SetFrame(int index) {
    if (m_frames.empty()) return;
    m_currentFrame = std::max(0, std::min(index, static_cast<int>(m_frames.size()) - 1));
    m_elapsed = 0.f;
}

// ------- //
// Setters //
// ------- //

void AnimatedTexture::SetPosition(Vector2Df position)  { m_position = position; Rebuild(); }
void AnimatedTexture::SetScale(Vector2Df scale)        { m_scale    = scale;    Rebuild(); }
void AnimatedTexture::SetColor(Color color)            { m_color    = color;    Rebuild(); }

void AnimatedTexture::SetRotation(float rotation) {
    m_rotation = rotation;
    // Rebuild() not needed until rotation is applied via a transform matrix.
}

void AnimatedTexture::SetTexture(const char* texturePath) {
    LoadGif(texturePath);
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
    glUniform4fv(loc, 1, glm::value_ptr(glm::vec4(
        m_color.r / 255.f, m_color.g / 255.f,
        m_color.b / 255.f, m_color.a / 255.f)));

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

    float texW = static_cast<float>(m_frameWidth)  * m_scale.x;
    float texH = static_cast<float>(m_frameHeight) * m_scale.y;

    auto [x0, y0] = ToNDC(camera.WorldToScreen(m_position),
                           winW, winH);
    auto [x1, y1] = ToNDC(camera.WorldToScreen({ m_position.x + texW,
                                                  m_position.y + texH }),
                           winW, winH);

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
    glUniform4fv(loc, 1, glm::value_ptr(glm::vec4(
        m_color.r / 255.f, m_color.g / 255.f,
        m_color.b / 255.f, m_color.a / 255.f)));

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
    if (m_frames.empty()) return;
    DrawWith(m_frames[static_cast<size_t>(m_currentFrame)].textureID);
}

void AnimatedTexture::Draw(const Camera2D& camera) {
    if (m_frames.empty()) return;
    DrawWith(m_frames[static_cast<size_t>(m_currentFrame)].textureID, camera);
}

void AnimatedTexture::Delete() {
    FreeFrames();

    if (m_VAO) { glDeleteVertexArrays(1, &m_VAO); m_VAO = 0; }
    if (m_VBO) { glDeleteBuffers(1, &m_VBO);      m_VBO = 0; }
    if (m_EBO) { glDeleteBuffers(1, &m_EBO);      m_EBO = 0; }
}
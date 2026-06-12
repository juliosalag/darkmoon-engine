#pragma once

#include <vector>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../utils/math.hpp"
#include "../utils/color.hpp"
#include "../resources/resource_shader.hpp"
#include "../managers/window.hpp"
#include "../managers/camera.hpp"

// Loads a GIF

struct AnimatedTexture {
private:
    struct Frame {
        GLuint textureID { 0 };
        float  delay     { 0.1f };  // seconds
    };

    std::vector<Frame> m_frames {};
    int                m_currentFrame { 0 };
    float              m_elapsed      { 0.f };

    bool  m_playing { true  };
    bool  m_looping { true  };
    float m_speed   { 1.f   };   // multiplier (>1 = faster)

    GLuint m_VAO {}, m_VBO {}, m_EBO {};

    Vector2Df m_position {};
    Vector2Df m_scale    { 1.f, 1.f };
    float     m_rotation { 0.f };
    Color     m_color    { WHITE };

    int m_frameWidth  { 0 };
    int m_frameHeight { 0 };

    Shader* m_shader {};
    Window* m_window {};

    std::string m_path {};

    // ------- //
    // Helpers //
    // ------- //
    void Rebuild();
    void LoadGif(const char* path);
    void FreeFrames();

    static std::array<float, 2> ToNDC(Vector2Df p, float winW, float winH) {
        return { ( p.x / winW) * 2.f - 1.f,
                -((p.y / winH) * 2.f - 1.f) };
    }

    // Upload one RGBA pixel buffer to a new GL texture and return its ID.
    static GLuint UploadTexture(const unsigned char* pixels, int w, int h);

    // Low-level draw using an arbitrary texture ID (avoids code duplication).
    void DrawWith(GLuint texID) const;
    void DrawWith(GLuint texID, const Camera2D& camera) const;

public:
    AnimatedTexture() = default;

    // texturePath: path to a GIF file (or any stb-supported image for a single-frame "animation").
    AnimatedTexture(Vector2Df position, Vector2Df scale, float rotation, const char* texturePath, Window* window, Shader* shader = nullptr);

    ~AnimatedTexture() { Delete(); }

    // Non-copyable (owns GL resources)
    AnimatedTexture(const AnimatedTexture&)            = delete;
    AnimatedTexture& operator=(const AnimatedTexture&) = delete;

    // Movable
    AnimatedTexture(AnimatedTexture&&) noexcept;
    AnimatedTexture& operator=(AnimatedTexture&&) noexcept;

    // ----------------- //
    // Animation control //
    // ----------------- //

    // Advance the animation by dt seconds. Call once per frame.
    void Update(float dt);

    void Play()  { m_playing = true;  }
    void Pause() { m_playing = false; }
    void Reset() { m_currentFrame = 0; m_elapsed = 0.f; m_playing = true; }

    // Jump to a specific frame index (clamped).
    void SetFrame(int index);

    // ------- //
    // Getters //
    // ------- //

    Vector2Df GetPosition()  const { return m_position; }
    Vector2Df GetScale()     const { return m_scale;    }
    float     GetRotation()  const { return m_rotation; }
    Color     GetColor()     const { return m_color;    }

    float     GetWidth()  const { return static_cast<float>(m_frameWidth)  * m_scale.x; }
    float     GetHeight() const { return static_cast<float>(m_frameHeight) * m_scale.y; }
    Vector2Df GetSize()   const { return { GetWidth(), GetHeight() }; }

    int  GetTextureWidth()  const { return m_frameWidth;  }
    int  GetTextureHeight() const { return m_frameHeight; }

    int  GetFrameCount()   const { return static_cast<int>(m_frames.size()); }
    int  GetCurrentFrame() const { return m_currentFrame; }
    bool IsPlaying()       const { return m_playing; }
    bool IsLooping()       const { return m_looping; }
    float GetSpeed()       const { return m_speed;   }

    // ------- //
    // Setters //
    // ------- //

    void SetPosition(Vector2Df position);
    void SetScale(Vector2Df scale);
    void SetRotation(float rotation);   // stored; rotation via transform matrix TODO
    void SetColor(Color color);

    void SetLooping(bool loop)    { m_looping = loop;  }
    void SetSpeed(float speed)    { m_speed   = speed; }

    // Hot-swap: load a new GIF keeping the current transform.
    void SetTexture(const char* texturePath);

    // -------- //
    // Commands //
    // -------- //

    void Delete();
    void Draw();
    void Draw(const Camera2D& camera);
};
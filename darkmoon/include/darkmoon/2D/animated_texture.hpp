#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../utils/math.hpp"
#include "../utils/color.hpp"
#include "../resources/resource_shader.hpp"
#include "../resources/resource_animated_texture.hpp"
#include "../managers/window.hpp"
#include "../managers/resource_manager.hpp"
#include "../managers/camera.hpp"

struct AnimatedTexture {
private:
    GLuint m_VAO {}, m_VBO {}, m_EBO {};

    int   m_currentFrame { 0 };
    float m_elapsed      { 0.f };

    bool  m_playing { true  };
    bool  m_looping { true  };
    float m_speed   { 1.f   };

    Vector2Df m_position {};
    Vector2Df m_scale    { 1.f, 1.f };
    float     m_rotation { 0.f };
    Color     m_color    { WHITE };

    Shader*                   m_shader   {};
    Window*                   m_window   {};
    ResourceAnimatedTexture*  m_resource {};

    ResourceManager& m_RM = ResourceManager::getInstance();

    void Rebuild();

    static std::array<float, 2> ToNDC(Vector2Df p, float winW, float winH) {
        return { ( p.x / winW) * 2.f - 1.f,
                -((p.y / winH) * 2.f - 1.f) };
    }

    void DrawWith(GLuint texID) const;
    void DrawWith(GLuint texID, const Camera2D& camera) const;

public:
    AnimatedTexture() = default;

    AnimatedTexture(Vector2Df position, Vector2Df scale, float rotation, const char* texturePath, Window* window, Shader* shader = nullptr);

    ~AnimatedTexture() { Delete(); }

    AnimatedTexture(const AnimatedTexture&)            = delete;
    AnimatedTexture& operator=(const AnimatedTexture&) = delete;

    AnimatedTexture(AnimatedTexture&&) noexcept;
    AnimatedTexture& operator=(AnimatedTexture&&) noexcept;

    // ----------------- //
    // Animation control //
    // ----------------- //

    void Update(float dt);

    void Play()  { m_playing = true;  }
    void Pause() { m_playing = false; }
    void Reset() { m_currentFrame = 0; m_elapsed = 0.f; m_playing = true; }

    void SetFrame(int index);

    // ------- //
    // Getters //
    // ------- //

    Vector2Df GetPosition() const { return m_position; }
    Vector2Df GetScale()    const { return m_scale;    }
    float     GetRotation() const { return m_rotation; }
    Color     GetColor()    const { return m_color;    }

    float     GetWidth()  const { return static_cast<float>(m_resource->getWidth())  * m_scale.x; }
    float     GetHeight() const { return static_cast<float>(m_resource->getHeight()) * m_scale.y; }
    Vector2Df GetSize()   const { return { GetWidth(), GetHeight() }; }

    int GetTextureWidth()  const { return m_resource->getWidth();  }
    int GetTextureHeight() const { return m_resource->getHeight(); }

    int   GetFrameCount()   const { return m_resource->getFrameCount(); }
    int   GetCurrentFrame() const { return m_currentFrame; }
    bool  IsPlaying()       const { return m_playing; }
    bool  IsLooping()       const { return m_looping; }
    float GetSpeed()        const { return m_speed;   }

    // ------- //
    // Setters //
    // ------- //

    void SetPosition(Vector2Df position);
    void SetScale(Vector2Df scale);
    void SetRotation(float rotation);
    void SetColor(Color color);

    void SetLooping(bool loop)  { m_looping = loop;  }
    void SetSpeed(float speed)  { m_speed   = speed; }

    void SetTexture(const char* texturePath);

    // -------- //
    // Commands //
    // -------- //

    void Delete();
    void Draw();
    void Draw(const Camera2D& camera);
};
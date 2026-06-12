#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../utils/math.hpp"
#include "../utils/color.hpp"
#include "../resources/resource_shader.hpp"
#include "../resources/resource_texture.hpp"
#include "../managers/window.hpp"
#include "../managers/resource_manager.hpp"
#include "../managers/camera.hpp"

struct Texture {
private:
    GLuint m_VAO {}, m_VBO {}, m_EBO {};

    Vector2Df        m_position {};
    Vector2Df        m_scale    { 1.f, 1.f };
    float            m_rotation { 0.f };
    Color            m_color    { WHITE };

    Shader*          m_shader          {};
    Window*          m_window          {};
    ResourceTexture* m_resourceTexture {};

    ResourceManager& m_RM = ResourceManager::getInstance();

    // Rebuild VBO from current state (position, scale, color)
    void Rebuild();

    static std::array<float, 2> ToNDC(Vector2Df p, float winW, float winH) {
        return { (p.x / winW) * 2.f - 1.f,
                -((p.y / winH) * 2.f - 1.f) };
    }

public:
    Texture() = default;

    Texture(Vector2Df position, Vector2Df scale, float rotation,
            const char* texturePath, Window* window, Shader* shader = nullptr);

    ~Texture() { Delete(); }

    // ------- //
    // Getters //
    // ------- //

    Vector2Df GetPosition()  const { return m_position; }
    Vector2Df GetScale()     const { return m_scale;    }
    float     GetRotation()  const { return m_rotation; }
    Color     GetColor()     const { return m_color;    }

    float GetWidth()  const;   // texture width  * scaleX
    float GetHeight() const;   // texture height * scaleY
    Vector2Df GetSize() const { return { GetWidth(), GetHeight() }; }

    // Raw texture dimensions (unscaled)
    int GetTextureWidth()  const;
    int GetTextureHeight() const;

    // ------- //
    // Setters //
    // ------- //

    void SetPosition(Vector2Df position);
    void SetScale(Vector2Df scale);
    void SetRotation(float rotation);   // TODO (transform matrix)
    void SetColor(Color color);

    // Hot-swap the texture keeping the same transform
    void SetTexture(const char* texturePath);

    // -------- //
    // Commands //
    // -------- //  

    void  SetOpacity(float opacity);
    float GetOpacity() const { return m_color.a / 255.f; }

    void Delete();
    void Draw();
    void Draw(const Camera2D& camera);
};
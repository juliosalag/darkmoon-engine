#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../utils/math.hpp"
#include "../utils/color.hpp"
#include "../resources/resource_shader.hpp"
#include "../resources/resource_texture.hpp"
#include "../managers/window.hpp"
#include "../managers/resource_manager.hpp"

struct Texture {
private:
    GLuint m_VAO {}, m_VBO {}, m_EBO {};
    Vector2D m_position {};
    Color m_color { WHITE };
    Vector2D m_scale { 1 , 1 };
    float m_rotation { 0 };
    Shader* m_shader {};
    Window* m_window {};
    ResourceTexture* m_resourceTexture {};

    ResourceManager& m_RM = ResourceManager::getInstance();

public:

    Texture(Vector2D position, Vector2D scale, float rotation, const char* texturePath, Window* window, Shader* shader)
        : m_position(position), m_scale(scale), m_rotation(rotation), m_shader(shader), m_window(window)
    {
        m_resourceTexture = m_RM.loadResource<ResourceTexture>(texturePath);
    }

};
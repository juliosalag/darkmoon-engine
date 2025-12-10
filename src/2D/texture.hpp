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

    Texture(Vector2D position, Vector2D scale, float rotation, const char* texturePath, Window* window, Shader* shader = nullptr)
        : m_position(position), m_scale(scale), m_rotation(rotation), m_shader(shader == nullptr ? window->GetBasicTextureShader2D() : shader), m_window(window)
    {
        m_resourceTexture = m_RM.loadResource<ResourceTexture>(texturePath);
    
        float vertex[] = {
            // Positions
            (static_cast<float>(m_position.x) / static_cast<float>(m_window->GetWidth())) * 2 - 1,
            -((static_cast<float>(m_position.y) / static_cast<float>(m_window->GetHeight())) * 2 - 1),
            // Color
            m_color.r/255.f, m_color.g/255.f, m_color.b/255.f,
            // Texture coords
            0.0f, 0.0f,
            // ---------------- //
            (static_cast<float>(m_position.x + m_resourceTexture->getWitdh() * static_cast<float>(m_scale.x)) / static_cast<float>(m_window->GetWidth())) * 2 - 1,
            -((static_cast<float>(m_position.y) / static_cast<float>(m_window->GetHeight())) * 2 - 1),
            m_color.r/255.f, m_color.g/255.f, m_color.b/255.f,
            1.0f, 0.0f,
            // ---------------- //
            (static_cast<float>(m_position.x) / static_cast<float>(m_window->GetWidth())) * 2 - 1,
            -((static_cast<float>(m_position.y + m_resourceTexture->getHeight() * static_cast<float>(m_scale.y)) / static_cast<float>(m_window->GetHeight())) * 2 - 1),
            m_color.r/255.f, m_color.g/255.f, m_color.b/255.f,
            0.0f, 1.0f,
            // ---------------- //
            (static_cast<float>(m_position.x + m_resourceTexture->getWitdh() * static_cast<float>(m_scale.x)) / static_cast<float>(m_window->GetWidth())) * 2 - 1,
            -((static_cast<float>(m_position.y + m_resourceTexture->getHeight() * static_cast<float>(m_scale.y)) / static_cast<float>(m_window->GetHeight())) * 2 - 1),
            m_color.r/255.f, m_color.g/255.f, m_color.b/255.f,
            1.0f, 1.0f,
        };
        GLuint index[] = { 0, 1, 2, 1, 2, 3 };

        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // texture coord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }

    ~Texture(){ Unload(); }

    void Unload(){
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
    }

    void Draw(){
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glUseProgram(m_shader->getIDShader());

        GLint colorUniform = glGetUniformLocation(m_shader->getIDShader(), "customColor");
        glUniform4fv(colorUniform, 1, glm::value_ptr(glm::vec4(m_color.r/255.f, m_color.g/255.f, m_color.b/255.f, m_color.a/255.f)));

        // Draw Texture
        glBindTexture(GL_TEXTURE_2D, m_resourceTexture->getIDTexture());
        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_BLEND);
    }

    void SetPosition(Vector2D position) {
        m_position = position;

        float vertex[] = {
            // Positions
            (static_cast<float>(m_position.x) / static_cast<float>(m_window->GetWidth())) * 2 - 1,
            -((static_cast<float>(m_position.y) / static_cast<float>(m_window->GetHeight())) * 2 - 1),
            // Color
            m_color.r/255.f, m_color.g/255.f, m_color.b/255.f,
            // Texture coords
            0.0f, 0.0f,
            // ---------------- //
            (static_cast<float>(m_position.x + m_resourceTexture->getWitdh() * static_cast<float>(m_scale.x)) / static_cast<float>(m_window->GetWidth())) * 2 - 1,
            -((static_cast<float>(m_position.y) / static_cast<float>(m_window->GetHeight())) * 2 - 1),
            m_color.r/255.f, m_color.g/255.f, m_color.b/255.f,
            1.0f, 0.0f,
            // ---------------- //
            (static_cast<float>(m_position.x) / static_cast<float>(m_window->GetWidth())) * 2 - 1,
            -((static_cast<float>(m_position.y + m_resourceTexture->getHeight() * static_cast<float>(m_scale.y)) / static_cast<float>(m_window->GetHeight())) * 2 - 1),
            m_color.r/255.f, m_color.g/255.f, m_color.b/255.f,
            0.0f, 1.0f,
            // ---------------- //
            (static_cast<float>(m_position.x + m_resourceTexture->getWitdh() * static_cast<float>(m_scale.x)) / static_cast<float>(m_window->GetWidth())) * 2 - 1,
            -((static_cast<float>(m_position.y + m_resourceTexture->getHeight() * static_cast<float>(m_scale.y)) / static_cast<float>(m_window->GetHeight())) * 2 - 1),
            m_color.r/255.f, m_color.g/255.f, m_color.b/255.f,
            1.0f, 1.0f,
        };

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex), vertex);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }   

    Vector2D GetPosition(){ return m_position; }
};
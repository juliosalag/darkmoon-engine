#pragma once

#include <glad/glad.h>
#include <string>

#include "../utils/math.hpp"
#include "../utils/color.hpp"
#include "../resources/resource_shader.hpp"
#include "../resources/resource_font.hpp"
#include "../managers/window.hpp"
#include "../managers/resource_manager.hpp"

struct Text {
private:
    GLuint m_VAO {}, m_VBO {};

    Vector2D m_position {};
    std::string m_text {};
    Color m_color { WHITE };
    float m_pixelHeight { 32.f };
    float m_scale { 1.0f };

    Window* m_window {};
    Shader* m_shader {};
    ResourceFont* m_resourceFont {};

    ResourceManager& m_RM = ResourceManager::getInstance();

    float toNDC_X(float px) const;
    float toNDC_Y(float py) const;

public:
    Text(Vector2D position, const std::string& text, const char* fontPath, Window* window, float pixelHeight = 32.f, Shader* shader = nullptr);
    ~Text() { Unload(); }

    void Draw();
    void Unload();

    void SetText(const std::string& text) { m_text = text; }
    std::string GetText() const { return m_text; }

    void SetScale(float scale) { m_scale = scale; }
    float GetScale() const { return m_scale; }

    void SetPosition(Vector2D position) { m_position = position; }
    Vector2D GetPosition() const { return m_position; }

    void SetColor(Color color) { m_color = color; }
    Color GetColor() { return m_color; }

    float MeasureText() const;
    float GetTextHeight() const;
};
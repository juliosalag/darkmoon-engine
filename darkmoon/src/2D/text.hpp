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
    Color m_color { WHITE };
    float m_pixelHeight { 32.f };

    Window* m_window {};
    Shader* m_shader {};
    ResourceFont* m_resourceFont {};

    ResourceManager& m_RM = ResourceManager::getInstance();

    float toNDC_X(float px) const;
    float toNDC_Y(float py) const;

public:
    Text(Vector2D position, const char* fontPath, float pixelHeight, Window* window, Shader* shader = nullptr);
    ~Text() { Unload(); }

    void Draw(const std::string& text);
    void Unload();

    void SetPosition(Vector2D position) { m_position = position; }
    void SetColor(Color color) { m_color = color; }
    void SetPixelHeight(float h) { m_pixelHeight = h; }
    Vector2D GetPosition() const { return m_position; }
    float MeasureText(const std::string& text) const;
};
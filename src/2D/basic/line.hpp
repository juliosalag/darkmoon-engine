#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../utils/math.hpp"
#include "../../utils/color.hpp"
#include "../../resources/resource_shader.hpp"
#include "../../managers/window.hpp"

struct Line {
private:
    GLuint m_VAO {}, m_VBO {};
    Vector2D m_startPosition {}, m_endPosition {};
    Color m_color { BLACK };
    int m_width { 1 };
    Shader* m_shader {};
    Window* m_window {};

public:
    Line(Vector2D startPosition, Vector2D endPosition, Color color, int width, Window* window, Shader* shader);
    ~Line();

    // Delete VAO and VBO buffers
    void Delete();

    // Draw line
    void Draw();

    // ------- //
    // Setters //
    // ------- //

    void SetPosition(Vector2D startPosition, Vector2D endPosition);
    void SetColor(Color color) { m_color = color; }
    void SetWidth(int width) { m_width = width; }

    // ------- //
    // Getters //
    // ------- //

    Vector2D GetStartPosition() { return m_startPosition; }
    Vector2D GetEndPosition() { return m_endPosition; }
    Color GetColor() { return m_color; }
    int GetWidth() { return m_width; }
};
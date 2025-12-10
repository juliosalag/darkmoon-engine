#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../utils/math.hpp"
#include "../../utils/color.hpp"
#include "../../resources/resource_shader.hpp"
#include "../../managers/window.hpp"

struct Pixel {
private:
    GLuint m_VAO {}, m_VBO {};
    Vector2D m_position {};
    Color m_color { BLACK };
    int m_size { 1 };
    Shader* m_shader {};
    Window* m_window {};

public:
    Pixel(Vector2D position, Color color, int size, Window* window, Shader* shader);
    ~Pixel();

    // Delete VAO and VBO buffers
    void Delete();

    // Draw pixel
    void Draw();

    // ------- //
    // Setters //
    // ------- //

    void SetPosition(Vector2D position);
    void SetColor(Color color) { m_color = color; }
    void SetSize(int size) { m_size = size; }

    // ------- //
    // Getters //
    // ------- //

    Vector2D GetPosition() { return m_position; }
    Color GetColor() { return m_color; }
    int GetSize() { return m_size; }
};


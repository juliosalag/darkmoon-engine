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
    Vector2Df m_position {};
    Color m_color { BLACK };
    int m_size { 1 };
    Shader* m_shader {};
    Window* m_window {};

public:
    Pixel() = default;

    // Constructor: initializes pixel data and OpenGL buffers
    Pixel(Vector2Df position, Color color, int size, Window* window, Shader* shader = nullptr);

    // Destructor: cleans up GPU resources
    ~Pixel();

    // Deletes VAO and VBO buffers from GPU
    void Delete();

    // Renders the pixel on screen
    void Draw();

    // ------- //
    // Setters //
    // ------- //

    // Updates pixel position and GPU buffer data
    void SetPosition(Vector2Df position);

    // Sets pixel color
    void SetColor(Color color) { m_color = color; }

    // Sets pixel size
    void SetSize(int size) { m_size = size; }

    // Sets shader
    void SetShader(Shader* shader){ m_shader = shader; }

    // ------- //
    // Getters //
    // ------- //

    Vector2Df GetPosition() { return m_position; }
    Color     GetColor()    { return m_color; }
    int       GetSize()     { return m_size; }
    Shader*   GetShader()   { return m_shader; }
};
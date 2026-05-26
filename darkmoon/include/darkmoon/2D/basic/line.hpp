#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../utils/math.hpp"
#include "../../utils/color.hpp"
#include "../../resources/resource_shader.hpp"
#include "../../managers/window.hpp"
#include "../../managers/camera.hpp"

struct Line {
private:
    GLuint m_VAO {}, m_VBO {};
    Vector2Df m_startPosition {}, m_endPosition {};
    Color m_color { BLACK };
    int m_width { 1 };
    Shader* m_shader {};
    Window* m_window {};

public:
    Line() = default;

    // Constructor: initializes line data and OpenGL buffers
    Line(Vector2Df startPosition, Vector2Df endPosition, Color color, int width, Window* window, Shader* shader = nullptr);

    // Destructor: cleans up GPU resources
    ~Line();

    // Deletes VAO and VBO buffers from GPU
    void Delete();

    // Renders the line on screen
    void Draw();
    void Draw(const Camera2D& camera);

    // ------- //
    // Setters //
    // ------- //

    // Updates both start and end positions and GPU buffer data
    void SetPosition(Vector2Df startPosition, Vector2Df endPosition);

    // Sets line color
    void SetColor(Color color) { m_color = color; }

    // Sets line width
    void SetWidth(int width) { m_width = width; }

    // Sets shader
    void SetShader(Shader* shader){ m_shader = shader; }

    // ------- //
    // Getters //
    // ------- //

    Vector2Df GetStartPosition() { return m_startPosition; }
    Vector2Df GetEndPosition()   { return m_endPosition; }
    Color     GetColor()         { return m_color; }
    int       GetWidth()         { return m_width; }
    Shader*   GetShader()        { return m_shader; }

};
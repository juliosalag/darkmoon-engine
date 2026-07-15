#pragma once

#include <vector>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../utils/math.hpp"
#include "../utils/color.hpp"
#include "../resources/resource_shader.hpp"
#include "../managers/window.hpp"
#include "../managers/camera.hpp"

// ------ //
// Circle //
// ------ //

struct Circle {
private:
    GLuint m_VAO {}, m_VBO {};

    Vector2Df m_center {};
    float     m_radius { 1.0f };
    int       m_segments { 64 };
    Color     m_color { WHITE };
    Shader*   m_shader {};
    Window*   m_window {};

    void Rebuild();

    static float toNDC_X(float px, float winW) { return (px / winW) * 2.f - 1.f; }
    static float toNDC_Y(float py, float winH) { return -((py / winH) * 2.f - 1.f); }

public:
    Circle() = default;
    Circle(Vector2Df center, float radius, Color color, Window* window, int segments = 64, Shader* shader = nullptr);
    ~Circle() { Delete(); }

    // ------- //
    // Getters //
    // ------- //

    Vector2Df GetCenter()   const { return m_center;   }
    float     GetRadius()   const { return m_radius;   }
    int       GetSegments() const { return m_segments; }
    Color     GetColor()    const { return m_color;    }

    // ------- //
    // Setters //
    // ------- //

    void SetCenter(Vector2Df center);
    void SetRadius(float radius);
    void SetSegments(int segments);
    void SetColor(Color color) { m_color = color; }

    // -------- //
    // Commands //
    // -------- //

    void Draw();
    void Draw(const Camera2D& camera);
    void Delete();
};

// ----------- //
// CircleLines //
// ----------- //

struct CircleLines {
private:
    GLuint m_VAO {}, m_VBO {};

    Vector2Df m_center {};
    float     m_radius { 1.0f };
    int       m_segments { 64 };
    float     m_lineWidth { 1.0f };
    Color     m_color { WHITE };
    Shader*   m_shader {};
    Window*   m_window {};

    void Rebuild();

    static float toNDC_X(float px, float winW) { return (px / winW) * 2.f - 1.f; }
    static float toNDC_Y(float py, float winH) { return -((py / winH) * 2.f - 1.f); }

public:
    CircleLines() = default;
    CircleLines(Vector2Df center, float radius, Color color, Window* window, int segments = 64, float lineWidth = 1.0f, Shader* shader = nullptr);
    ~CircleLines() { Delete(); }

    // ------- //
    // Getters //
    // ------- //

    Vector2Df GetCenter()    const { return m_center;    }
    float     GetRadius()    const { return m_radius;    }
    int       GetSegments()  const { return m_segments;  }
    float     GetLineWidth() const { return m_lineWidth; }
    Color     GetColor()     const { return m_color;     }

    // ------- //
    // Setters //
    // ------- //

    void SetCenter(Vector2Df center);
    void SetRadius(float radius);
    void SetSegments(int segments);
    void SetLineWidth(float width) { m_lineWidth = width; }
    void SetColor(Color color)     { m_color = color;     }

    // -------- //
    // Commands //
    // -------- //

    void Draw();
    void Draw(const Camera2D& camera);
    void Delete();
};
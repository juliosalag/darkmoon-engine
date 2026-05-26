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

// -------------- //
// RegularPolygon //
// -------------- //

struct RegularPolygon {
private:
    GLuint m_VAO {}, m_VBO {};

    Vector2Df m_center {};
    float     m_radius   { 1.0f };
    int       m_sides    { 6 };
    float     m_rotation { 0.0f }; // Grados
    Color     m_color    { WHITE };
    Shader*   m_shader   {};
    Window*   m_window   {};

    void Rebuild();

    static float toNDC_X(float px, float winW) { return (px / winW) * 2.f - 1.f; }
    static float toNDC_Y(float py, float winH) { return -((py / winH) * 2.f - 1.f); }

public:
    RegularPolygon() = default;
    RegularPolygon(Vector2Df center, float radius, int sides, Color color, Window* window, float rotation = -1000.0f, Shader* shader = nullptr);
    ~RegularPolygon() { Delete(); }

    // ------- //
    // Getters //
    // ------- //

    Vector2Df GetCenter()   const { return m_center;   }
    float     GetRadius()   const { return m_radius;   }
    int       GetSides()    const { return m_sides;    }
    float     GetRotation() const { return m_rotation; }
    Color     GetColor()    const { return m_color;    }

    // ------- //
    // Setters //
    // ------- //

    void SetCenter(Vector2Df center);
    void SetRadius(float radius);
    void SetSides(int sides);
    void SetRotation(float degrees);
    void SetColor(Color color) { m_color = color; }

    // -------- //
    // Commands //
    // -------- //

    void Draw();
    void Draw(const Camera2D& camera);
    void Delete();
};

// ------------------- //
// RegularPolygonLines //
// ------------------- //

struct RegularPolygonLines {
private:
    GLuint m_VAO {}, m_VBO {};

    Vector2Df m_center {};
    float     m_radius    { 1.0f };
    int       m_sides     { 6 };
    float     m_rotation  { 0.0f };
    float     m_lineWidth { 1.0f };
    Color     m_color     { WHITE };
    Shader*   m_shader    {};
    Window*   m_window    {};

    void Rebuild();

    static float toNDC_X(float px, float winW) { return (px / winW) * 2.f - 1.f; }
    static float toNDC_Y(float py, float winH) { return -((py / winH) * 2.f - 1.f); }

public:
    RegularPolygonLines() = default;
    RegularPolygonLines(Vector2Df center, float radius, int sides, Color color, Window* window, float rotation = -1000.0f, float lineWidth = 1.0f, Shader* shader = nullptr);
    ~RegularPolygonLines() { Delete(); }

    // ------- //
    // Getters //
    // ------- //

    Vector2Df GetCenter()    const { return m_center;    }
    float     GetRadius()    const { return m_radius;    }
    int       GetSides()     const { return m_sides;     }
    float     GetRotation()  const { return m_rotation;  }
    float     GetLineWidth() const { return m_lineWidth; }
    Color     GetColor()     const { return m_color;     }

    // ------- //
    // Setters //
    // ------- //

    void SetCenter(Vector2Df center);
    void SetRadius(float radius);
    void SetSides(int sides);
    void SetRotation(float degrees);
    void SetLineWidth(float width) { m_lineWidth = width; }
    void SetColor(Color color)     { m_color = color;     }

    // -------- //
    // Commands //
    // -------- //

    void Draw();
    void Draw(const Camera2D& camera);
    void Delete();
};
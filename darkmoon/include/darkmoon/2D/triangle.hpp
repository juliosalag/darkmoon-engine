#pragma once

#include "../utils/color.hpp"
#include "../resources/resource_shader.hpp"
#include "../managers/camera.hpp"
#include "./basic/line.hpp"
#include <darkmoon/utils/math.hpp>

// -------- //
// Triangle //
// -------- //

struct Triangle {
private:
    GLuint    m_VAO {}, m_VBO {}, m_EBO {};
    Vector2Df  m_vertexA {}, m_vertexB {}, m_vertexC {};
    Color     m_color  { BLACK };
    Shader*   m_shader {};
    Window*   m_window {};

    void Rebuild();

    static std::array<float, 2> ToNDC(Vector2Df p, float winW, float winH) {
        return { (static_cast<float>(p.x) / winW) * 2.f - 1.f,
                -((static_cast<float>(p.y) / winH) * 2.f - 1.f) };
    }

public:
    Triangle(Vector2Df vertexA, Vector2Df vertexB, Vector2Df vertexC,
             Color color, Window* window, Shader* shader = nullptr);

    ~Triangle() { Delete(); }

    // ------- //
    // Getters //
    // ------- //

    Vector2Df GetVertexA() const { return m_vertexA; }
    Vector2Df GetVertexB() const { return m_vertexB; }
    Vector2Df GetVertexC() const { return m_vertexC; }
    Color    GetColor()   const { return m_color;   }

    // ------- //
    // Setters //
    // ------- //

    // Desplaza los tres vértices manteniendo la forma, usando A como origen
    void SetPosition(Vector2Df position);
    void SetVertexA(Vector2Df v);
    void SetVertexB(Vector2Df v);
    void SetVertexC(Vector2Df v);
    void SetColor(Color color) { m_color = color; }

    // -------- //
    // Commands //
    // -------- //

    void Draw();
    void Draw(const Camera2D& camera);
    void Delete();
};

// ------------- //
// TriangleLines //
// ------------- //

struct TriangleLines {
private:
    Vector2Df  m_vertexA {}, m_vertexB {}, m_vertexC {};
    Color     m_color  { BLACK };
    int       m_lineWidth {};
    Window*   m_window {};
    Shader*   m_shader {};

    Line m_edgeAB, m_edgeBC, m_edgeCA;

    void Rebuild();

public:
    TriangleLines(Vector2Df vertexA, Vector2Df vertexB, Vector2Df vertexC,
                  Color color, int width, Window* window, Shader* shader = nullptr);

    ~TriangleLines() { Delete(); }

    // ------- //
    // Getters //
    // ------- //

    Vector2Df GetVertexA() const { return m_vertexA; }
    Vector2Df GetVertexB() const { return m_vertexB; }
    Vector2Df GetVertexC() const { return m_vertexC; }
    Color    GetColor()   const { return m_color;   }

    Line* GetEdgeAB() { return &m_edgeAB; }
    Line* GetEdgeBC() { return &m_edgeBC; }
    Line* GetEdgeCA() { return &m_edgeCA; }

    // ------- //
    // Setters //
    // ------- //

    void SetPosition(Vector2Df position);
    void SetVertexA(Vector2Df v);
    void SetVertexB(Vector2Df v);
    void SetVertexC(Vector2Df v);
    void SetColor(Color color) { m_color = color; Rebuild(); }

    // -------- //
    // Commands //
    // -------- //

    void Draw();
    void Draw(const Camera2D& camera);
    void Delete();
};
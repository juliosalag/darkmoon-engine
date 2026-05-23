#pragma once

#include "../utils/color.hpp"
#include "../resources/resource_shader.hpp"
#include "./basic/line.hpp"
#include <darkmoon/utils/math.hpp>

// 
// A --- B 
// |     |
// C --- D
//

struct Rectangle {
private:
    GLuint m_VAO {}, m_VBO {}, m_EBO {};
    Vector2Df m_vertexA {}, m_vertexB {}, m_vertexC {}, m_vertexD {};
    Color m_color { BLACK };
    Shader* m_shader {};
    Window* m_window {};

    void Rebuild();

public:
    Rectangle() = default;

    Rectangle(Vector2Df position, float width, float height, Color color, Window* window, Shader* shader = nullptr);
    Rectangle(Vector2Df vertexA, Vector2Df vertexB, Vector2Df vertexC, Vector2Df vertexD, Color color, Window* window, Shader* shader = nullptr);

    ~Rectangle(){ Delete(); };

    // ------- //
    // Getters //
    // ------- //

    Vector2Df GetVertexA() const { return m_vertexA; }
    Vector2Df GetVertexB() const { return m_vertexB; }
    Vector2Df GetVertexC() const { return m_vertexC; }
    Vector2Df GetVertexD() const { return m_vertexD; }

    // Position = vertex A (top-left)
    Vector2Df GetPosition() const { return m_vertexA; }
    Color GetColor(){ return m_color; }

    float GetWidth()  const { return m_vertexB.x - m_vertexA.x; }
    float GetHeight() const { return m_vertexC.y - m_vertexA.y; }
    Vector2Df GetSize() const { return { GetWidth(), GetHeight() }; }

    // ------- //
    // Setters //
    // ------- //

    // Move the whole rectangle so that vertex A lands on position
    void SetPosition(Vector2Df position);

    // Set individual vertices and rebuild
    void SetVertexA(Vector2Df v);
    void SetVertexB(Vector2Df v);
    void SetVertexC(Vector2Df v);
    void SetVertexD(Vector2Df v);

    // Resize keeping vertex A fixed
    void SetWidth(float width);
    void SetHeight(float height);
    void SetSize(float width, float height);
    void SetSize(Vector2Df size);

    // Set color
    void SetColor(Color color){ m_color = color; }

    // -------- //
    // Commands //
    // -------- //

    void Delete();
    void Draw();
};

struct RectangleLines {
private:

    // Store raw data so we can rebuild edges on change
    Vector2Df m_vertexA {}, m_vertexB {}, m_vertexC {}, m_vertexD {};
    Color     m_color  {};
    int       m_width  {};
    Window*   m_window {};
    Shader*   m_shader {};

    Line m_edgeAB, m_edgeBD, m_edgeCD, m_edgeCA;
    
    void Rebuild();

public:
    RectangleLines(Vector2Df position, float width, float height, Color color, int lineWidth, Window* window, Shader* shader = nullptr);
    RectangleLines(Vector2Df vertexA, Vector2Df vertexB, Vector2Df vertexC, Vector2Df vertexD, Color color, int width, Window* window, Shader* shader = nullptr);

    ~RectangleLines(){ Delete(); };

    // ------- //
    // Getters //
    // ------- //

    Vector2Df GetVertexA() const { return m_vertexA; }
    Vector2Df GetVertexB() const { return m_vertexB; }
    Vector2Df GetVertexC() const { return m_vertexC; }
    Vector2Df GetVertexD() const { return m_vertexD; }

    Vector2Df GetPosition() const { return m_vertexA; }
    Color GetColor(){ return m_color; }

    float GetWidth()  const { return m_vertexB.x - m_vertexA.x; }
    float GetHeight() const { return m_vertexC.y - m_vertexA.y; }
    Vector2Df GetSize() const { return { GetWidth(), GetHeight() }; }

    // ------- //
    // Setters //
    // ------- //

    void SetPosition(Vector2Df position);

    void SetVertexA(Vector2Df v);
    void SetVertexB(Vector2Df v);
    void SetVertexC(Vector2Df v);
    void SetVertexD(Vector2Df v);

    void SetWidth(float width);
    void SetHeight(float height);
    void SetSize(float width, float height);
    void SetSize(Vector2Df size);

    // Set color
    void SetColor(Color color){ m_edgeAB.SetColor(color); m_edgeBD.SetColor(color); m_edgeCD.SetColor(color); m_edgeCA.SetColor(color); }

    // -------- //
    // Commands //
    // -------- //

    void Delete();
    void Draw();

    Line* GetEdgeAB(){ return &m_edgeAB; };
    Line* GetEdgeBD(){ return &m_edgeBD; };
    Line* GetEdgeCD(){ return &m_edgeCD; };
    Line* GetEdgeCA(){ return &m_edgeCA; };
};
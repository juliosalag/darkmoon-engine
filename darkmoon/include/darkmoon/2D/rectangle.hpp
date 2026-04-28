#pragma once

#include "../utils/color.hpp"
#include "../resources/resource_shader.hpp"
#include "./basic/line.hpp"
#include <darkmoon/utils/math.hpp>

struct Rectangle {
private:
    GLuint m_VAO {}, m_VBO {}, m_EBO {};
    Vector2D m_vertexA {}, m_vertexB {}, m_vertexC {}, m_vertexD {};
    Color m_color { BLACK };
    Shader* m_shader {};
    Window* m_window {};

public:
    Rectangle() = default;

    Rectangle(Vector2D vertexA, Vector2D vertexB, Vector2D vertexC, Vector2D vertexD, Color color, Window* window, Shader* shader = nullptr)
        : m_vertexA(vertexA), m_vertexB(vertexB), m_vertexC(vertexC), m_vertexD(vertexD), m_color(color), m_shader(shader == nullptr ? window->GetBasicShader2D() : shader), m_window(window)
    {
        float vertex[] = {
            (static_cast<float>(m_vertexA.x) / static_cast<float>(m_window->GetWidth())) * 2 - 1,
            -((static_cast<float>(m_vertexA.y) / static_cast<float>(m_window->GetHeight())) * 2 - 1),
            (static_cast<float>(m_vertexB.x) / static_cast<float>(m_window->GetWidth())) * 2 - 1,
            -((static_cast<float>(m_vertexB.y) / static_cast<float>(m_window->GetHeight())) * 2 - 1),
            (static_cast<float>(m_vertexC.x) / static_cast<float>(m_window->GetWidth())) * 2 - 1,
            -((static_cast<float>(m_vertexC.y) / static_cast<float>(m_window->GetHeight())) * 2 - 1),
            (static_cast<float>(m_vertexD.x) / static_cast<float>(m_window->GetWidth())) * 2 - 1,
            -((static_cast<float>(m_vertexD.y) / static_cast<float>(m_window->GetHeight())) * 2 - 1),
        };
        GLuint index[] = { 0, 1, 2, 1, 2, 3 };

        // 
        // A --- B 
        // |     |
        // C --- D
        //

        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    };

    ~Rectangle(){ Delete(); };

    Vector2D GetVertexA(){ return m_vertexA; }
    Vector2D GetVertexB(){ return m_vertexB; }
    Vector2D GetVertexC(){ return m_vertexC; }
    Vector2D GetVertexD(){ return m_vertexD; }

    void Delete() {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
    }

    void Draw(){
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glUseProgram(m_shader->getIDShader());

        GLint colorUniform = glGetUniformLocation(m_shader->getIDShader(), "customColor");
        glUniform4fv(colorUniform, 1, glm::value_ptr(glm::vec4(m_color.r/255.f, m_color.g/255.f, m_color.b/255.f, m_color.a/255.f)));

        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glDisable(GL_BLEND);
    }

};

struct RectangleLines {
private:
    Line m_edgeAB, m_edgeBD, m_edgeCD, m_edgeCA; 

public:
    RectangleLines(Vector2D vertexA, Vector2D vertexB, Vector2D vertexC, Vector2D vertexD, Color color, int width, Window* window, Shader* shader = nullptr)
        : m_edgeAB(vertexA, vertexB, color, width, window, shader),
          m_edgeBD(vertexB, vertexD, color, width, window, shader),
          m_edgeCD(vertexC, vertexD, color, width, window, shader),
          m_edgeCA(vertexC, vertexA, color, width, window, shader) {};

    ~RectangleLines(){ Delete(); };

    void Delete(){
        m_edgeAB.Delete();
        m_edgeBD.Delete();
        m_edgeCD.Delete();
        m_edgeCA.Delete();
    }

    void Draw(){
        m_edgeAB.Draw();
        m_edgeBD.Draw();
        m_edgeCD.Draw();
        m_edgeCA.Draw();
    }

    Line* GetEdgeAB(){ return &m_edgeAB; };
    Line* GetEdgeBD(){ return &m_edgeBD; };
    Line* GetEdgeCD(){ return &m_edgeCD; };
    Line* GetEdgeCA(){ return &m_edgeCA; };
};
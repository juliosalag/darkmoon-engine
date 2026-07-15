#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <array>

#include "../utils/math.hpp"
#include "../utils/color.hpp"
#include "../resources/resource_shader.hpp"
#include "../resources/resource_texture.hpp"
#include "../managers/window.hpp"
#include "../managers/resource_manager.hpp"
#include "../managers/camera.hpp"

struct TileSet {
private:
    int   m_tileW   {};
    int   m_tileH   {};
    int   m_cols    {};
    int   m_rows    {};

    Vector2Df m_scale    { 1.f, 1.f };
    float     m_rotation { 0.f };
    Color     m_color    { WHITE };

    Window* m_window          {};
    Shader* m_shader          {};
    ResourceTexture* m_resourceTexture {};
    ResourceManager& m_RM = ResourceManager::getInstance();

    unsigned int m_vao {0};
    unsigned int m_vbo {0};
    unsigned int m_ebo {0};
    int          m_colorUniformLoc {-1};

    static std::array<float, 2> ToNDC(Vector2Df p, float winW, float winH) {
        return { (p.x / winW) * 2.f - 1.f,
                -((p.y / winH) * 2.f - 1.f) };
    }

    void DrawQuad(Vector2Df screenTL, Vector2Df screenBR, float uvX0, float uvY0, float uvX1, float uvY1, float winW, float winH) const;
    void IndexToXY(int index, int& outCol, int& outRow) const;
    
    void InitGLBuffers();

public:
    TileSet() = default;
    TileSet(int tileW, int tileH, int cols, int rows, const char* texturePath, Window* window, Shader* shader = nullptr);
    ~TileSet(); 

    // Getters
    int       GetTileW()    const { return m_tileW;    }
    int       GetTileH()    const { return m_tileH;    }
    int       GetCols()     const { return m_cols;     }
    int       GetRows()     const { return m_rows;     }
    int       GetCount()    const { return m_cols * m_rows; }
    Vector2Df GetScale()    const { return m_scale;    }
    float     GetRotation() const { return m_rotation; }
    Color     GetColor()    const { return m_color;    }

    float GetTileWidth()  const { return static_cast<float>(m_tileW) * m_scale.x; }
    float GetTileHeight() const { return static_cast<float>(m_tileH) * m_scale.y; }

    // Setters
    void SetScale(Vector2Df scale)   { m_scale    = scale;    }
    void SetRotation(float rotation) { m_rotation = rotation; }
    void SetColor(Color color)       { m_color    = color;    }
    void SetTexture(const char* path);

    // Commands
    void Draw(int col, int row, Vector2Df pos) const;
    void Draw(int index, Vector2Df pos) const;
    void Draw(int col, int row, Vector2Df pos, const Camera2D& camera) const;
    void Draw(int index,        Vector2Df pos, const Camera2D& camera) const;
};
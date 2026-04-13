#pragma once

#include <glad/glad.h>
#include <array>

#include "resource.hpp"

struct GlyphInfo {
    float u0, v0, u1, v1;
    float bearingX, bearingY;
    float advance;
    float width, height;
};

struct ResourceFont : public Resource {
public:
    static constexpr int FIRST_CHAR  = 32;
    static constexpr int CHAR_COUNT  = 96;
    static constexpr int ATLAS_W     = 512;
    static constexpr int ATLAS_H     = 512;

    ResourceFont(std::size_t idResource, std::size_t fileType, const char* filePath, float pixelHeight = 32.0f);
    ~ResourceFont() { unload(); }

    void unload() override;

    GLuint atlasID { 0 };
    float pixelHeight { 32.0f };
    std::array<GlyphInfo, CHAR_COUNT> glyphs {};

private:
    void setup() override {}
};
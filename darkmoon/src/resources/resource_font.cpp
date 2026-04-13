#include "resource_font.hpp"

#pragma GCC diagnostic ignored "-Wconversion"
#define STB_TRUETYPE_IMPLEMENTATION
#include "../libs/stb_truetype.h"
#pragma GCC diagnostic warning "-Wconversion"

#include <fstream>
#include <vector>

ResourceFont::ResourceFont(std::size_t idResource, std::size_t fileType, const char* filePath, float pixelHeight)
    : Resource(idResource, fileType, filePath), pixelHeight(pixelHeight)
{
    // 1. Read file .ttf in memory
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) { 
        m_isLoaded = false;  
        std::cout << "[ERROR] Could not open file \"" << filePath << "\"\n";
        return;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<unsigned char> ttfBuffer(size);
    if (!file.read(reinterpret_cast<char*>(ttfBuffer.data()), size)) {
        m_isLoaded = false;
        std::cout << "[ERROR] Failed to read file \"" << filePath << "\"\n";
        return;
    }

    stbtt_fontinfo fontInfo;
    stbtt_InitFont(&fontInfo, ttfBuffer.data(), 0);

    int iAscent, iDescent, iLineGap;
    stbtt_GetFontVMetrics(&fontInfo, &iAscent, &iDescent, &iLineGap);

    float scale = stbtt_ScaleForPixelHeight(&fontInfo, pixelHeight);
    ascent  =  static_cast<float>(iAscent)  * scale;
    descent =  static_cast<float>(iDescent) * scale;
    lineGap =  static_cast<float>(iLineGap) * scale;

    // 2. Generate bitmap atlas
    std::vector<unsigned char> bitmap(ATLAS_W * ATLAS_H, 0);
    stbtt_bakedchar bakedChars[CHAR_COUNT];

    int result = stbtt_BakeFontBitmap(
        ttfBuffer.data(), 0,
        pixelHeight,
        bitmap.data(), ATLAS_W, ATLAS_H,
        FIRST_CHAR, CHAR_COUNT,
        bakedChars
    );

    if (result == 0) {
        m_isLoaded = false;
        std::cout << "[ERROR] Atlas (" << ATLAS_W << "x" << ATLAS_H << ") too small for \"" << filePath << "\" at " << pixelHeight << "px — increase ATLAS_W/ATLAS_H\n";
        return;
    }

    // 3. Load atlas as OpenGL texture (one channel: GL_RED)
    glGenTextures(1, &atlasID);
    glBindTexture(GL_TEXTURE_2D, atlasID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, ATLAS_W, ATLAS_H, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);

    // 4. Convert stbtt_bakedchar to GlyphInfo
    for (int i = 0; i < CHAR_COUNT; ++i) {
        const auto& bc = bakedChars[i];
        auto& g = glyphs[i];

        g.u0 = bc.x0 / (float)ATLAS_W;
        g.v0 = bc.y0 / (float)ATLAS_H;
        g.u1 = bc.x1 / (float)ATLAS_W;
        g.v1 = bc.y1 / (float)ATLAS_H;

        g.width    = (float)(bc.x1 - bc.x0);
        g.height   = (float)(bc.y1 - bc.y0);
        g.bearingX = bc.xoff;
        g.bearingY = bc.yoff;
        g.advance  = bc.xadvance;
    }

    std::cout << "[LOAD] Font: \"" << filePath << "\" | ID: " << atlasID << "\n";
    m_isLoaded = true;
}

void ResourceFont::unload() {
    if (atlasID) {
        std::cout << "[UNLOAD] Font: \"" << getFilePath() << "\" | ID " << atlasID << "\n";
        glDeleteTextures(1, &atlasID);
        atlasID = 0;
    }

    m_isLoaded = false;
}
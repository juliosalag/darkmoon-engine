#include "resource_font.hpp"

#pragma GCC diagnostic ignored "-Wconversion"
#define STB_TRUETYPE_IMPLEMENTATION
#include "../libs/stb_truetype.h"
#pragma GCC diagnostic warning "-Wconversion"

#include <fstream>
#include <vector>

static const int RANGES[][2] = {
    { 0x0020, 0x007E },   // ASCII
    { 0x00A0, 0x00FF },   // Latin-1 Supplement
    { 0x0100, 0x017F },   // Latin Extended-A
};

ResourceFont::ResourceFont(std::size_t idResource, std::size_t fileType, const char* filePath, float pixelHeight)
    : Resource(idResource, fileType, filePath), pixelHeight(pixelHeight)
{
    // 1. Read .ttf file into memory
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        m_isLoaded = false;
        std::cout << "[ERROR] ResourceFont: could not open file \"" << filePath << "\"\n";
        return;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<unsigned char> ttfBuffer(size);
    if (!file.read(reinterpret_cast<char*>(ttfBuffer.data()), size)) {
        m_isLoaded = false;
        std::cout << "[ERROR] ResourceFont: failed to read file \"" << filePath << "\"\n";
        return;
    }

    // 2. Extract font vertical metrics (ascent, descent, line gap)
    stbtt_fontinfo fontInfo;
    stbtt_InitFont(&fontInfo, ttfBuffer.data(), 0);

    int iAscent, iDescent, iLineGap;
    stbtt_GetFontVMetrics(&fontInfo, &iAscent, &iDescent, &iLineGap);
    float scale = stbtt_ScaleForPixelHeight(&fontInfo, pixelHeight);

    ascent = static_cast<float>(iAscent) * scale;
    descent = static_cast<float>(iDescent) * scale;
    lineGap = static_cast<float>(iLineGap) * scale;

    // 3. Count total codepoints across all ranges
    int totalGlyphs = 0;
    for (auto& range : RANGES)
        totalGlyphs += range[1] - range[0] + 1;

    // 4. Allocate atlas bitmap and packed char buffer
    std::vector<unsigned char> bitmap(ATLAS_W * ATLAS_H, 0);
    std::vector<stbtt_packedchar> packedChars(totalGlyphs);

    stbtt_pack_context packCtx;
    if (!stbtt_PackBegin(&packCtx, bitmap.data(), ATLAS_W, ATLAS_H, 0, 1, nullptr)) {
        m_isLoaded = false;
        std::cout << "[ERROR] ResourceFont: stbtt_PackBegin failed for \"" << filePath << "\"\n";
        return;
    }

    stbtt_PackSetOversampling(&packCtx, 1, 1);

    // 5. Prepare each codepoint range into the atlas
    int offset = 0;
    for (auto& range : RANGES) {
        int count = range[1] - range[0] + 1;
        stbtt_pack_range pr;
        pr.font_size = pixelHeight;
        pr.first_unicode_codepoint_in_range = range[0];
        pr.array_of_unicode_codepoints = nullptr;
        pr.num_chars = count;
        pr.chardata_for_range = packedChars.data() + offset;
        stbtt_PackFontRanges(&packCtx, ttfBuffer.data(), 0, &pr, 1);
        offset += count;
    }

    stbtt_PackEnd(&packCtx);

    // 6. Upload atlas bitmap to GPU as a single-channel (GL_RED) texture
    glGenTextures(1, &atlasID);
    glBindTexture(GL_TEXTURE_2D, atlasID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, ATLAS_W, ATLAS_H, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);

    // 7. Convert packedchar data to GlyphInfo and store in the codepoint map
    offset = 0;
    for (auto& range : RANGES) {
        int count = range[1] - range[0] + 1;
        for (int i = 0; i < count; ++i) {
            const auto& pc = packedChars[offset + i];
            GlyphInfo g;
            g.u0 = pc.x0 / (float)ATLAS_W;
            g.v0 = pc.y0 / (float)ATLAS_H;
            g.u1 = pc.x1 / (float)ATLAS_W;
            g.v1 = pc.y1 / (float)ATLAS_H;
            g.width    = (float)(pc.x1 - pc.x0);
            g.height   = (float)(pc.y1 - pc.y0);
            g.bearingX = pc.xoff;
            g.bearingY = pc.yoff;
            g.advance  = pc.xadvance;
            glyphs[range[0] + i] = g;
        }
        offset += count;
    }

    m_isLoaded = true;
    std::cout << "[LOAD] ResourceFont: \"" << filePath << " | ID: " << atlasID << "\n";
}

void ResourceFont::unload() {
    if (atlasID) {
        std::cout << "[UNLOAD] ResourceFont: \"" << getFilePath() << "\" | ID " << atlasID << "\n";
        glDeleteTextures(1, &atlasID);
        atlasID = 0;
    }
    m_isLoaded = false;
}
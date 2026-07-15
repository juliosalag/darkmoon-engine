#include "../libs/stb_image.h"

#include <glad/glad.h>
#include <darkmoon/resources/resource_animated_texture.hpp>

#include <cstdio>
#include <vector>
#include <iostream>

// Centiseconds → seconds. GIF spec stores delays in 1/100 s units.
// Browsers treat 0 or 1 cs as 10 cs (100 ms) - mirror that behaviour.
static float centiToSec(int cs) {
    return (cs <= 1 ? 10 : cs) / 100.f;
}

static GLuint uploadFrame(const unsigned char* pixels, int w, int h) {
    GLuint id = 0;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    glBindTexture(GL_TEXTURE_2D, 0);
    return id;
}

ResourceAnimatedTexture::ResourceAnimatedTexture(std::size_t idResource, std::size_t fileType, const char* filePath)
    : Resource(idResource, fileType, filePath)
{
    // ------------------------ //
    // 1. Read file into memory //
    // ------------------------ //
    FILE* f = std::fopen(filePath, "rb");
    if (!f) {
        std::cout << "[ERROR] AnimatedTexture: cannot open (" << filePath << ")\n";
        return;
    }

    std::fseek(f, 0, SEEK_END);
    long fileSize = std::ftell(f);
    std::rewind(f);

    std::vector<stbi_uc> fileData(static_cast<size_t>(fileSize));
    std::fread(fileData.data(), 1, static_cast<size_t>(fileSize), f);
    std::fclose(f);

    // ----------------------------- //
    // 2. Try GIF (multi-frame) path //
    // ----------------------------- //
    int w = 0, h = 0, nFrames = 0, comp = 0;
    int* delays = nullptr;

    stbi_uc* pixels = stbi_load_gif_from_memory(
        fileData.data(), static_cast<int>(fileData.size()),
        &delays, &w, &h, &nFrames, &comp, 4);

    if (pixels && nFrames > 0) {
        m_width  = w;
        m_height = h;
        m_frames.reserve(static_cast<size_t>(nFrames));

        const size_t frameBytes = static_cast<size_t>(w * h * 4);

        for (int i = 0; i < nFrames; ++i) {
            Frame frame;
            frame.delay     = delays ? centiToSec(delays[i]) : 0.1f;
            frame.textureID = uploadFrame(pixels + i * frameBytes, w, h);
            m_frames.push_back(frame);
        }

        stbi_image_free(pixels);
        if (delays) stbi_image_free(delays);

        m_isLoaded = true;

    } else {
        // ------------------------------------------- //
        // 3. Fallback: single-frame image (PNG, JPG…) //
        // ------------------------------------------- //
        if (pixels) { stbi_image_free(pixels); }
        if (delays) { stbi_image_free(delays); }

        stbi_uc* img = stbi_load(filePath, &w, &h, &comp, 4);
        if (img) {
            m_width  = w;
            m_height = h;

            Frame frame;
            frame.delay     = 0.1f;
            frame.textureID = uploadFrame(img, w, h);
            m_frames.push_back(frame);

            stbi_image_free(img);
            m_isLoaded = true;
        }
    }

    m_isLoaded
        ? std::cout << "[LOAD] AnimatedTexture ID: " << m_idResource
                    << " (" << m_filePath << ") - "
                    << m_frames.size() << " frame(s)\n"
        : std::cout << "[ERROR] Loading AnimatedTexture (" << m_filePath << ")\n";
}

void ResourceAnimatedTexture::unload() {
    for (auto& f : m_frames)
        if (f.textureID) glDeleteTextures(1, &f.textureID);
    m_frames.clear();

    if (m_isLoaded)
        std::cout << "[UNLOAD] AnimatedTexture ID: " << m_idResource
                  << " (" << m_filePath << ")\n";

    m_isLoaded = false;
}
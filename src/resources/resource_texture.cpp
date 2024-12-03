#include "resource_texture.hpp"

#pragma GCC diagnostic ignored "-Wconversion"
#define STB_IMAGE_IMPLEMENTATION
#include "../libs/stb_image.h"
#pragma GCC diagnostic warning "-Wconversion"

// PUBLIC

namespace DarkMoon {
    bool Texture::load(const char* filePath) {
        this->m_filePath = filePath;
        setup();

        isLoaded() ? std::cout << "Load a texture (ID: " << m_id << ") -> " << m_filePath << std::endl
            : std::cout << "Error loading a texture -> " << m_filePath << std::endl;

        return isLoaded();
    }

    bool Texture::loadFrame(GifFileType* gifFile, int frameIndex) {
        this->m_filePath = "Loaded from GIF frame";
        setup();

        GifImageDesc* img = &gifFile->SavedImages[frameIndex].ImageDesc;
        ColorMapObject* colorMap = img->ColorMap ? img->ColorMap : gifFile->SColorMap;
        GifByteType* raster = gifFile->SavedImages[frameIndex].RasterBits;

        m_width = img->Width;
        m_height = img->Height;
        m_nrChannels = 4;

        unsigned char* data = (unsigned char*)stbi__malloc(static_cast<std::size_t>(m_width * m_height * m_nrChannels));
        memset(data, 0, static_cast<std::size_t>(m_width * m_height * m_nrChannels));

        int transparentColor = -1;
        if (gifFile->SavedImages[frameIndex].ExtensionBlockCount > 0) {
            for (int i = 0; i < gifFile->SavedImages[frameIndex].ExtensionBlockCount; i++) {
                if (gifFile->SavedImages[frameIndex].ExtensionBlocks[i].Function == GRAPHICS_EXT_FUNC_CODE) {
                    transparentColor = (unsigned char)gifFile->SavedImages[frameIndex].ExtensionBlocks[i].Bytes[3];
                    break;
                }
            }
        }

        for (int y = 0; y < img->Height; y++) {
            for (int x = 0; x < img->Width; x++) {
                GifByteType* p = raster + y * img->Width + x;
                data[y * m_width * m_nrChannels + x * m_nrChannels + 0] = colorMap->Colors[*p].Red;
                data[y * m_width * m_nrChannels + x * m_nrChannels + 1] = colorMap->Colors[*p].Green;
                data[y * m_width * m_nrChannels + x * m_nrChannels + 2] = colorMap->Colors[*p].Blue;
                data[y * m_width * m_nrChannels + x * m_nrChannels + 3] = (*p == transparentColor) ? 0 : 255;
            }
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);

        m_isLoad = true;

        return isLoaded();
    }

    void Texture::unload() {
        glDeleteTextures(1, &m_idTexture);
        if (isLoaded())
            std::cout << "Unload a texture (ID: " << m_id << ")" << std::endl;
        m_isLoad = false;
    }

    // PRIVATE

    void Texture::setup() {
        glGenTextures(1, &m_idTexture);
        glBindTexture(GL_TEXTURE_2D, m_idTexture);

        // Set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Load image, create texture and generate mipmaps
        unsigned char* data = stbi_load(m_filePath.c_str(), &m_width, &m_height, &m_nrChannels, STBI_rgb_alpha);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            m_isLoad = true;
        }
        stbi_image_free(data);
    }
}
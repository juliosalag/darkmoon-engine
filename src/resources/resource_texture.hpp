#pragma once
#include "resource.hpp"

#include <gif_lib.h>
#include <vector>
#include <GL/glew.h>

namespace DarkMoon {
    struct Texture : public Resource {
    public:
        Texture() {};
        Texture(std::size_t id) { this->m_id = id; };
        ~Texture() { unload(); };

        bool load(const char* filePath) override;
        bool loadFrame(GifFileType* gifFile, int frame);
        void unload() override;

        // Getters

        GLuint getIDTexture() { return m_idTexture; };
        int getWidth() { return m_width; };
        int getHeight() { return m_height; };

    private:
        GLuint m_idTexture;
        int m_width, m_height, m_nrChannels;

        void setup() override;
    };
}
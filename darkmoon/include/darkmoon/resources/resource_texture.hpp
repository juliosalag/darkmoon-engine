#pragma once

#include <glad/glad.h>
//#include <GL/glew.h>

#include "resource.hpp"

struct ResourceTexture : public Resource {
public:
    ResourceTexture(std::size_t idResource, std::size_t fileType, const char* filePath);
    ~ResourceTexture() { unload(); };

    void unload() override;

    int getWitdh(){ return m_width; }
    int getHeight(){ return m_height; }
    int getnrChannels(){ return m_nrChannels; }

    GLuint getIDTexture(){ return m_idTexture; }

private:
    GLuint m_idTexture {};
    int m_width {}, m_height {}, m_nrChannels {};

    void setup() override {};
};
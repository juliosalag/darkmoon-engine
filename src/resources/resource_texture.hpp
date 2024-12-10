#pragma once

#include <glad/glad.h>
//#include <GL/glew.h>

#include "resource.hpp"

struct ResourceTexture : public Resource {
public:
    ResourceTexture(std::size_t idResource, std::size_t fileType, const char* filePath);
    ~ResourceTexture() { unload(); };

    void unload() override;

private:
    GLuint m_idTexture {};
    int m_width {}, m_height {}, m_nrChannels {};

    void setup() override;
};
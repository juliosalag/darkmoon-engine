#include "../../libs/stb_image.h"

#include "resource_texture.hpp"

ResourceTexture::ResourceTexture(std::size_t idResource, std::size_t fileType, const char* filePath) 
    : Resource(idResource, fileType, filePath) {

    setup();

    m_isLoaded ? std::cout << "[LOAD] Texture ID: " << m_idResource << " (" << m_filePath << ")\n"
        :  std::cout << "[ERROR] Loading a texture (" << m_filePath << ")\n";
}

void ResourceTexture::unload() {
    /*
    glDeleteTextures(1, &m_idTexture);
    */

    if(m_isLoaded) std::cout << "[UNLOAD] Texture ID: " << m_idResource << " (" << m_filePath << ")\n";

    this->m_isLoaded = false;
}

void ResourceTexture::setup() {
    /*
    glGenTextures(1, &m_idTexture);
    glBindTexture(GL_TEXTURE_2D, m_idTexture);

    // Set texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Load image, create texture and generate mipmaps
    unsigned char* data = stbi_load(m_filePath.c_str(), &m_width, &m_height, &m_nrChannels, STBI_rgb_alpha);
    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        this->m_isLoaded = true;
    }
    stbi_image_free(data);
    */

    this->m_isLoaded = true;
}
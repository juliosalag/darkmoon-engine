#pragma once

#include <glad/glad.h>
#include <fstream>
#include <sstream>

#include "resource.hpp"

struct ResourceShader : public Resource {
public:
    ResourceShader(std::size_t id, std::size_t fileType, const char* vertexPath = "", const char* fragmentPath = "", const char* geometryPath = "");
    ~ResourceShader(){ unload(); }; 

    void use();
    void unload() override;

    GLuint getIDShader(){ return m_idShader; };

private:
    GLuint m_idShader {};
    std::string m_vertexPath {};
    std::string m_fragmentPath {};
    std::string m_geometryPath {};

    void setup() override;
    std::string ReadShader(std::string code, std::ifstream file);
};
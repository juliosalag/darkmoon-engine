#pragma once

#include <cstddef>
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <string>

#include "resource.hpp"

struct ResourceShader : public Resource {
public:
    ResourceShader(std::size_t id, std::size_t fileType, const char* vertexPath = "", const char* fragmentPath = "", const char* geometryPath = "");
    ResourceShader(std::size_t id, std::size_t fileType, const char* path, const std::string& vertexCode = "", const std::string& fragmentCode  = "", const std::string& geometryCode = "");
    ~ResourceShader(){ unload(); }; 

    void use();
    void unload() override;

    GLuint getIDShader(){ return m_idShader; };

private:
    GLuint m_idShader {};
    std::string m_vertexPath {}, m_fragmentPath {}, m_geometryPath {};
    std::string m_vertexCode {}, m_fragmentCode {}, m_geometryCode {};
    
    void setup() override;
    std::string ReadShader(std::string code, std::ifstream file);
};
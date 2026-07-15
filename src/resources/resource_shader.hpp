#pragma once

#include "resource.hpp"

#include <string>
#include <cstring>

#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace DarkMoon {
    struct Shader : public Resource {
    public:
        Shader(std::size_t id, const char* vertexPath = "", const char* fragmentPath = "", const char* geometryPath = "");
        ~Shader() { unload(); };

        void use();
        bool load(const char* filePath) override;
        void unload() override;

        // Getters

        GLuint getIDShader() { return m_idShader; };

    private:
        GLuint m_idShader;
        std::string m_vertexPath;
        std::string m_fragmentPath;
        std::string m_geometryPath;

        void setup() override;
    };
}
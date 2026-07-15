#include "resource_shader.hpp"

namespace DarkMoon {
    Shader::Shader(std::size_t id, const char* vertexPath, const char* fragmentPath, const char* geometryPath) {
        this->m_id = id;
        this->m_vertexPath = vertexPath;
        this->m_fragmentPath = fragmentPath;
        this->m_geometryPath = geometryPath;
    };

    bool Shader::load(const char* filePath) {
        this->m_filePath = filePath;

        setup();

        std::cout << "Load a shader (ID: " << m_id << ") -> " << m_filePath << std::endl;
        m_isLoad = true;

        return isLoaded();
    }

    void Shader::use() { glUseProgram(m_idShader); }

    void Shader::unload() {
        if (isLoaded())
            std::cout << "Unload a shader (ID: " << m_id << ")" << std::endl;
        glDeleteProgram(m_idShader);
        m_isLoad = false;
    }

    // PRIVATE

    void Shader::setup() {

        //----- Retrieve the vertex/fragment source code from filePath -----//
        std::string vertexCode {};
        std::string fragmentCode {};
        std::string geometryCode {};
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        std::ifstream gShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            // open files
            vShaderFile.open(m_vertexPath);
            fShaderFile.open(m_fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();

            if(m_geometryPath != ""){
                gShaderFile.open(m_geometryPath);
                std::stringstream gShaderStream;
                gShaderStream << gShaderFile.rdbuf();
                gShaderFile.close();
                geometryCode = gShaderStream.str();
            }
        }
        catch (const std::ifstream::failure& e) {
            std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n"
                << "Error code: " << e.code() << "\n"
                << "Exception: " << e.what() << std::endl;
        }

        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        const char* gShaderCode = geometryCode.c_str();

        //----- Compile shaders -----//
        GLuint vertex, fragment, geometry;

        int success;
        char infoLog[512];

        // Vertex Shader
        //--------------
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        // check for shader compile errors
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        };

        // Fragment Shader
        //----------------
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        // check for shader compile errors
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        // Geometry Shader
        //----------------

        if(!geometryCode.empty()){
            geometry = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometry, 1, & gShaderCode, NULL);
            glCompileShader(geometry);
            // check for shader compile errors
            glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
            if(!success){
                glGetShaderInfoLog(geometry, 512, NULL, infoLog);
                std::cerr << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
            }
        }

        // Link Shaders
        // ------------
        m_idShader = glCreateProgram();
        glAttachShader(m_idShader, vertex);
        glAttachShader(m_idShader, fragment);
        glLinkProgram(m_idShader);
        // check for linking errors
        glGetProgramiv(m_idShader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(m_idShader, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
}
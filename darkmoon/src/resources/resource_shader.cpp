#include <darkmoon/resources/resource_shader.hpp>

Shader::Shader(std::size_t idResource, std::size_t fileType, const char* vertexPath, const char* fragmentPath, const char* geometryPath) 
    : Resource(idResource, fileType, (std::string(vertexPath) + fragmentPath + geometryPath).c_str()) {

    m_vertexPath = vertexPath;
    m_fragmentPath = fragmentPath;
    m_geometryPath = geometryPath;

    setup();

    m_isLoaded ? std::cout << "[LOAD] Shader ID: " << m_idResource << /*" (" << m_filePath << ")\n"*/ "\n"
        :  std::cout << "[ERROR] Loading a shader (" << m_filePath << ")\n";
}

Shader::Shader(std::size_t idResource, std::size_t fileType, const char* path, const std::string& vertexCode, const std::string& fragmentCode , const std::string& geometryCode) 
    : Resource(idResource, fileType, path) {

    m_vertexCode = vertexCode;
    m_fragmentCode = fragmentCode;
    m_geometryCode = geometryCode;

    setup();

    m_isLoaded ? std::cout << "[LOAD] Shader ID: " << m_idResource << /*" (" << m_filePath << ")\n"*/ "\n"
        :  std::cout << "[ERROR] Loading a shader (" << m_filePath << ")\n";
}

void Shader::unload() {

    if(m_isLoaded) std::cout << "[UNLOAD] Shader ID: " << m_idResource << /*" (" << m_filePath << ")\n"*/ "\n";

    if (glIsProgram(m_idShader)) {
        glDeleteProgram(m_idShader);
    }
    m_isLoaded = false;
}

void Shader::setup() {

    m_isLoaded = true;

    std::string vertexCode {}, fragmentCode {}, geometryCode {};

    // Flow 1: from strings
    if (!m_vertexCode.empty()) {
        vertexCode   = m_vertexCode;
        fragmentCode = m_fragmentCode;
        geometryCode = m_geometryCode;
    }
    // Flow 2: from files
    else {
        std::ifstream vShaderFile{}, fShaderFile{}, gShaderFile{};
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            if (m_vertexPath != "") {
                vShaderFile.open(m_vertexPath);
                std::stringstream ss; ss << vShaderFile.rdbuf();
                vertexCode = ss.str();
            }
            if (m_fragmentPath != "") {
                fShaderFile.open(m_fragmentPath);
                std::stringstream ss; ss << fShaderFile.rdbuf();
                fragmentCode = ss.str();
            }
            if (m_geometryPath != "") {
                gShaderFile.open(m_geometryPath);
                std::stringstream ss; ss << gShaderFile.rdbuf();
                geometryCode = ss.str();
            }
        }
        catch (const std::ifstream::failure& e) {
            std::cerr << "[ERROR] Shader file not succesfully read\n"
                      << "Error code: " << e.code() << "\n"
                      << "Exception: " << e.what() << "\n";
            m_isLoaded = false;
            return; // Do not continue with empty/invalid code
        }
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    const char* gShaderCode = geometryCode.c_str();

    // Compile shaders

    GLuint vertex {}, fragment {}, geometry {}; // Init to 0 so glIsShader returns false if unused
    int success {};
    char infolog[512] {};

    // Vertex shader
    if (!vertexCode.empty()) {
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertex, sizeof(infolog), NULL, infolog);
            std::cerr << "[ERROR] Vertex shader compilation failed\n" << infolog << "\n";
            m_isLoaded = false;
            glDeleteShader(vertex);
            vertex = 0;
        }
    }

    // Fragment shader
    if (!fragmentCode.empty()) {
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragment, sizeof(infolog), NULL, infolog);
            std::cerr << "[ERROR] Fragment shader compilation failed\n" << infolog << "\n";
            m_isLoaded = false;
            glDeleteShader(fragment);
            fragment = 0;
        }
    }

    // Geometry shader
    if (!geometryCode.empty()) {
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &gShaderCode, NULL);
        glCompileShader(geometry);
        glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(geometry, sizeof(infolog), NULL, infolog);
            std::cerr << "[ERROR] Geometry shader compilation failed\n" << infolog << "\n";
            m_isLoaded = false;
            glDeleteShader(geometry);
            geometry = 0;
        }
    }

    // Link shaders into program
    m_idShader = glCreateProgram();

    if (glIsShader(vertex))   glAttachShader(m_idShader, vertex);
    if (glIsShader(fragment)) glAttachShader(m_idShader, fragment);
    if (glIsShader(geometry)) glAttachShader(m_idShader, geometry);

    glLinkProgram(m_idShader);

    glGetProgramiv(m_idShader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_idShader, sizeof(infolog), NULL, infolog);
        std::cerr << "[ERROR] Shader program linking failed\n" << infolog << "\n";
        m_isLoaded = false;
        glDeleteProgram(m_idShader);
    }

    // Shaders are no longer needed once linked into the program
    if (glIsShader(vertex))   glDeleteShader(vertex);
    if (glIsShader(fragment)) glDeleteShader(fragment);
    if (glIsShader(geometry)) glDeleteShader(geometry);
}
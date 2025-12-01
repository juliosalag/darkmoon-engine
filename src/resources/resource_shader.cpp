#include "resource_shader.hpp"

ResourceShader::ResourceShader(std::size_t idResource, std::size_t fileType, const char* vertexPath, const char* fragmentPath, const char* geometryPath) 
    : Resource(idResource, fileType, (std::string(vertexPath) + fragmentPath + geometryPath).c_str()) {

    m_vertexPath = vertexPath;
    m_fragmentPath = fragmentPath;
    m_geometryPath = geometryPath;

    setup();

    m_isLoaded ? std::cout << "[LOAD] Shader ID: " << m_idResource << /*" (" << m_filePath << ")\n"*/ "\n"
        :  std::cout << "[ERROR] Loading a shader (" << m_filePath << ")\n";
}

ResourceShader::ResourceShader(std::size_t idResource, std::size_t fileType, const char* path, const std::string& vertexCode, const std::string& fragmentCode , const std::string& geometryCode) 
    : Resource(idResource, fileType, path) {

    m_vertexCode = vertexCode;
    m_fragmentCode = fragmentCode;
    m_geometryCode = geometryCode;

    setup();

    m_isLoaded ? std::cout << "[LOAD] Shader ID: " << m_idResource << /*" (" << m_filePath << ")\n"*/ "\n"
        :  std::cout << "[ERROR] Loading a shader (" << m_filePath << ")\n";
}

void ResourceShader::unload() {

    if(m_isLoaded) std::cout << "[UNLOAD] Shader ID: " << m_idResource << /*" (" << m_filePath << ")\n"*/ "\n";

    if (glIsProgram(m_idShader)) {
        glDeleteProgram(m_idShader);
    }
    m_isLoaded = false;
}

void ResourceShader::setup() { // TODO: Refactorizar el codigo //

    m_isLoaded = true;

    // Retrieve the vertex/fragment/geometry source code from filePath
    std::string vertexCode {}, fragmentCode {}, geometryCode {};
    std::ifstream vShaderFile{}, fShaderFile {}, gShaderFile {};
    
    // Ensure ifstream objects can throw exceptions
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    
    // Read files //

    try {
        if(m_vertexPath != ""){
            vShaderFile.open(m_vertexPath);
            std::stringstream vShaderStream;
            vShaderStream << vShaderFile.rdbuf();
            vShaderFile.close();
            vertexCode = vShaderStream.str();
        }

        if(m_fragmentPath != ""){
            fShaderFile.open(m_fragmentPath);
            std::stringstream fShaderStream;
            fShaderStream << fShaderFile.rdbuf();
            fShaderFile.close();
            fragmentCode = fShaderStream.str();
        }

        if(m_geometryPath != ""){
            gShaderFile.open(m_geometryPath);
            std::stringstream gShaderStream;
            gShaderStream << gShaderFile.rdbuf();
            gShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (const std::ifstream::failure& e){
        std::cerr << "[ERROR] Shader file not succesfully read\n" << "Error code: " << e.code() << "\n" << "Exception: " << e.what() << "\n";
        m_isLoaded = false;
    }

    if(!m_vertexCode.empty()){
        vertexCode = m_vertexCode;
        fragmentCode = m_fragmentCode;
        geometryCode = m_geometryCode;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    const char* gShaderCode = geometryCode.c_str();


    // Compile shaders //

    GLuint vertex, fragment, geometry;

    int success {};
    char infolog[512] {};

    // Vertex Shader
    if(!vertexCode.empty()){
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        // Check shader compile errors
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if(!success){
            glGetShaderInfoLog(vertex, sizeof(infolog), NULL, infolog);
            std::cerr << "[ERROR] Shader vertex compilation failed\n";
            m_isLoaded = false;
            glDeleteShader(vertex);
        }
    }

    // Fragment Shader
    if(!fragmentCode.empty()){
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        // Check shader compile errors
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if(!success){
            glGetShaderInfoLog(fragment, sizeof(infolog), NULL, infolog);
            std::cerr << "[ERROR] Shader fragment compilation failed\n";
            m_isLoaded = false;
            glDeleteShader(fragment);
        }
    }

    // Geometry Shader
    if(!geometryCode.empty()){
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &gShaderCode, NULL);
        glCompileShader(geometry);
        // Check shader compile errors
        glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
        if(!success){
            glGetShaderInfoLog(geometry, sizeof(infolog), NULL, infolog);
            std::cerr << "[ERROR] Shader geometry compilation failed\n";
            m_isLoaded = false;
            glDeleteShader(geometry);
        }
        
    }

    // Link Shaders //

    m_idShader = glCreateProgram();

    if(glIsShader(vertex)) glAttachShader(m_idShader, vertex);
    if(glIsShader(fragment)) glAttachShader(m_idShader, fragment);
    if(glIsShader(geometry)) glAttachShader(m_idShader, geometry);

    glLinkProgram(m_idShader);

    glGetProgramiv(m_idShader, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(m_idShader, sizeof(infolog), NULL, infolog);
        std::cerr << "[ERROR] Shader program linking failed\n" << infolog << "\n";
        m_isLoaded = false,
        glDeleteProgram(m_idShader);
    }

    if(glIsShader(vertex)) glDeleteShader(vertex);
    if(glIsShader(fragment)) glDeleteShader(fragment);
    if(glIsShader(geometry)) glDeleteShader(geometry);
}
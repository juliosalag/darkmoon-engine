#include "windows_manager.hpp"

#include <iostream>
#include "../libs/stb_image.h"

// Windows manager
namespace DarkMoon {
    bool WindowsManager::initWindow(int width, int height, const char* title) {
        this->m_width = width;
        this->m_height = height;

        // glfw: initialize and configure
        // ------------------------------
        if (!glfwInit()) {
            std::cerr << "Error initializing GLFW" << std::endl;
            return false;
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Deactivate V-Sync
        //glfwSwapInterval(0);

        // Enable V-Sync
        // glfwSwapInterval(1);

        // glfw window creation
        // --------------------
        m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (!m_window) {
            std::cerr << "Error creating window" << std::endl;
            glfwTerminate();
            return false;
        }

        // Set the window icon
        GLFWimage images[1];
        images[0].pixels = stbi_load("assets/mago_icon.png", &images[0].width, &images[0].height, 0, 4); //rgba channels 
        glfwSetWindowIcon(m_window, 1, images);
        stbi_image_free(images[0].pixels);

        glfwMakeContextCurrent(m_window);
        //glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback());

        // glfwSetErrorCallback([](auto error, auto description){
        //     std::fprintf(stderr, "Glfw Error %d: %s\n", error, description);
        //     throw std::runtime_error("Glfw Error");
        // });

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        //if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        //    std::cerr << "Error initilizing GLAD" std::endl;
        //    return false;
        //}

        if (glewInit() != GLEW_OK) {
            std::cerr << "Error initializing GLEW" << std::endl;
            glfwTerminate();
            return false;
        }

        if (!glfwExtensionSupported("GL_ARB_vertex_array_object") || !GLEW_VERSION_3_3) {
            std::cerr << "Error: OpenGL 3.3 and GL_ARB_vertex_array_object are required" << std::endl;
            return false;
        }

        glViewport(0, 0, m_width, m_height);
        //std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

        setTargetFPS(60);
        m_lastFrameTime = getTime();

        return true;
    }

    void WindowsManager::framebuffer_size_callback() {
        glViewport(0, 0, m_width, m_height);
    }

    void WindowsManager::closeWindow() {
        if (m_window) {
            glfwDestroyWindow(m_window);
        }
        glfwTerminate();
    }

    bool WindowsManager::windowShouldClose() const {
        return m_window && glfwWindowShouldClose(m_window);
    }

    void WindowsManager::setWindowSize(int width, int height) {
        if (m_window) {
            m_width = width;
            m_height = height;
            glfwSetWindowSize(m_window, m_width, m_height);
            framebuffer_size_callback();
        }
    }

    bool WindowsManager::isWindowFullscreen() {
        int monitorCount{};
        // Check if the window is in fullscreen mode
        GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
        for (int i = 0; i < monitorCount; i++) {
            GLFWmonitor* monitor = monitors[i];
            if (glfwGetWindowMonitor(m_window) == monitor)
                return true;
        }

        return false;
    }

    void WindowsManager::toggleFullscreen() {
        if (m_window) {
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();

            // Fullscreen mode -> Windowed mode
            if (isWindowFullscreen()) {
                glfwSetWindowMonitor(m_window, nullptr, 100, 100, m_oldWidth, m_oldHeight, GLFW_DONT_CARE);

                m_width = m_oldWidth;
                m_height = m_oldHeight;
            }
            // Windowed mode -> Fullscreen mode
            else {
                const GLFWvidmode* mode = glfwGetVideoMode(monitor);
                glfwSetWindowMonitor(m_window, monitor, 01, 01, mode->width, mode->height, mode->refreshRate);

                m_oldWidth = m_width;
                m_oldHeight = m_height;

                m_width = mode->width;
                m_height = mode->height;
            }
        }
    }

    float WindowsManager::getWidthRatio() {
        return (float)m_width / (float)1920;
    }

    float WindowsManager::getHeightRatio() {
        return (float)m_height / (float)1080;
    }

    glm::vec2 WindowsManager::getMonitorSize() {
        int monitorCount;
        GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
        GLFWmonitor* monitor = monitors[0];
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        return glm::vec2(mode->width, mode->height);
    }

    // Framebuffer

    void WindowsManager::createFramebuffer(){
        // Framebuffer
        glGenFramebuffers(1, &m_framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

        // Create a texture to hold the color buffer
        glGenTextures(1, &m_texColorBuffer);
        glBindTexture(GL_TEXTURE_2D, m_texColorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texColorBuffer, 0);

        // Create a renderbuffer object for depth and stencil attachment
        glGenRenderbuffers(1, &m_RBO);
        glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cerr << "Framebuffer is not complete\n";

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void WindowsManager::configureFramebufferVAO(){
        float quadVertices[] = {
            // positions    // texCoords
            -1.0f,  1.0f,   0.0f, 1.0f,
            -1.0f, -1.0f,   0.0f, 0.0f,
             1.0f, -1.0f,   1.0f, 0.0f,

            -1.0f,  1.0f,   0.0f, 1.0f,
             1.0f, -1.0f,   1.0f, 0.0f,
             1.0f,  1.0f,   1.0f, 1.0f
        };

        glGenVertexArrays(1, &m_screenVAO);
        glGenBuffers(1, &m_screenVBO);

        glBindVertexArray(m_screenVAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_screenVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    }

    void WindowsManager::applySobelFilter(){
        configureFramebufferVAO();

        // Sobel Filter

        glDisable(GL_DEPTH_TEST);

        RenderManager& rm = RenderManager::getInstance();
        rm.useShader(rm.shaders["sobel"]);

        glUniform1f(glGetUniformLocation(rm.shaders["sobel"]->getIDShader(), "depthThreshold"), rm.depthThreshold);
        glUniform1f(glGetUniformLocation(rm.shaders["sobel"]->getIDShader(), "normalThreshold"), rm.normalThreshold);    

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texColorBuffer);
        
        glBindVertexArray(m_screenVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glEnable(GL_DEPTH_TEST);

        // Clean up resources

        cleanup();
    }

    void WindowsManager::cleanup(){
        glDeleteFramebuffers(1, &m_framebuffer);
        glDeleteTextures(1, &m_texColorBuffer);
        glDeleteRenderbuffers(1, &m_RBO);
        glDeleteVertexArrays(1, &m_screenVAO);
        glDeleteBuffers(1, &m_screenVBO);
    }

    // Drawing-related functions

    void WindowsManager::beginDrawing() {
        // Shader Cartoon
        //RenderManager& rm = RenderManager::getInstance();
        //if(rm.activeShaderCartoon){
        //    createFramebuffer();
        //    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
        //    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //}

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, m_width, 0, m_height, -1, 1);
    }

    void WindowsManager::endDrawing() {
        // Shader Cartoon
        //RenderManager& rm = RenderManager::getInstance();
        //if(rm.activeShaderCartoon){
        //    glBindFramebuffer(GL_FRAMEBUFFER, 0);
        //    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
        //    applySobelFilter();
        //}

        glfwSwapBuffers(m_window);
        controlFrameRate();
        glfwPollEvents();
    }

    // Timing-related functions
    void WindowsManager::setTargetFPS(int targetFPS) {
        m_targetFrameTime = 1.0 / targetFPS;
    }

    double WindowsManager::getTargetFPS() {
        return 1.0 / m_targetFrameTime;
    }

    double WindowsManager::getFrameTime() {
        return m_deltaTime;
    }

    double WindowsManager::getTime() {
        return glfwGetTime();
    }

    int WindowsManager::getFPS() {
        return m_FPS;
    }

    void WindowsManager::controlFrameRate() {
        double currentFrameTime = tim.getElapsedTime();
        m_deltaTime = currentFrameTime - m_lastFrameTime;
        m_lastFrameTime = currentFrameTime;

        double sleepTime = m_targetFrameTime - m_deltaTime;
        if (sleepTime > 0.0) {
            double endTime = currentFrameTime + sleepTime;
            while (tim.getElapsedTime() < endTime) {
                // Bucle activo
            }
            currentFrameTime = tim.getElapsedTime();
            m_deltaTime = currentFrameTime - m_lastFrameTime;
            m_lastFrameTime = currentFrameTime;
        }

        m_frames++;
        if (currentFrameTime - m_lastFPSTime >= 1.0) {
            m_FPS = m_frames;
            m_frames = 0;
            m_lastFPSTime += 1.0;
        }
    }
}
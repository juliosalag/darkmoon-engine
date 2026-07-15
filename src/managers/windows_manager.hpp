#pragma once

#include "render_manager.hpp"

//#include <glad/glad.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <glm/glm.hpp>

#include <thread>
#include <chrono>

struct Timer {
    Timer() : start(std::chrono::high_resolution_clock::now()) {}

    double getElapsedTime() {
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        return static_cast<double>(elapsed.count()) / 1000000.0;
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
};

namespace DarkMoon {
    struct WindowsManager {
    private:
        GLFWwindow* m_window{};
        int m_width{};
        int m_height{};
        int m_oldWidth{};
        int m_oldHeight{};

        int m_FPS{};
        int m_frames{};
        double m_lastFrameTime{};
        double m_targetFrameTime{};
        double m_lastFPSTime{};
        double m_deltaTime{};

        void controlFrameRate();
        void framebuffer_size_callback();

        // Framebuffer
        GLuint m_framebuffer {};
        GLuint m_texColorBuffer {};
        GLuint m_RBO {};
        GLuint m_screenVAO {}, m_screenVBO {};

        void createFramebuffer();
        void configureFramebufferVAO();
        void applySobelFilter();

        void cleanup();

    public:
        // Window-related functions
        bool initWindow(int width, int height, const char* title);
        void closeWindow();
        bool windowShouldClose() const;
        int getScreenWidth() { return m_width; };
        int getScreenHeight() { return m_height; };
        GLFWwindow* getWindow() { return m_window; };
        void setWindowSize(int width, int height);
        bool isWindowFullscreen();
        void toggleFullscreen();
        float getWidthRatio();
        float getHeightRatio();
        glm::vec2 getMonitorSize();

        // Drawing-related functions
        void beginDrawing();
        void endDrawing();

        // Timing-related functions
        void setTargetFPS(int fps);
        double getTargetFPS();
        double getFrameTime();
        double getTime();
        int getFPS();
        Timer tim{};

        static WindowsManager& getInstance() {
            static WindowsManager instance;
            return instance;
        }
    };
}
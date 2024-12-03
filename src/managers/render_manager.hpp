#pragma once

#include "../resources/resource_shader.hpp"
#include "../resources/resource_texture.hpp"
#include "../resources/resource_font.hpp"
#include "../resources/resource_mesh.hpp"
#include "../components/entity_camera.hpp"
#include "../components/entity_light.hpp"

#include "resource_manager.hpp"

#include "../utils/color.hpp"

#include <map>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace DarkMoon {
    struct RenderManager {
    private:
        Shader* m_shaderProgram{};
        int m_width{}, m_height{};

    public:
        Camera* m_camera{};
        Material* defaultMaterial{};
        Font* defaultFont{};
        std::vector<Light*> lights{};

        std::vector<PointLight*> pointLights {};
        std::vector<DirectionalLight*> directionalLights {};
        std::vector<SpotLight*> spotLights {};

        // Shaders
        std::map<std::string, Shader*> shaders;

        bool activeLights { false };
        void updateLights();
        void checkLights();

        // Shader cartoon
        bool activeShaderCartoon { false };
        float depthThreshold = { 0.5f };
        float normalThreshold = { 0.76f };
        glm::vec4 outlineColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); 

        // Screen width and height
        void setWindowSize(int width, int height) { m_width = width; m_height = height; };

        // Get position from 3D world to 2D world
        glm::vec2 getWorldToScreen(glm::vec3 pos3D);

        // Basic drawing functions
        void beginMode3D();
        void endMode3D();

        // Camera
        void setCamera(Camera* camera) { m_camera = camera; };

        // Basic drawing functions
        void clearBackground(Color color);

        // ChangeShader
        void useShader(Shader* shader) {
            m_shaderProgram = shader;
            glUseProgram(m_shaderProgram->getIDShader());
        };

        Shader* getShader() { return m_shaderProgram; };
        int getWidth() { return m_width; };
        int getHeight() { return m_height; };

        static RenderManager& getInstance() {
            static RenderManager instance;
            return instance;
        }

        // Auxiliars
        glm::vec4 normalizeColor(const Color& color) {
            return glm::vec4(
                static_cast<float>(color.r) / 255.0f,
                static_cast<float>(color.g) / 255.0f,
                static_cast<float>(color.b) / 255.0f,
                static_cast<float>(color.a) / 255.0f
            );
        }
        float normalizeX(float x) { return (x / static_cast<float>(m_width)) * 2 - 1; };
        float normalizeY(float y) { return -((y / static_cast<float>(m_height)) * 2 - 1); };
    };
}
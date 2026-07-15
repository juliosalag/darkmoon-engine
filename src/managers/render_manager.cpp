#include "render_manager.hpp"

#include <chrono>
#include <iostream>

// Basic drawing functions
namespace DarkMoon {

    // Get position from 3D world to 2D world
    glm::vec2 RenderManager::getWorldToScreen(glm::vec3 pos3D) {
        // Transform world position to clip space
        glm::vec4 clipSpace = m_camera->getProjectionMatrix(m_width, m_height) * m_camera->getViewMatrix() * glm::vec4(pos3D, 1.0f);

        // Convert clip space to normalized device coordinates (NDC)
        glm::vec3 ndc = glm::vec3(clipSpace) / clipSpace.w;

        // Convert NDC to screen space
        glm::vec2 screenPos;
        screenPos.x = (ndc.x + 1.0f) * 0.5f * static_cast<float>(m_width);
        screenPos.y = (1.0f - ndc.y) * 0.5f * static_cast<float>(m_height);

        return screenPos;
    }

    void RenderManager::beginMode3D() {
        //checkLights();
        glEnable(GL_DEPTH_TEST);
        //glEnable(GL_CULL_FACE);
        useShader(shaders["3D"]);
    }

    void RenderManager::endMode3D() {
        //glPopMatrix();
        //glPopAttrib();

        glDisable(GL_DEPTH_TEST);
        //glDisable(GL_CULL_FACE);
        useShader(shaders["color"]);
    }

    void RenderManager::updateLights() {
        pointLights.clear();
        directionalLights.clear();
        spotLights.clear();
    }

    void RenderManager::checkLights() {
        useShader(shaders["3D"]);
        glUniform1i(glGetUniformLocation(shaders["3D"]->getIDShader(), "activeLights"), activeLights ? 1 : 0);
        glUniform1i(glGetUniformLocation(shaders["3D"]->getIDShader(), "activeShaderCartoon"), activeShaderCartoon ? 1 : 0);

        if (activeLights) {

            //std::cout << "-----------------------------\n";
            //std::cout << "Luces direccionales: " << directionalLights.size() << "\n";
            //std::cout << "Luces puntuales: " << pointLights.size() << "\n";
            //std::cout << "Luces focales: " << spotLights.size() << "\n";

            // Directional Lights //

            if (!directionalLights.empty()) {
                for (std::size_t i = 0; i < directionalLights.size(); i++) {
                    std::string directionUniformName = "gDirectionalLight.direction";
                    std::string colorUniformName = "gDirectionalLight.base.color";
                    std::string ambientUniformName = "gDirectionalLight.base.ambientIntensity";
                    std::string diffuseUniformName = "gDirectionalLight.base.diffuseIntensity";

                    glUniform3fv(glGetUniformLocation(shaders["3D"]->getIDShader(), directionUniformName.c_str()), 1, glm::value_ptr(directionalLights[i]->direction));
                    glUniform3fv(glGetUniformLocation(shaders["3D"]->getIDShader(), colorUniformName.c_str()), 1, glm::value_ptr(normalizeColor(directionalLights[i]->color)));
                    glUniform1f(glGetUniformLocation(shaders["3D"]->getIDShader(), ambientUniformName.c_str()), directionalLights[i]->ambientIntensity);
                    glUniform1f(glGetUniformLocation(shaders["3D"]->getIDShader(), diffuseUniformName.c_str()), directionalLights[i]->diffuseIntensity);
                }

            }
            //glUniform1i(glGetUniformLocation(shaders["3D"]->getIDShader(), "NumDirectionalLights"), static_cast<int>(directionalLights.size()));

            // Point Lights //

            if (!pointLights.empty()) {
                for (std::size_t i = 0; i < pointLights.size(); i++) {
                    std::string positionUniformName = "gPointLights[" + std::to_string(i) + "].position";
                    std::string colorUniformName = "gPointLights[" + std::to_string(i) + "].base.color";
                    std::string constantUniformName = "gPointLights[" + std::to_string(i) + "].attenuation.constant";
                    std::string linearUniformName = "gPointLights[" + std::to_string(i) + "].attenuation.linear";
                    std::string quadraticUniformName = "gPointLights[" + std::to_string(i) + "].attenuation.exp";
                    std::string ambientUniformName = "gPointLights[" + std::to_string(i) + "].base.ambientIntensity";
                    std::string diffuseUniformName = "gPointLights[" + std::to_string(i) + "].base.diffuseIntensity";

                    glUniform3fv(glGetUniformLocation(shaders["3D"]->getIDShader(), positionUniformName.c_str()), 1, glm::value_ptr(pointLights[i]->position));
                    glUniform3fv(glGetUniformLocation(shaders["3D"]->getIDShader(), colorUniformName.c_str()), 1, glm::value_ptr(normalizeColor(pointLights[i]->color)));
                    glUniform1f(glGetUniformLocation(shaders["3D"]->getIDShader(), constantUniformName.c_str()), pointLights[i]->constant);
                    glUniform1f(glGetUniformLocation(shaders["3D"]->getIDShader(), linearUniformName.c_str()), pointLights[i]->linear);
                    glUniform1f(glGetUniformLocation(shaders["3D"]->getIDShader(), quadraticUniformName.c_str()), pointLights[i]->quadratic);
                    glUniform1f(glGetUniformLocation(shaders["3D"]->getIDShader(), ambientUniformName.c_str()), pointLights[i]->ambientIntensity);
                    glUniform1f(glGetUniformLocation(shaders["3D"]->getIDShader(), diffuseUniformName.c_str()), pointLights[i]->diffuseIntensity);
                }
            }
            glUniform1i(glGetUniformLocation(shaders["3D"]->getIDShader(), "gNumPointLights"), static_cast<int>(pointLights.size()));

            // Spot Lights //

            if (!spotLights.empty()) {
                for (std::size_t i = 0; i < spotLights.size(); i++) {
                    std::string colorUniformName = "gSpotLights[" + std::to_string(i) + "].base.base.color";
                    std::string positionUniformName = "gSpotLights[" + std::to_string(i) + "].base.position";
                    std::string directionUniformName = "gSpotLights[" + std::to_string(i) + "].direction";
                    std::string cutOffUniformName = "gSpotLights[" + std::to_string(i) + "].cutOff";

                    std::string constantUniformName = "gSpotLights[" + std::to_string(i) + "].base.attenuation.constant";
                    std::string linearUniformName = "gSpotLights[" + std::to_string(i) + "].base.attenuation.linear";
                    std::string quadraticUniformName = "gSpotLights[" + std::to_string(i) + "].base.attenuation.exp";

                    std::string ambientUniformName = "gSpotLights[" + std::to_string(i) + "].base.base.ambientIntensity";
                    std::string diffuseUniformName = "gSpotLights[" + std::to_string(i) + "].base.base.diffuseIntensity";

                    glUniform3fv(glGetUniformLocation(shaders["3D"]->getIDShader(), positionUniformName.c_str()), 1, glm::value_ptr(spotLights[i]->position));
                    glUniform3fv(glGetUniformLocation(shaders["3D"]->getIDShader(), directionUniformName.c_str()), 1, glm::value_ptr(spotLights[i]->direction));
                    glUniform1f(glGetUniformLocation(shaders["3D"]->getIDShader(), cutOffUniformName.c_str()), spotLights[i]->cutOff);
                    glUniform3fv(glGetUniformLocation(shaders["3D"]->getIDShader(), colorUniformName.c_str()), 1, glm::value_ptr(normalizeColor(spotLights[i]->color)));

                    glUniform1f(glGetUniformLocation(shaders["3D"]->getIDShader(), constantUniformName.c_str()), spotLights[i]->constant);
                    glUniform1f(glGetUniformLocation(shaders["3D"]->getIDShader(), linearUniformName.c_str()), spotLights[i]->linear);
                    glUniform1f(glGetUniformLocation(shaders["3D"]->getIDShader(), quadraticUniformName.c_str()), spotLights[i]->quadratic);

                    glUniform1f(glGetUniformLocation(shaders["3D"]->getIDShader(), ambientUniformName.c_str()), spotLights[i]->ambientIntensity);
                    glUniform1f(glGetUniformLocation(shaders["3D"]->getIDShader(), diffuseUniformName.c_str()), spotLights[i]->diffuseIntensity);
                }
            }
            glUniform1i(glGetUniformLocation(shaders["3D"]->getIDShader(), "gNumSpotLights"), static_cast<int>(spotLights.size()));

        }

        if (activeShaderCartoon) {
            glUniform1f(glGetUniformLocation(shaders["3D"]->getIDShader(), "depthThreshold"), depthThreshold);
            glUniform1f(glGetUniformLocation(shaders["3D"]->getIDShader(), "normalThreshold"), normalThreshold);
            glUniform4f(glGetUniformLocation(shaders["3D"]->getIDShader(), "_outlineColor"), outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a);
        }
    }

    // Basic drawing functions

    void RenderManager::clearBackground(Color color) {
        glClearColor(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}
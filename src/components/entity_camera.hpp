#pragma once

#include "entity.hpp"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace DarkMoon {
    // Camera projection
    enum struct CameraProjection { CAMERA_PERSPECTIVE, CAMERA_ORTHOGRAPHIC };

    struct Ray {
        glm::vec3 origin;
        glm::vec3 direction;
    };

    struct Camera : Entity {
    public:
        // CameraProjection
        CameraProjection cameraProjection{ CameraProjection::CAMERA_ORTHOGRAPHIC };

        // Camera Attributes
        glm::vec3 position{ 1.0f, 1.0f, 1.0f };
        glm::vec3 target{ 0.0f, 0.0f, 0.0f };
        glm::vec3 up{ 0.0f, 1.0f, 0.0f };

        glm::vec3 direction;

        glm::vec3 front{ 0.0f, 0.0f, -1.0f };
        glm::vec3 right;
        glm::vec2 mousePos{ 0.0f, 0.0f };
        Ray rayFromMouse{};
        float fovy{ 60.0f };

        Camera() {
            updateCameraVectors();
        }

        // Returns the view matrix
        glm::mat4 getViewMatrix() {
            return glm::lookAt(position, target, up);
        }

        // Return the projection matrix
        glm::mat4 getProjectionMatrix(int screenWidth, int screenHeight) const {
            // Camera Perspective
            if (cameraProjection == CameraProjection::CAMERA_PERSPECTIVE) {
                return glm::perspective(glm::radians(fovy),
                    static_cast<float>(screenWidth) / static_cast<float>(screenHeight),
                    0.1f, 100.0f);
            }
            // Camera Orthographic
            else if (cameraProjection == CameraProjection::CAMERA_ORTHOGRAPHIC) {
                float aspect = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);
                float top = fovy / 2.0f;
                float right = top * aspect;

                return glm::ortho(-right, right, -top, top, 0.1f, 1000.0f);
            }

            return glm::mat4(1.0f);
        }

        Ray getMouseRay(int mouseX, int mouseY, int screenWidth, int screenHeight) {
            if (mousePos != glm::vec2(mouseX, mouseY)) {
                mousePos = glm::vec2(mouseX, mouseY);
            }
            else
                return rayFromMouse;
            float x = (2.0f * static_cast<float>(mouseX)) / static_cast<float>(screenWidth) - 1.0f;
            float y = 1.0f - (2.0f * static_cast<float>(mouseY)) / static_cast<float>(screenHeight);
            float z = 1.0f;

            glm::vec3 deviceCords(x, y, z);
            float aspect = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);
            float top = fovy / 2.0f;
            float right = top * aspect;

            auto matPro = glm::ortho(-right, right, -top, top, 0.01f, 1000.0f);

            glm::vec3 nearPoint = unproject(glm::vec3(deviceCords.x, deviceCords.y, 0.0f), matPro, getViewMatrix());
            glm::vec3 farPoint = unproject(glm::vec3(deviceCords.x, deviceCords.y, 1.0f), matPro, getViewMatrix());
            glm::vec3 cameraPlanePointerPos = unproject(glm::vec3(deviceCords.x, deviceCords.y, -1.0f), matPro, getViewMatrix());

            glm::vec3 rayWorld = glm::normalize(farPoint - nearPoint);

            rayFromMouse.origin = cameraPlanePointerPos;
            rayFromMouse.direction = rayWorld;

            return rayFromMouse;
        }

        glm::vec3 unproject(glm::vec3 source, glm::mat4 projection, glm::mat4 view)
        {
            glm::vec3 result = glm::vec3(0.0f);

            // Calculate unprojected matrix (multiply view matrix by projection matrix) and invert it
            glm::mat4 matViewProj = projection * view;

            // Calculate inverted matrix
            glm::mat4 matViewProjInv = glm::inverse(matViewProj);

            // Create vec4 from source point
            glm::vec4 vec = glm::vec4(source, 1.0f);

            // Multiply vec point by unprojected matrix
            glm::vec4 transformed = matViewProjInv * vec;

            // Normalized world points in vectors
            result.x = transformed.x / transformed.w;
            result.y = transformed.y / transformed.w;
            result.z = transformed.z / transformed.w;

            return result;
        }

        void updateCameraVectors() {
            front = glm::normalize(target - position);
            right = glm::normalize(glm::cross(front, up));
        }

        void draw(glm::mat4) override {};
    };
}
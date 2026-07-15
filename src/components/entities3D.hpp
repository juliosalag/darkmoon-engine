#pragma once 
#include "entity.hpp"
#include "../utils/color.hpp"
#include "../managers/render_manager.hpp"

#include <vector>
#include <GL/gl.h>
#include <glm/glm.hpp>

namespace DarkMoon {
    struct Point3D : Entity {
        glm::vec3 position;
        float pointSize;
        Color color;
        GLuint VBO, VAO;
        GLint colorUniform, modelUniform, viewUniform, projectionUniform, KdUniform, KaUniform, KsUniform, ShininessUniform;
        RenderManager& rm = RenderManager::getInstance();

        Point3D(glm::vec3 pos = { 0.0f, 0.0f, 0.0f }, float pSize = 1.0f, Color c = D_BLACK)
            : position(pos), pointSize(pSize), color(c) {

            // Define vertices for the point
            float vertices[] = {
                position.x, position.y, position.z, 0.0f, 0.0f, 0.0f,
            };

            // Create and configure VAO, VBO
            rm.beginMode3D();
            //rm.useShader(rm.activeLights ? shaders["lights"] : shaders["3D"]);
            rm.useShader(rm.shaders["3D"]);
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            // position attribute
            constexpr size_t VertexSize = 6;
            constexpr size_t VertexByteSize = VertexSize * sizeof(float);
            constexpr size_t ColorOffset = 3 * sizeof(float);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VertexByteSize, (void*)0);
            glEnableVertexAttribArray(0);

            // color attribute
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VertexByteSize, (void*)ColorOffset);
            glEnableVertexAttribArray(1);

            // Get uniform locations
            colorUniform = glGetUniformLocation(rm.getShader()->getIDShader(), "customColor");
            modelUniform = glGetUniformLocation(rm.getShader()->getIDShader(), "model");
            viewUniform = glGetUniformLocation(rm.getShader()->getIDShader(), "view");
            projectionUniform = glGetUniformLocation(rm.getShader()->getIDShader(), "projection");
            KdUniform = glGetUniformLocation(rm.getShader()->getIDShader(), "Kd");
            KaUniform = glGetUniformLocation(rm.getShader()->getIDShader(), "Ka");
            KsUniform = glGetUniformLocation(rm.getShader()->getIDShader(), "Ks");
            ShininessUniform = glGetUniformLocation(rm.getShader()->getIDShader(), "Shininess");
            rm.endMode3D();
        };

        ~Point3D() {
            // Clean up resources
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
        }

        void draw(glm::mat4 transMatrix) override {

            rm.beginMode3D();
            rm.useShader(rm.shaders["3D"]);

            auto nColor = rm.normalizeColor(color);

            // Update vertices for the point
            float vertices[] = {
                position.x, position.y, position.z, nColor.x, nColor.y, nColor.z,
            };

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

            // Colors
            glUniform4fv(colorUniform, 1, glm::value_ptr(nColor));

            // Transform
            glm::mat4 model = transMatrix; // Apply transformation matrix
            glm::mat4 view = rm.m_camera->getViewMatrix();
            glm::mat4 projection = rm.m_camera->getProjectionMatrix(rm.getWidth(), rm.getHeight());
            glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(projection));

            // Bind default texture
            glBindTexture(GL_TEXTURE_2D, rm.defaultMaterial->texture->getIDTexture());

            // Material
            glUniform3fv(KdUniform, 1, glm::value_ptr(rm.defaultMaterial->getDiffuseColor()));
            glUniform3fv(KaUniform, 1, glm::value_ptr(rm.defaultMaterial->getAmbientColor()));
            glUniform3fv(KsUniform, 1, glm::value_ptr(rm.defaultMaterial->getSpecularColor()));
            glUniform1f(ShininessUniform, rm.defaultMaterial->getShininess());

            // Draw the point
            glPointSize(pointSize);
            glBindVertexArray(VAO);
            glDrawArrays(GL_POINTS, 0, 1);
            glPointSize(1.0f);

            // Unbind default texture
            glBindTexture(GL_TEXTURE_2D, 0);

            rm.endMode3D();
        };
    };

    struct Line3D : Entity {
        glm::vec3 startPos, endPos;
        float lineSize;
        Color color;

        Line3D(glm::vec3 sP = { 0.0f, 0.0f, 0.0f }, glm::vec3 eP = { 1.0f, 0.0f, 0.0f }, float lSize = 1.0f, Color c = D_BLACK)
            : startPos(sP), endPos(eP), lineSize(lSize), color(c) {};

        void draw(glm::mat4 transMatrix) override {
            RenderManager& rm = RenderManager::getInstance();

            rm.beginMode3D();
            //rm.useShader(rm.shaders["3Dv2"]);

            auto nColor = rm.normalizeColor(color);

            // Define vertices for the line
            float vertices[] = {
                // Position                           // Color
                startPos.x, startPos.y, startPos.z,   nColor.x, nColor.y, nColor.z,
                  endPos.x,   endPos.y,   endPos.z,   nColor.x, nColor.y, nColor.z,
            };

            // Create and configure VAO, VBO
            GLuint VBO, VAO;
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            // position attribute
            constexpr size_t VertexSize = 6;
            constexpr size_t VertexByteSize = VertexSize * sizeof(float);
            constexpr size_t ColorOffset = 3 * sizeof(float);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VertexByteSize, (void*)0);
            glEnableVertexAttribArray(0);

            // color attribute
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VertexByteSize, (void*)ColorOffset);
            glEnableVertexAttribArray(1);

            // Colors
            GLint colorUniform = glGetUniformLocation(rm.getShader()->getIDShader(), "customColor");
            glUniform4fv(colorUniform, 1, glm::value_ptr(nColor));

            // Transform
            glm::mat4 model = transMatrix; // Apply Transformation Matrix
            glm::mat4 view = rm.m_camera->getViewMatrix();
            glm::mat4 projection = rm.m_camera->getProjectionMatrix(rm.getWidth(), rm.getHeight());
            glUniformMatrix4fv(glGetUniformLocation(rm.getShader()->getIDShader(), "model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(glGetUniformLocation(rm.getShader()->getIDShader(), "view"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(rm.getShader()->getIDShader(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

            // Bind default texture
            glBindTexture(GL_TEXTURE_2D, rm.defaultMaterial->texture->getIDTexture());

            // Material
            glUniform3fv(glGetUniformLocation(rm.getShader()->getIDShader(), "Kd"), 1, glm::value_ptr(rm.defaultMaterial->getDiffuseColor()));
            glUniform3fv(glGetUniformLocation(rm.getShader()->getIDShader(), "Ka"), 1, glm::value_ptr(rm.defaultMaterial->getAmbientColor()));
            glUniform3fv(glGetUniformLocation(rm.getShader()->getIDShader(), "Ks"), 1, glm::value_ptr(rm.defaultMaterial->getSpecularColor()));
            glUniform1f(glGetUniformLocation(rm.getShader()->getIDShader(), "Shininess"), rm.defaultMaterial->getShininess());

            // Draw the line
            glLineWidth(lineSize);
            glBindVertexArray(VAO);
            glDrawArrays(GL_LINES, 0, 2);
            glLineWidth(1.0f);

            // Unbind default texture
            glBindTexture(GL_TEXTURE_2D, 0);

            // Clean up resources
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);

            rm.endMode3D();
        }
    };

    struct Grid : Entity {
        int slices;
        float spacing;
        Color color;

        Grid(int sli = 10, float spac = 1.0f, Color c = D_GRAY)
            : slices(sli), spacing(spac), color(c) {};

        void draw(glm::mat4 transMatrix) override {
            RenderManager& rm = RenderManager::getInstance();

            rm.beginMode3D();

            auto nColor = rm.normalizeColor(color);

            // Define vertices for the grid
            std::vector<float> vertices;
            float halfSize = static_cast<float>(slices) * spacing * 0.5f;
            for (int i = 0; i < slices + 1; ++i) {
                float x = static_cast<float>(i) * spacing - halfSize;
                vertices.push_back(x); vertices.push_back(0.0f); vertices.push_back(-halfSize);
                vertices.push_back(nColor.x); vertices.push_back(nColor.y); vertices.push_back(nColor.z);

                vertices.push_back(x); vertices.push_back(0.0f); vertices.push_back(halfSize);
                vertices.push_back(nColor.x); vertices.push_back(nColor.y); vertices.push_back(nColor.z);

                float z = static_cast<float>(i) * spacing - halfSize;
                vertices.push_back(-halfSize); vertices.push_back(0.0f); vertices.push_back(z);
                vertices.push_back(nColor.x); vertices.push_back(nColor.y); vertices.push_back(nColor.z);

                vertices.push_back(halfSize); vertices.push_back(0.0f); vertices.push_back(z);
                vertices.push_back(nColor.x); vertices.push_back(nColor.y); vertices.push_back(nColor.z);
            }

            // Create and configure VAO, VBO
            GLuint VBO, VAO;
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

            // position attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            // color attribute
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            // Colors
            GLint colorUniform = glGetUniformLocation(rm.getShader()->getIDShader(), "customColor");
            glUniform4fv(colorUniform, 1, glm::value_ptr(nColor));

            // Transform
            glm::mat4 model = transMatrix; // Apply Transformation Matrix
            glm::mat4 view = rm.m_camera->getViewMatrix();
            glm::mat4 projection = rm.m_camera->getProjectionMatrix(rm.getWidth(), rm.getHeight());
            glUniformMatrix4fv(glGetUniformLocation(rm.getShader()->getIDShader(), "model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(glGetUniformLocation(rm.getShader()->getIDShader(), "view"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(rm.getShader()->getIDShader(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

            // Bind default texture
            glBindTexture(GL_TEXTURE_2D, rm.defaultMaterial->texture->getIDTexture());

            // Material
            glUniform3fv(glGetUniformLocation(rm.getShader()->getIDShader(), "Kd"), 1, glm::value_ptr(rm.defaultMaterial->getDiffuseColor()));
            glUniform3fv(glGetUniformLocation(rm.getShader()->getIDShader(), "Ka"), 1, glm::value_ptr(rm.defaultMaterial->getAmbientColor()));
            glUniform3fv(glGetUniformLocation(rm.getShader()->getIDShader(), "Ks"), 1, glm::value_ptr(rm.defaultMaterial->getSpecularColor()));
            glUniform1f(glGetUniformLocation(rm.getShader()->getIDShader(), "Shininess"), rm.defaultMaterial->getShininess());

            // Draw the grid
            glBindVertexArray(VAO);
            glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(vertices.size() / 6));

            // Unbind default texture
            glBindTexture(GL_TEXTURE_2D, 0);

            // Clean up resources
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);

            rm.endMode3D();
        };
    };

    struct Plane : Entity {
        glm::vec3 centerPos;
        glm::vec2 size;
        Color color;

        Plane(glm::vec3 cP = { 0.0f, 0.0f, 0.0f }, glm::vec2 s = { 10.0f, 10.0f }, Color c = D_GRAY)
            : centerPos(cP), size(s), color(c) {};

        void draw(glm::mat4 transMatrix) override {
            RenderManager& rm = RenderManager::getInstance();

            rm.beginMode3D();

            auto nColor = rm.normalizeColor(color);

            // Calculate half size for convenience
            glm::vec2 halfSize = size * 0.5f;

            // Define vertices and indices for the plane
            float vertices[] = {
                // positions                                                       // colors
                centerPos.x - halfSize.x, centerPos.y, centerPos.z - halfSize.y,   nColor.x, nColor.y, nColor.z,
                centerPos.x + halfSize.x, centerPos.y, centerPos.z - halfSize.y,   nColor.x, nColor.y, nColor.z,
                centerPos.x - halfSize.x, centerPos.y, centerPos.z + halfSize.y,   nColor.x, nColor.y, nColor.z,
                centerPos.x + halfSize.x, centerPos.y, centerPos.z + halfSize.y,   nColor.x, nColor.y, nColor.z,
            };

            GLuint indices[] = { 0, 1, 2, 1, 2, 3 };

            // Create and configure VAO, VBO, and EBO
            GLuint VBO, VAO, EBO;
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            // position attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            // color attribute
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            // Colors
            GLint colorUniform = glGetUniformLocation(rm.getShader()->getIDShader(), "customColor");
            glUniform4fv(colorUniform, 1, glm::value_ptr(nColor));

            // Transform
            glm::mat4 model = transMatrix;
            glm::mat4 view = rm.m_camera->getViewMatrix();
            glm::mat4 projection = rm.m_camera->getProjectionMatrix(rm.getWidth(), rm.getHeight());
            glUniformMatrix4fv(glGetUniformLocation(rm.getShader()->getIDShader(), "model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(glGetUniformLocation(rm.getShader()->getIDShader(), "view"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(rm.getShader()->getIDShader(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

            // Bind default texture
            glBindTexture(GL_TEXTURE_2D, rm.defaultMaterial->texture->getIDTexture());

            // Material
            glUniform3fv(glGetUniformLocation(rm.getShader()->getIDShader(), "Kd"), 1, glm::value_ptr(rm.defaultMaterial->getDiffuseColor()));
            glUniform3fv(glGetUniformLocation(rm.getShader()->getIDShader(), "Ka"), 1, glm::value_ptr(rm.defaultMaterial->getAmbientColor()));
            glUniform3fv(glGetUniformLocation(rm.getShader()->getIDShader(), "Ks"), 1, glm::value_ptr(rm.defaultMaterial->getSpecularColor()));
            glUniform1f(glGetUniformLocation(rm.getShader()->getIDShader(), "Shininess"), rm.defaultMaterial->getShininess());

            // Draw the plane
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            // Unbind default texture
            glBindTexture(GL_TEXTURE_2D, 0);

            // Clean up resources
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);

            rm.endMode3D();
        };
    };

    struct Cube : Entity {
        glm::vec3 position;
        glm::vec3 size;
        Color color;

        Cube(glm::vec3 pos = { 0.0f, 0.0f, 0.0f }, glm::vec3 s = { 1.0f, 1.0f, 1.0f }, Color c = D_BLACK)
            : position(pos), size(s), color(c) {};

        void draw(glm::mat4 transMatrix) override {
            RenderManager& rm = RenderManager::getInstance();

            rm.beginMode3D();
            rm.useShader(rm.shaders["3D"]);

            auto nColor = rm.normalizeColor(color);

            // Define vertices and indices for the plane
            float halfSizeX = size.x / 2.0f;
            float halfSizeY = size.y / 2.0f;
            float halfSizeZ = size.z / 2.0f;

            float vertices[] = {
                // positions                        // colors
                -halfSizeX, -halfSizeY, halfSizeZ,  nColor.x, nColor.y, nColor.z, nColor.w,
                halfSizeX, -halfSizeY, halfSizeZ,  nColor.x, nColor.y, nColor.z, nColor.w,
                halfSizeX,  halfSizeY, halfSizeZ,  nColor.x, nColor.y, nColor.z, nColor.w,
                -halfSizeX,  halfSizeY, halfSizeZ,  nColor.x, nColor.y, nColor.z, nColor.w,
                -halfSizeX, -halfSizeY, -halfSizeZ, nColor.x, nColor.y, nColor.z, nColor.w,
                halfSizeX, -halfSizeY, -halfSizeZ, nColor.x, nColor.y, nColor.z, nColor.w,
                halfSizeX,  halfSizeY, -halfSizeZ, nColor.x, nColor.y, nColor.z, nColor.w,
                -halfSizeX,  halfSizeY, -halfSizeZ, nColor.x, nColor.y, nColor.z, nColor.w,
            };
            GLuint indices[] = {
                0, 1, 2, 2, 3, 0,
                4, 5, 6, 6, 7, 4,
                0, 1, 5, 5, 4, 0,
                1, 2, 6, 6, 5, 1,
                2, 3, 7, 7, 6, 2,
                3, 0, 4, 4, 7, 3
            };

            // Create and configure VAO, VBO, and EBO
            GLuint VBO, VAO, EBO;
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            // Configurar atributos de vértice
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            glBindVertexArray(0);

            // Set the uniform color in the shader
            GLint colorUniform = glGetUniformLocation(rm.getShader()->getIDShader(), "customColor");
            glUseProgram(rm.getShader()->getIDShader());
            glUniform4fv(colorUniform, 1, glm::value_ptr(nColor));

            // Transform
            glm::mat4 model = transMatrix;
            glm::mat4 view = rm.m_camera->getViewMatrix();
            glm::mat4 projection = rm.m_camera->getProjectionMatrix(rm.getWidth(), rm.getHeight());

            glUniformMatrix4fv(glGetUniformLocation(rm.getShader()->getIDShader(), "model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(glGetUniformLocation(rm.getShader()->getIDShader(), "view"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(rm.getShader()->getIDShader(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

            // Bind default texture
            glBindTexture(GL_TEXTURE_2D, rm.defaultMaterial->texture->getIDTexture());

            // Material
            glUniform3fv(glGetUniformLocation(rm.getShader()->getIDShader(), "Kd"), 1, glm::value_ptr(rm.defaultMaterial->getDiffuseColor()));
            glUniform3fv(glGetUniformLocation(rm.getShader()->getIDShader(), "Ka"), 1, glm::value_ptr(rm.defaultMaterial->getAmbientColor()));
            glUniform3fv(glGetUniformLocation(rm.getShader()->getIDShader(), "Ks"), 1, glm::value_ptr(rm.defaultMaterial->getSpecularColor()));
            glUniform1f(glGetUniformLocation(rm.getShader()->getIDShader(), "Shininess"), rm.defaultMaterial->getShininess());

            // Draw cube
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            // Unbind default texture
            glBindTexture(GL_TEXTURE_2D, 0);

            // Clean up resources
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);

            rm.endMode3D();
        };
    };

    struct CubeWires : Entity {
        glm::vec3 position;
        glm::vec3 size;
        Color color;

        CubeWires(glm::vec3 pos = { 0.0f, 0.0f, 0.0f }, glm::vec3 s = { 1.0f, 1.0f, 1.0f }, Color c = D_BLACK)
            : position(pos), size(s), color(c) {};

        void draw(glm::mat4 transMatrix) override {
            RenderManager& rm = RenderManager::getInstance();

            rm.beginMode3D();

            auto nColor = rm.normalizeColor(color);

            // Define vertices and indices for the wireframe cube
            float halfSizeX = size.x / 2.0f;
            float halfSizeY = size.y / 2.0f;
            float halfSizeZ = size.z / 2.0f;

            float vertices[] = {
                // positions                        // colors
                -halfSizeX, -halfSizeY, halfSizeZ,  nColor.x, nColor.y, nColor.z, nColor.w,
                halfSizeX, -halfSizeY, halfSizeZ,  nColor.x, nColor.y, nColor.z, nColor.w,
                halfSizeX,  halfSizeY, halfSizeZ,  nColor.x, nColor.y, nColor.z, nColor.w,
                -halfSizeX,  halfSizeY, halfSizeZ,  nColor.x, nColor.y, nColor.z, nColor.w,
                -halfSizeX, -halfSizeY, -halfSizeZ, nColor.x, nColor.y, nColor.z, nColor.w,
                halfSizeX, -halfSizeY, -halfSizeZ, nColor.x, nColor.y, nColor.z, nColor.w,
                halfSizeX,  halfSizeY, -halfSizeZ, nColor.x, nColor.y, nColor.z, nColor.w,
                -halfSizeX,  halfSizeY, -halfSizeZ, nColor.x, nColor.y, nColor.z, nColor.w,
            };
            GLuint indices[] = {
                0, 1, 1, 2, 2, 3, 3, 0,
                4, 5, 5, 6, 6, 7, 7, 4,
                0, 4, 1, 5, 2, 6, 3, 7
            };

            // Create and configure VAO, VBO, and EBO
            GLuint VBO, VAO, EBO;
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            // Set up vertex attribute pointers
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            glBindVertexArray(0);

            // Set wireframe mode
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

            // Set the uniform color in the shader
            GLint colorUniform = glGetUniformLocation(rm.getShader()->getIDShader(), "customColor");
            glUseProgram(rm.getShader()->getIDShader());
            glUniform4fv(colorUniform, 1, glm::value_ptr(nColor));

            // Transform
            glm::mat4 model = transMatrix;
            glm::mat4 view = rm.m_camera->getViewMatrix();
            glm::mat4 projection = rm.m_camera->getProjectionMatrix(rm.getWidth(), rm.getHeight());

            glUniformMatrix4fv(glGetUniformLocation(rm.getShader()->getIDShader(), "model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(glGetUniformLocation(rm.getShader()->getIDShader(), "view"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(rm.getShader()->getIDShader(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

            // Bind default texture
            glBindTexture(GL_TEXTURE_2D, rm.defaultMaterial->texture->getIDTexture());

            // Material
            glUniform3fv(glGetUniformLocation(rm.getShader()->getIDShader(), "Kd"), 1, glm::value_ptr(rm.defaultMaterial->getDiffuseColor()));
            glUniform3fv(glGetUniformLocation(rm.getShader()->getIDShader(), "Ka"), 1, glm::value_ptr(rm.defaultMaterial->getAmbientColor()));
            glUniform3fv(glGetUniformLocation(rm.getShader()->getIDShader(), "Ks"), 1, glm::value_ptr(rm.defaultMaterial->getSpecularColor()));
            glUniform1f(glGetUniformLocation(rm.getShader()->getIDShader(), "Shininess"), rm.defaultMaterial->getShininess());

            // Draw the wireframe cube
            glBindVertexArray(VAO);
            glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            // Unbind default texture
            glBindTexture(GL_TEXTURE_2D, 0);

            // Reset to fill mode
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            // Clean up resources
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);

            rm.endMode3D();
        };
    };

    struct Vertex2 {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 textCoords;
    };

    struct Billboard : Entity {
    private:
        GLuint m_VAO{}, m_VBO{}, m_EBO{};
        std::vector<Vertex2> m_vertices;
        std::vector<uint16_t> m_indices;
    public:
        Texture* texture = { nullptr };
        glm::vec3 position{ 0, 0, 0 };
        glm::vec2 size{ 10, 10 };
        Color color = { D_WHITE };

        Billboard(Texture* text, glm::vec3 pos, glm::vec2 siz)
            : texture(text), position(pos), size(siz) {
            glGenVertexArrays(1, &m_VAO);
            glGenBuffers(1, &m_VBO);
            glGenBuffers(1, &m_EBO);

            setupVerticesAndIndices();
        };

        ~Billboard() {
            // Clean up resources
            glDeleteVertexArrays(1, &m_VAO);
            glDeleteBuffers(1, &m_VBO);
            glDeleteBuffers(1, &m_EBO);
        };

        void setupVerticesAndIndices() {
            // Define vertices and indices for the billboard
            glm::vec2 halfSize = size * 0.5f;
            m_vertices = {
                {{-halfSize.x, 0.0f, -halfSize.y},   {0.0f, 1.0f, 0.0f},   {0.0f, 1.0f}},
                {{ halfSize.x, 0.0f, -halfSize.y},   {0.0f, 1.0f, 0.0f},   {1.0f, 1.0f}},
                {{-halfSize.x, 0.0f,  halfSize.y},   {0.0f, 1.0f, 0.0f},   {0.0f, 0.0f}},
                {{ halfSize.x, 0.0f,  halfSize.y},   {0.0f, 1.0f, 0.0f},   {1.0f, 0.0f}},
            };
            m_indices = { 0, 1, 2, 1, 2, 3 };
        }

        void draw(glm::mat4) override {
            RenderManager& rm = RenderManager::getInstance();

            rm.beginMode3D();
            //rm.useShader(rm.shaders["3Dv2"]);

            glm::vec3 camDirection = glm::normalize(rm.m_camera->position - position);
            glm::vec3 billboardDirection = glm::vec3(0.0f, 0.0f, -1.0f);
            float angleY = static_cast<float>(glm::degrees(atan2(camDirection.x, camDirection.z) - atan2(billboardDirection.x, billboardDirection.z)));
            float angleX = static_cast<float>(glm::degrees(atan2(camDirection.y, camDirection.z) - atan2(billboardDirection.y, billboardDirection.z)));

            glm::mat4 billboardRotation = glm::rotate(glm::mat4(1.0f), glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));
            billboardRotation = glm::rotate(billboardRotation, glm::radians(angleX), glm::vec3(1.0f, 0.0f, 0.0f));

            glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position) * billboardRotation;

            auto nColor = rm.normalizeColor(color);

            setupVerticesAndIndices();

            glBindVertexArray(m_VAO);

            glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
            glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex2), m_vertices.data(), GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(uint16_t), m_indices.data(), GL_STATIC_DRAW);

            // position attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2), (void*)offsetof(Vertex2, position));
            glEnableVertexAttribArray(0);

            // normal attribute
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2), (void*)offsetof(Vertex2, normal));
            glEnableVertexAttribArray(1);

            // texture coordinate attribute
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2), (void*)offsetof(Vertex2, textCoords));
            glEnableVertexAttribArray(2);

            // Colors
            GLint colorUniform = glGetUniformLocation(rm.getShader()->getIDShader(), "customColor");
            glUniform4fv(colorUniform, 1, glm::value_ptr(nColor));

            // Transform
            glm::mat4 view = rm.m_camera->getViewMatrix();
            glm::mat4 projection = rm.m_camera->getProjectionMatrix(rm.getWidth(), rm.getHeight());
            glUniformMatrix4fv(glGetUniformLocation(rm.getShader()->getIDShader(), "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
            glUniformMatrix4fv(glGetUniformLocation(rm.getShader()->getIDShader(), "view"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(rm.getShader()->getIDShader(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

            // Bind texture
            if (!texture)
                texture = rm.defaultMaterial->texture;
            glBindTexture(GL_TEXTURE_2D, texture->getIDTexture());

            // Draw the plane
            glBindVertexArray(m_VAO);
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_SHORT, 0);

            // Unbind texture
            glBindTexture(GL_TEXTURE_2D, 0);
            glActiveTexture(GL_TEXTURE0);

            rm.endMode3D();
        }
    };
}
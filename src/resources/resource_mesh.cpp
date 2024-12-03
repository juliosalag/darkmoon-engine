#include "resource_mesh.hpp"

namespace DarkMoon {
    Mesh::Mesh(std::size_t id, std::vector<Vertex> vertices, std::vector<uint16_t> indices, Material* material, std::string name, bool hasBones) {
        this->m_id = id;
        this->vertices = vertices;
        this->indices = indices;
        this->material = material;
        this->m_name = name;
        this->hasBones = hasBones;
    }

    bool Mesh::load(const char* filePath) {

        setupMesh();

        isLoaded() ? std::cout << "Load a mesh (ID: " << m_id << ") -> " << filePath << std::endl : std::cout << "Error loading a mesh" << std::endl;

        return isLoaded();
    }

    void Mesh::unload() {
        // Delete buffers
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
        m_isLoad = false;
        std::cout << "Unload a mesh (ID: " << m_id << ")" << std::endl;
    }

    void Mesh::setupMesh() {
        // Generate vertex array and buffers
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);
        //glGenBuffers(1, &m_BAO);

        // Bind vertex array
        glBindVertexArray(m_VAO);

        // Bind buffer and fill with vertex data
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        // Bind buffer and fill with index data
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint16_t), &indices[0], GL_STATIC_DRAW);

        //Bind buffer and fill with nones data
        // glBindBuffer(GL_ARRAY_BUFFER,m_BAO);
        // glBufferData(GL_ARRAY_BUFFER,sizeof(num_bones[0]) * num_bones.size(), &num_bones[0], GL_STATIC_DRAW);

        // Enable and specify vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // Enable and specify vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        // Enable and specify vertex coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textCoords));
        // if (hasBones)
        // {
            // Enable and specify tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
        // Enable and specify bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, BiTangent));
        // Enable and specify bones ids
        glEnableVertexAttribArray(5);
        glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, boneIDs));
        //weights
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, weights));
        // }

        // Unbind vertex array
        glBindVertexArray(0);

        if (m_VAO != 0 && m_VBO != 0 && m_EBO != 0)
            m_isLoad = true;
    }

    void Mesh::draw(glm::mat4 transMatrix, Color color) {
        RenderManager& rm = RenderManager::getInstance();
        rm.beginMode3D();
        //glEnable(GL_CULL_FACE);
        auto& shaders = rm.shaders;
        rm.useShader(shaders["3D"]);

        // Asume que 'shaderProgram' es el ID de tu programa de shader
        GLint isBoneAnimationEnabledLocation = glGetUniformLocation(rm.getShader()->getIDShader(), "isBoneAnimationEnabled");
        if (hasBones && animID != std::numeric_limits<std::size_t>::max() && !am.isAnimationsEmpty())
        {
            auto transforms = am.GetFinalBoneMatrices(animID);
            glUniform1i(isBoneAnimationEnabledLocation, GL_TRUE);

#ifdef _WIN32
            float degrees = 0.f;
            if (!rotateBones)
                degrees = -90.f;
#else
            float degrees = 90.f;
            if (!rotateBones)
                degrees = 0.f;
#endif

            for (std::size_t i = 0; i < transforms.size(); ++i) {
                // Rotar la matriz en 90 grados alrededor del eje Z
                glm::mat4 rotatedTransform = glm::rotate(transforms[i], glm::radians(degrees), glm::vec3(0.0f, 0.0f, 1.0f));

                std::string boneStr = "finalBonesMatrices[" + std::to_string(i) + "]";
                int boneLocation = glGetUniformLocation(rm.getShader()->getIDShader(), boneStr.c_str());
                glUniformMatrix4fv(boneLocation, 1, GL_FALSE, glm::value_ptr(rotatedTransform));
            }
        }
        else
            glUniform1i(isBoneAnimationEnabledLocation, GL_FALSE);

        // Set the uniform color in the shader
        GLint colorUniform = glGetUniformLocation(rm.getShader()->getIDShader(), "customColor");
        glUniform4fv(colorUniform, 1, glm::value_ptr(rm.normalizeColor(color)));

        // Transform
        glm::mat4 model = transMatrix;
        glm::mat4 view = rm.m_camera->getViewMatrix();
        glm::mat4 projection = rm.m_camera->getProjectionMatrix(rm.getWidth(), rm.getHeight());

        glUniformMatrix4fv(glGetUniformLocation(rm.getShader()->getIDShader(), "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(rm.getShader()->getIDShader(), "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(rm.getShader()->getIDShader(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // Texture
        if (!material->texture)
            material->texture = rm.defaultMaterial->texture;
        glBindTexture(GL_TEXTURE_2D, material->texture->getIDTexture());

        // Material
        //std::cout << "--------------------------\n";
        //std::cout << material->getDiffuseColor().x << " | " << material->getDiffuseColor().y << " | " << material->getDiffuseColor().z << "\n";
        //std::cout << material->getAmbientColor().x << " | " << material->getAmbientColor().y << " | " << material->getAmbientColor().z << "\n";
        //std::cout << material->getSpecularColor().x << " | " << material->getSpecularColor().y << " | " << material->getSpecularColor().z << "\n";

        glUniform3fv(glGetUniformLocation(rm.getShader()->getIDShader(), "Kd"), 1, glm::value_ptr(material->getDiffuseColor()));
        glUniform3fv(glGetUniformLocation(rm.getShader()->getIDShader(), "Ka"), 1, glm::value_ptr(material->getAmbientColor()));
        glUniform3fv(glGetUniformLocation(rm.getShader()->getIDShader(), "Ks"), 1, glm::value_ptr(material->getSpecularColor()));
        glUniform1f(glGetUniformLocation(rm.getShader()->getIDShader(), "Shininess"), material->getShininess());

        // Draw the triangle of mesh
        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_SHORT, 0);
        glBindVertexArray(0);

        // Unbind default texture
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE0);

        rm.endMode3D();
        //glDisable(GL_CULL_FACE);
    }

    void Mesh::drawLines(glm::mat4 transMatrix, Color color) {

        RenderManager& rm = RenderManager::getInstance();

        rm.beginMode3D();

        GLint colorUniform = glGetUniformLocation(rm.getShader()->getIDShader(), "customColor");
        glUseProgram(rm.getShader()->getIDShader());
        glUniform4fv(colorUniform, 1, glm::value_ptr(rm.normalizeColor(color)));

        // Transform
        glm::mat4 model = transMatrix;
        glm::mat4 view = rm.m_camera->getViewMatrix();
        glm::mat4 projection = rm.m_camera->getProjectionMatrix(rm.getWidth(), rm.getHeight());

        glUniformMatrix4fv(glGetUniformLocation(rm.getShader()->getIDShader(), "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(rm.getShader()->getIDShader(), "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(rm.getShader()->getIDShader(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // Draw the lines of mesh
        glBindVertexArray(m_VAO);
        glDrawElements(GL_LINES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_SHORT, 0);
        glBindVertexArray(0);

        rm.endMode3D();
    }
}
#pragma once
#include "resource.hpp"
#include "resource_texture.hpp"
#include "resource_material.hpp"
#include "../utils/color.hpp"
#include "../managers/render_manager.hpp"
#include "../managers/animator_manager.hpp"

#include <vector>
#include <memory>
#include <array>
#include <map>
#include <cstdint>
#include <glm/glm.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace DarkMoon {
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 textCoords;
        //tangent
        glm::vec3 Tangent;
        //bitangent
        glm::vec3 BiTangent;
        //bone indexes which will influence this vertex
        glm::ivec4 boneIDs;
        //weights from each bone
        glm::vec4 weights;

        // void AddBoneata(uint BondeID, float weight) {
        //     for (uint i = 0; i < MAX_NUM_BONES_PER_VERTEX; i++) {
        //         if (m_Weights[i] == 0.0) {
        //             m_BonesIDs[i] = BondeID;
        //             m_Weights[i] = weight;
        //             return;
        //         }
        //     }
        //     //si salta, aumentar ,max_nume_bones_per_vertex
        //     assert(0);
        // }
    };

    struct Mesh : public Resource {
    public:
        // Mesh data
        std::vector<Vertex> vertices;
        std::vector<uint16_t> indices;
        bool hasBones;
        std::size_t animID{ std::numeric_limits<std::size_t>::max() };
        bool rotateBones{ true };
        //std::vector<VertexBoneData>num_bones{};
        Material* material;
        AnimationManager& am = AnimationManager::getInstance();

        Mesh(std::size_t, std::vector<Vertex>, std::vector<uint16_t>, Material*, std::string name = "", bool hasBones = false);
        ~Mesh() { unload(); };

        bool load(const char*) override;
        void unload() override;

        void draw(glm::mat4 transMatrix, Color color = D_GRAY);
        void drawLines(glm::mat4 transMatrix, Color color = D_BLACK);

    private:
        // Render data
        GLuint m_VAO; // Vertex Array Object
        GLuint m_VBO; // Vertex Buffer Object
        GLuint m_EBO; // Element Buffer Object
        // GLuint m_BAO; // Bones Array Object
        std::string m_name;

        void setupMesh();
    };
}
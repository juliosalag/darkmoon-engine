#pragma once
#include "node.hpp"
#include "entity.hpp"

#include "../resources/resource_mesh.hpp"
#include "../resources/resource_material.hpp"
#include "../managers/resource_manager.hpp"
#include "../managers/render_manager.hpp"
#include "../utils/BoneInfo.hpp"

#include <vector>
#include <map>
#include <string>
#include <array>
#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace DarkMoon {
    struct Model : Entity {

    private:
        const char* m_name;
        std::vector<Mesh*> m_meshes;
        bool m_loaded{ false };

        //#############Aniamtion data#############################
        //vector of vertex data influence by bone
        // std::vector<Vertex> m_Bones{};
        // // //num of vertex
        // std::vector<int> mesh_base_vertex{};
        // Name and id of each bone
        // void SetVertexBoneDataToDefault(Vertex& vertex) {
        //     for (int i = 0; i < MAX_NUM_BONES_PER_VERTEX;i++) {
        //         vertex.m_BonesIDs[i] = -1;
        //         vertex.m_Weights[i] = 0.0f;
        //     }
        // }

        // void SetVertexBoneData(Vertex& vertex, int bondeID, float weight) {
        //     for (int i = 0; i < MAX_NUM_BONES_PER_VERTEX;i++) {
        //         if (vertex.m_BonesIDs[i] < 0) {
        //             vertex.m_Weights[i] = weight;
        //             vertex.m_BonesIDs[i] = bondeID;
        //             break;
        //         }
        //     }
        // }
        void processBone(std::vector<glm::ivec4>& boneIDs_all, std::vector<glm::vec4>& weights_all, aiMesh* mesh, const aiScene* scene);
        // int getBoneID(const aiBone* pBone);
        //##############################################################


        void processNode(aiNode*, const aiScene*, ResourceManager& rm);
        void processMesh(aiMesh*, aiMaterial*, const aiScene*, ResourceManager& rm);
        Material* processMaterial(aiMaterial*, ResourceManager& rm);
        void processTextures(aiMaterial*, Material*, ResourceManager& rm);



    public:
        static constexpr std::size_t maxID = std::numeric_limits<std::size_t>::max();
        std::vector<BoneInfo> boneVector{};
        int m_BoneCounter{ 0 };
        std::size_t animID{ std::numeric_limits<std::size_t>::max() };
        bool hasBones{ false }, rotateBones{ false };

        auto& getMeshes() { return m_meshes; };
        auto& getboneInfoMap() { return boneVector; };
        int& getBoneCount() { return m_BoneCounter; };
        void ReadMissingBones(const aiAnimation* animation);
        glm::mat4 getBoneTransform(const std::string& name);

        Color color = D_WHITE;
        bool drawModel{ true }, drawWires{ false };

        void load(const char* filePath, ResourceManager& rm);
        void unload() override;
        void draw(glm::mat4) override;

        bool isLoaded() { return m_loaded; };
        void setColor(Color c) { color = c; };
    };
}
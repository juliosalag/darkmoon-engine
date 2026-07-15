#pragma once

#include <assimp/postprocess.h>
#include "bone.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "../utils/BoneInfo.hpp"

#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include <string>
#include <vector>
#include <map>
#include <unordered_map>

struct AssimpNodeData
{
    glm::mat4 transformation{};
    std::string name{};
    std::size_t childrenCount{};
    std::vector<AssimpNodeData> children{};
    Bone* bone = nullptr;
};

static glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4* from);

struct Animation
{
    Animation() = default;

    Animation(const std::string& animationPath, std::vector<BoneInfo>& modelBones)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        assert(scene && scene->mRootNode);
        scene->mRootNode->mTransformation = aiMatrix4x4();
        if (scene->mNumAnimations == 0)
            return;
        aiAnimation* animation = scene->mAnimations[0];
        duration = (float)animation->mDuration;
        tps = (float)animation->mTicksPerSecond;
        generateBoneTree(&rootNode, scene->mRootNode);
        // Reset all root transformations
        rootNode.transformation = glm::mat4(1.0f);
        loadIntermediateBones(animation, modelBones);
    }

    Animation(const aiScene* scene, std::vector<BoneInfo>& modelBones, std::size_t animID)
    {
        scene->mRootNode->mTransformation = aiMatrix4x4();
        aiAnimation* animation = scene->mAnimations[animID];
        duration = (float)animation->mDuration;
        tps = (float)animation->mTicksPerSecond;
        generateBoneTree(&rootNode, scene->mRootNode);
        // Reset all root transformations
        rootNode.transformation = glm::mat4(1.0f);
        loadIntermediateBones(animation, modelBones);
    }

    Bone* findBone(const std::string& name)
    {
        auto search = bones.find(name);
        if (search != bones.end())
            return &search->second;
        return nullptr;
    }


    inline float getTicksPerSecond() { return tps; }

    inline float getDuration() { return duration; }

    inline float getSekDuration() { return duration / tps; }

    inline AssimpNodeData* getRootNode() { return &rootNode; }

    inline const std::vector<BoneInfo>& getBoneProps() { return boneProps; }

private:
    float duration = 0.0f;
    float tps = 0.0f;
    std::unordered_map<std::string, Bone> bones{};
    std::unordered_map<std::string, std::vector<glm::mat4>> preCalculateTransforms{};
    AssimpNodeData rootNode;
    std::vector<BoneInfo> boneProps;

    void loadIntermediateBones(const aiAnimation* animation, std::vector<BoneInfo>& modelBones)
    {
        auto& boneProps = modelBones;

        for (std::size_t i = 0; i < animation->mNumChannels; i++)
        {
            auto channel = animation->mChannels[i];
            std::string boneName = channel->mNodeName.data;
            int boneId = -1;

            for (std::size_t i = 0; i < boneProps.size(); i++) {
                if (boneProps[i].name == boneName) {
                    boneId = static_cast<int>(i);
                    break;
                }
            }

            if (boneProps.size() < 100) {
                if (boneId == -1) {
                    BoneInfo boneProp;
                    boneProp.name = boneName;
                    boneProps.push_back(boneProp);
                    boneId = static_cast<int>(boneProps.size() - 1);
                }
            }
            bones.insert({ boneName, Bone(channel->mNodeName.data, boneId, channel) });
        }

        this->boneProps = boneProps;
    }

    void generateBoneTree(AssimpNodeData* parent, const aiNode* src)
    {
        assert(src);

        parent->name = src->mName.data;
        parent->transformation = aiMatrix4x4ToGlm(&src->mTransformation);
        parent->childrenCount = src->mNumChildren;

        for (unsigned int i = 0; i < src->mNumChildren; i++)
        {
            AssimpNodeData newData;
            generateBoneTree(&newData, src->mChildren[i]);
            parent->children.push_back(newData);
        }
    }
};
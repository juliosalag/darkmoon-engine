#pragma once

#include <array>
#include "../utils/animation.hpp"

struct Anim
{
    Anim()
    {
        m_FinalBoneMatrices.reserve(100);

        for (int i = 0; i < 100; i++)
            m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
    }

    void reset()
    {
        m_CurrentAnimation = nullptr;
        m_QueueAnimation = nullptr;
        m_NextAnimation = nullptr;
        m_CurrentTime = 0.0f;
        interpolating = false;
        haltTime = 0.0f;
        interTime = 0.0f;
    }

    Animation* m_CurrentAnimation{ nullptr };
    Animation* m_QueueAnimation{ nullptr };
    Animation* m_NextAnimation{ nullptr };
    float m_CurrentTime{ 0.0f };
    bool interpolating{ false };
    float haltTime{ 0.0f };
    float interTime{ 0.0f };
    std::vector<glm::mat4> m_FinalBoneMatrices{};
};

struct AnimationManager
{
    std::size_t PlayAnimation(Animation* pAnimation)
    {
        m_Animations[nextID++].m_CurrentAnimation = pAnimation;
        return nextID - 1;
    }

    void ChangeAnimation(std::size_t id, Animation* pAnimation) {
        m_Animations[id].reset();
        m_Animations[id].m_CurrentAnimation = pAnimation;
    }

    void StopAnimation(std::size_t idanim) {
        //sacamos la animacion del vector
        if (idanim < m_Animations.size()) {
            m_Animations[idanim].reset();
        }
    }

    void interpolateAnimation(std::size_t id, Animation* pAnimation)
    {
        m_Animations[id].m_NextAnimation = pAnimation;
        m_Animations[id].interpolating = true;
    }

    bool isInterpolating(std::size_t id)
    {
        return m_Animations[id].interpolating;
    }

    void UpdateAnimation(float dt, std::size_t id)
    {
        auto& anim = m_Animations[id];
        if (anim.m_CurrentAnimation) {
            anim.m_CurrentTime = myFmod(anim.m_CurrentTime + anim.m_CurrentAnimation->getTicksPerSecond() * dt, anim.m_CurrentAnimation->getDuration());

            float transitionTime = anim.m_CurrentAnimation->getTicksPerSecond() * 0.2f;
            if (anim.interpolating && anim.interTime <= transitionTime) {
                anim.interTime += anim.m_CurrentAnimation->getTicksPerSecond() * dt;
                calculateBoneTransition(anim.m_CurrentAnimation->getRootNode(), glm::mat4(1.0f), anim.m_CurrentAnimation, anim.m_NextAnimation, anim.haltTime, anim.interTime, transitionTime, id);
                return;
            }
            else if (anim.interpolating) {
                if (anim.m_QueueAnimation) {
                    anim.m_CurrentAnimation = anim.m_NextAnimation;
                    anim.haltTime = 0.0f;
                    anim.m_NextAnimation = anim.m_QueueAnimation;
                    anim.m_QueueAnimation = nullptr;
                    anim.m_CurrentTime = 0.0f;
                    anim.interTime = 0.0;
                    return;
                }

                anim.interpolating = false;
                anim.m_CurrentAnimation = anim.m_NextAnimation;
                anim.m_CurrentTime = 0.0;
                anim.interTime = 0.0;
            }

            calculateBoneTransform(anim.m_CurrentAnimation->getRootNode(), glm::mat4(1.0f), anim.m_CurrentAnimation, anim.m_CurrentTime, id);
        }
    }

    void calculateBoneTransform(AssimpNodeData* node, glm::mat4 parentTransform, Animation* animation, float currentTime, std::size_t id)
    {
        std::string nodeName = node->name;
        glm::mat4 boneTransform = node->transformation;

        if (!node->bone)
            node->bone = animation->findBone(nodeName);

        auto* bone = node->bone;
        if (bone)
        {
            bone->update(currentTime);
            boneTransform = bone->getTransform();
        }

        glm::mat4 globalTransformation = parentTransform * boneTransform;

        auto boneProps = animation->getBoneProps();

        for (unsigned int i = 0; i < boneProps.size(); i++) {
            if (boneProps[i].name == nodeName) {
                glm::mat4 offset = boneProps[i].offset;
                m_Animations[id].m_FinalBoneMatrices[i] = globalTransformation * offset;
                break;
            }
        }

        for (std::size_t i = 0; i < node->childrenCount; i++)
            calculateBoneTransform(&node->children[i], globalTransformation, animation, currentTime, id);
    }

    bool isAnimationEnded(std::size_t id)
    {
        return m_Animations[id].m_CurrentTime >= m_Animations[id].m_CurrentAnimation->getDuration();
    }

    bool isAnimationsEmpty()
    {
        return m_Animations.empty();
    }

    std::vector<glm::mat4>& GetFinalBoneMatrices(std::size_t id)
    {
        return m_Animations[id].m_FinalBoneMatrices;
    }

    void calculateBoneTransition(const AssimpNodeData* curNode, glm::mat4 parentTransform, Animation* prevAnimation, Animation* nextAnimation, float haltTime, float currentTime, float transitionTime, std::size_t id)
    {
        std::string nodeName = curNode->name;
        glm::mat4 transform = curNode->transformation;

        Bone* prevBone = prevAnimation->findBone(nodeName);
        Bone* nextBone = nextAnimation->findBone(nodeName);

        if (prevBone && nextBone)
        {
            KeyPosition prevPos = prevBone->getPositions(haltTime);
            KeyRotation prevRot = prevBone->getRotations(haltTime);
            KeyScale prevScl = prevBone->getScalings(haltTime);

            KeyPosition nextPos = nextBone->getPositions(0.0f);
            KeyRotation nextRot = nextBone->getRotations(0.0f);
            KeyScale nextScl = nextBone->getScalings(0.0f);

            prevPos.timeStamp = 0.0f;
            prevRot.timeStamp = 0.0f;
            prevScl.timeStamp = 0.0f;

            nextPos.timeStamp = transitionTime;
            nextRot.timeStamp = transitionTime;
            nextScl.timeStamp = transitionTime;

            glm::mat4 p = Bone::interpolatePosition(currentTime, prevPos, nextPos);
            glm::mat4 r = Bone::interpolateRotation(currentTime, prevRot, nextRot);
            glm::mat4 s = Bone::interpolateScaling(currentTime, prevScl, nextScl);

            transform = p * r * s;
        }

        glm::mat4 globalTransformation = parentTransform * transform;

        auto boneProps = nextAnimation->getBoneProps();
        for (unsigned int i = 0; i < boneProps.size(); i++) {
            if (boneProps[i].name == nodeName) {
                glm::mat4 offset = boneProps[i].offset;
                m_Animations[id].m_FinalBoneMatrices[i] = globalTransformation * offset;
                break;
            }
        }

        for (std::size_t i = 0; i < curNode->childrenCount; i++)
            calculateBoneTransition(&curNode->children[i], globalTransformation, prevAnimation, nextAnimation, haltTime, currentTime, transitionTime, id);
    }

    static AnimationManager& getInstance() {
        static AnimationManager instance;
        return instance;
    }

    float myFmod(float a, float b) {
        float result = a - static_cast<float>(static_cast<int>(b) * static_cast<int>(a / b));
        if (result < 0) {
            result += b;
        }
        return result;
    }

    float getCurrentInSecs(std::size_t id)
    {
        return m_Animations[id].m_CurrentTime / m_Animations[id].m_CurrentAnimation->getTicksPerSecond();
    }

    glm::mat4 getBoneTransform(const std::string& name, Animation* animation)
    {
        auto* bone = animation->findBone(name);
        if (bone)
            return bone->getTransform();

        return glm::mat4(1.0f);
    }

    std::vector<Animation*> createAnimations(const std::string& animationPath, std::vector<BoneInfo>& modelBones)
    {
        std::vector<Animation*> animations;
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
        assert(scene && scene->mRootNode);

        for (std::size_t i = 0; i < scene->mNumAnimations; i++)
        {
            auto animation = std::make_unique<Animation>(scene, modelBones, i);
            auto rawPtr = animation.get();
            m_AnimationList.push_back(std::move(animation));
            animations.push_back(rawPtr);
        }

        return animations;
    }

    void resetAnims()
    {
        for (std::size_t i = 1; i < m_Animations.size(); i++)
            m_Animations[i].reset();

        nextID = 1;
    }

private:
    static inline std::size_t nextID = 0;
    std::array<Anim, 40> m_Animations{};
    std::vector<std::unique_ptr<Animation>> m_AnimationList{};
};

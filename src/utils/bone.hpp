#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

#include <assimp/scene.h>

#include <vector>

struct KeyPosition
{
    glm::vec3 position;
    float timeStamp;
};

struct KeyRotation
{
    glm::quat orientation;
    float timeStamp;
};

struct KeyScale
{
    glm::vec3 scale;
    float timeStamp;
};

struct Bone
{
private:
    glm::mat4 transform;
    std::vector<KeyPosition> positions;
    std::vector<KeyRotation> rotations;
    std::vector<KeyScale> scales;
    size_t numPositions;
    size_t numRotations;
    size_t numScalings;
    std::string name;
    unsigned int id;

public:
    Bone(const std::string& inName, int inId, const aiNodeAnim* channel) {
        name = inName;
        id = static_cast<unsigned int>(inId);
        transform = glm::mat4(1.0f);

        numPositions = channel->mNumPositionKeys;

        for (std::size_t positionIndex = 0; positionIndex < numPositions; ++positionIndex)
        {
            aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;
            float timeStamp = (float)channel->mPositionKeys[positionIndex].mTime;
            KeyPosition data = { glm::vec3(aiPosition.x, aiPosition.y, aiPosition.z), timeStamp };
            positions.push_back(data);
        }

        numRotations = channel->mNumRotationKeys;
        for (std::size_t rotationIndex = 0; rotationIndex < numRotations; ++rotationIndex)
        {
            aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
            float timeStamp = (float)channel->mRotationKeys[rotationIndex].mTime;
            KeyRotation data = { glm::quat(aiOrientation.w, aiOrientation.x, aiOrientation.y, aiOrientation.z), timeStamp };
            rotations.push_back(data);
        }

        numScalings = channel->mNumScalingKeys;
        for (std::size_t keyIndex = 0; keyIndex < numScalings; ++keyIndex)
        {
            aiVector3D scale = channel->mScalingKeys[keyIndex].mValue;
            float timeStamp = (float)channel->mScalingKeys[keyIndex].mTime;
            KeyScale data = { glm::vec3(scale.x, scale.y, scale.z), timeStamp };
            scales.push_back(data);
        }
    }

    KeyPosition getPositions(float animationTime) {
        size_t posIndex = (animationTime == 0.0f) ? 0 : getPositionIndex(animationTime) + 1;
        return positions[posIndex];
    }

    KeyRotation getRotations(float animationTime) {
        size_t rotIndex = (animationTime == 0.0f) ? 0 : getRotationIndex(animationTime) + 1;
        return rotations[rotIndex];
    }

    KeyScale getScalings(float animationTime) {
        size_t sclIndex = (animationTime == 0.0f) ? 0 : getScaleIndex(animationTime) + 1;
        return scales[sclIndex];
    }

    void update(float animationTime)
    {
        size_t posIndex = getPositionIndex(animationTime);
        glm::mat4 translation;
        if (numPositions == 1) {
            translation = glm::translate(glm::mat4(1.0f), positions[0].position);
        }
        else
            translation = interpolatePosition(animationTime, positions[posIndex], positions[posIndex + 1]);

        size_t rotIndex = getRotationIndex(animationTime);
        glm::mat4 rotation;
        if (numRotations == 1)
            rotation = glm::toMat4(glm::normalize(rotations[0].orientation));
        else
            rotation = interpolateRotation(animationTime, rotations[rotIndex], rotations[rotIndex + 1]);

        size_t sclIndex = getScaleIndex(animationTime);
        glm::mat4 scale;
        if (numScalings == 1)
            scale = glm::scale(glm::mat4(1.0f), scales[0].scale);
        else
            scale = interpolateScaling(animationTime, scales[sclIndex], scales[sclIndex + 1]);
        transform = translation * rotation * scale;
    }

    glm::mat4 getTransform() { return transform; }
    std::string getBoneName() const { return name; }
    unsigned int getId() const { return id; }

    size_t getPositionIndex(float animationTime)
    {
        for (size_t index = 0; index < numPositions - 1; ++index)
        {
            if (animationTime < positions[index + 1].timeStamp)
                return index;
        }
        return numPositions - 2;
    }

    size_t getRotationIndex(float animationTime)
    {
        for (size_t index = 0; index < numRotations - 1; ++index)
        {
            if (animationTime < rotations[index + 1].timeStamp)
                return index;
        }
        return numRotations - 2;
    }

    size_t getScaleIndex(float animationTime)
    {
        for (size_t index = 0; index < numScalings - 1; ++index)
        {
            if (animationTime < scales[index + 1].timeStamp)
                return index;
        }
        return numScalings - 2;
    }

    static float getScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
    {
        float scaleFactor = 0.0f;
        float midWayLength = animationTime - lastTimeStamp;
        float framesDiff = nextTimeStamp - lastTimeStamp;
        scaleFactor = midWayLength / framesDiff;
        return scaleFactor;
    }

    static glm::mat4 interpolatePosition(float animationTime, KeyPosition from, KeyPosition to)
    {
        float scaleFactor = getScaleFactor(from.timeStamp, to.timeStamp, animationTime);
        glm::vec3 finalPosition = glm::mix(from.position, to.position, scaleFactor);
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), finalPosition);
        return translation;
    }

    static glm::mat4 interpolateRotation(float animationTime, KeyRotation from, KeyRotation to)
    {
        float scaleFactor = getScaleFactor(from.timeStamp, to.timeStamp, animationTime);
        glm::quat finalRotation = glm::slerp(from.orientation, to.orientation, scaleFactor);
        finalRotation = glm::normalize(finalRotation);
        return glm::toMat4(finalRotation);
    }

    static glm::mat4 interpolateScaling(float animationTime, KeyScale from, KeyScale to)
    {
        float scaleFactor = getScaleFactor(from.timeStamp, to.timeStamp, animationTime);
        glm::vec3 finalScale = glm::mix(from.scale, to.scale, scaleFactor);
        return glm::scale(glm::mat4(1.0f), finalScale);
    }
};
#pragma once
#include "entity.hpp"
#include "../utils/color.hpp"

#include <vector>
#include <glm/glm.hpp>

namespace DarkMoon {
    struct Light : Entity {
        Color color = D_WHITE;
        bool enabled = { true };
        float ambientIntensity = { 1.0f };
        float diffuseIntensity = { 1.0f };

        Light(Color c)
            : color(c) {};
    };

    struct DirectionalLight : Light {
        glm::vec3 direction{};

        DirectionalLight(glm::vec3 dir, Color c)
            : Light(c), direction(dir) {};
    };

    struct PointLight : Light {
        glm::vec3 position{};
        float constant{ 1.0f };      // 1.0   // 0.9
        float linear{ 0.09f };       // 0.09  // 0.007
        float quadratic{ 0.032f };   // 0.032 // 0.00008

        PointLight(glm::vec3 pos, Color c)
            : Light(c), position(pos) {};

        // void draw(glm::mat4 transMatrix) override{
        //     position = glm::vec3(transMatrix[3][0], transMatrix[3][1], transMatrix[3][2]);
        // }

        void setIntensity(float intensity){
            constant  /= intensity; 
            linear    /= intensity; 
            quadratic /= intensity; 
        }
    };

    struct SpotLight : Light {
        glm::vec3 position{};
        glm::vec3 direction{};
        float cutOff{};
        float constant{ 1.0f };
        float linear{ 0.09f };
        float quadratic{ 0.032f }; 

        SpotLight(glm::vec3 pos, glm::vec3 dir, float cut, Color c)
            : Light(c), position(pos), direction(dir), cutOff(cut) {};

        void setIntensity(float intensity){
            constant  /= intensity; 
            linear    /= intensity; 
            quadratic /= intensity; 
        }
    };
}

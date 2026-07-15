#pragma once

#include <glm/glm.hpp>
#include <memory>

namespace DarkMoon {
    struct Entity {
        virtual ~Entity() {};

        virtual void draw(glm::mat4) {};
        virtual void unload() {};
        std::unique_ptr<Entity> clone() {
            return std::make_unique<Entity>(*this);
        };
    };

    //struct Light : Entity{
    //    void draw(glm::mat4) const override {};
    //};
}
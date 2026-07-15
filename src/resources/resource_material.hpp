#pragma once
#include "resource.hpp"
#include "resource_texture.hpp"

#include <vector>
#include <memory>
#include <glm/glm.hpp>

namespace DarkMoon {
    struct Material : public Resource {
    public:
        Texture* texture { nullptr };

        Material(std::size_t id) { this->m_id = id; };
        Material(std::size_t id, glm::vec3 ambientColor, glm::vec3 diffuseColor, glm::vec3 specularColor, float shininess) {
            this->m_id = id;
            this->m_ambientColor = ambientColor;
            this->m_diffuseColor = diffuseColor;
            this->m_specularColor = specularColor;
            this->m_shininess = shininess;
        };
        ~Material() { unload(); };

        bool load(const char* filePath) override {
            this->m_filePath = filePath;
            std::cout << "Load a material (ID:" << m_id << ") -> " << m_filePath << std::endl;
            m_isLoad = true;
            return isLoaded();
        };

        void unload() override {
            m_isLoad = false;
            std::cout << "Unload a material (ID:" << m_id << ")" << std::endl;
        };

        glm::vec3 getAmbientColor(){ return m_ambientColor; };
        glm::vec3 getDiffuseColor(){ return m_diffuseColor; };
        glm::vec3 getSpecularColor(){ return m_specularColor; };
        float getShininess(){ return m_shininess; };

    private:
        glm::vec3 m_ambientColor {};
        glm::vec3 m_diffuseColor {};
        glm::vec3 m_specularColor {};
        float m_shininess {};
    };
}
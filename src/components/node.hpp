#pragma once

#include <memory>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glm/gtc/quaternion.hpp>

#include "entity.hpp"
#include "entity_light.hpp"
#include "entity_model.hpp"
#include "entity_camera.hpp"

namespace DarkMoon {
    // Node
    struct Node {
    private:
        std::unique_ptr<Entity> m_entity;
        std::vector<std::unique_ptr<Node>> m_children;
        Node* m_parent;
        glm::vec3 m_translation;
        glm::quat m_rotation;
        glm::vec3 m_scale;
        glm::mat4 m_transformationMatrix;
        bool m_updateMatrix{ false };
        bool m_visible{ true };

    public:
        std::string name;

        Node();
        int addChild(std::unique_ptr<Node> child);
        int removeChild(Node* child);

        template<typename T> bool setEntity(std::unique_ptr<T> newEntity) {
            m_entity = std::move(newEntity);
            return true;
        }

        void setVisible(bool visible);
        void setVisibleOne(bool visible);
        bool isVisible() { return m_visible; };
        void clearChildren() { m_children.clear(); };
        void destroy();

        // Transform
        void traverse(glm::mat4 parentMatrix);
        void setTranslation(glm::vec3 newTranslation);
        void setRotation(glm::vec3 axis, float angle);
        void setScale(glm::vec3 newScale);
        void setTransformationMatrix(glm::mat4 newMatrix);
        void translate(glm::vec3 newTranslate);
        void rotate(glm::vec3 axis, float angle);
        void scale(glm::vec3 newScale);

        // Getters
        Entity* getEntity();
        template<typename T> T* getEntity() { return dynamic_cast<T*>(m_entity.get()); }
        Node* getParent();
        glm::vec3 getTranslation();
        glm::quat getRotation();
        glm::vec3 getScale();
        glm::mat4 getTransformationMatrix();
        std::vector<Node*> getChildren();
        bool getVisible() { return m_visible; };

        // Auxiliaries
        void printTransformationMatrix();
        void drawTree(std::string prefix = "", bool isLeft = false);
    };
}

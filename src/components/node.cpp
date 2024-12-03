#include "node.hpp"
#include <algorithm>
#include <iostream>
#include <iomanip>

namespace DarkMoon {
    Node::Node()
        : m_entity(nullptr),
        m_parent(nullptr),
        m_translation(0.0f),
        m_rotation(1.0f, 0.0f, 0.0f, 0.0f),
        m_scale(1.0f),
        m_transformationMatrix(1.0f) {}

    int Node::addChild(std::unique_ptr<Node> child) {
        m_children.push_back(std::move(child));
        m_children.back()->m_parent = this;
        return static_cast<int>(m_children.size() - 1);
    }

    int Node::removeChild(Node* child) {
        auto it = std::find_if(m_children.begin(), m_children.end(),
            [child](const std::unique_ptr<Node>& node) {
            return node.get() == child;
        });

        if (it != m_children.end()) {
            (*it)->m_parent = nullptr;
            m_children.erase(it);
            return 0; // Éxito
        }
        return -1;
    }

    void Node::setVisible(bool visible) {
        this->m_visible = visible;
        for (auto& child : m_children)
            child->setVisible(visible);
    }

    void Node::setVisibleOne(bool visible) {
        this->m_visible = visible;

        if (m_parent)
            m_parent->setVisibleOne(visible);
    }

    void Node::destroy() {
        if (m_parent)
            m_parent->removeChild(this);
    }

    void Node::traverse(glm::mat4 parentMatrix) {

        // Check changes
        if (m_updateMatrix) {

            m_transformationMatrix = parentMatrix
                * glm::translate(glm::mat4(1.0f), m_translation)
                * glm::mat4_cast(m_rotation)
                * glm::scale(glm::mat4(1.0f), m_scale);
        }   

        // Draw Entity
        if (m_entity && m_visible && m_parent->m_visible)
            m_entity->draw(m_transformationMatrix);

        for (auto& child : m_children) {
            if (m_updateMatrix)
                child->m_updateMatrix = true;
            child->traverse(m_transformationMatrix);
        }

        m_updateMatrix = false;
    }

    void Node::setTranslation(glm::vec3 newTranslation) {
        m_translation = newTranslation;
        m_updateMatrix = true;
    }

    void Node::setRotation(glm::vec3 axis, float angle) {
        m_rotation = glm::angleAxis(glm::radians(angle), glm::normalize(axis));
        m_updateMatrix = true;
    }

    void Node::setScale(glm::vec3 newScale) {
        m_scale = newScale;
        m_updateMatrix = true;
    }

    void Node::setTransformationMatrix(glm::mat4 newMatrix) {

        glm::vec3 newTranslate = newMatrix[3];
        setTranslation(newTranslate);

        glm::vec3 scale;
        scale.x = glm::length(glm::vec3(newMatrix[0]));
        scale.y = glm::length(glm::vec3(newMatrix[1]));
        scale.z = glm::length(glm::vec3(newMatrix[2]));
        m_scale = scale;

        // glm::mat4 rotation = glm::mat4();

        // rotation[0] = newMatrix[0] / scale.x;
        // rotation[1] = newMatrix[1] / scale.y;
        // rotation[2] = newMatrix[2] / scale.z;

        // m_rotation = rotation;

        m_transformationMatrix = newMatrix;
    }

    void Node::translate(glm::vec3 newTranslate) { setTranslation(m_translation + newTranslate); }
    void Node::scale(glm::vec3 newScale) { setScale(m_scale * newScale); }

    void Node::rotate(glm::vec3 axis, float angle) {
        auto additionalRotation = glm::angleAxis(glm::radians(angle), glm::normalize(axis));
        m_rotation = additionalRotation * m_rotation;
        m_updateMatrix = true;
    }

    // GETTERS

    Entity* Node::getEntity() { return m_entity.get(); }
    Node* Node::getParent() { return m_parent; }
    glm::vec3 Node::getTranslation() { return m_translation; }
    glm::quat Node::getRotation() { return m_rotation; }
    glm::vec3 Node::getScale() { return m_scale; }
    glm::mat4 Node::getTransformationMatrix() { return m_transformationMatrix; }

    std::vector<Node*> Node::getChildren() {
        std::vector<Node*> childrenPointers;
        for (auto& child : m_children)
            childrenPointers.push_back(child.get());

        return childrenPointers;
    }

    // AUXILIARS

    void Node::printTransformationMatrix() {
        std::cout << name << " --> Transformation Matrix:" << std::endl;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                std::cout << std::setw(2) << m_transformationMatrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    void Node::drawTree(std::string prefix, bool isLeft) {
        std::cout << prefix;
        std::cout << (isLeft ? "├──" : "└──");
        std::cout << "[" << (m_visible ? "X" : " ") << "] ";
        std::cout << name << std::endl;

        for (size_t i = 0; i < m_children.size(); ++i)
            m_children[i]->drawTree(prefix + (isLeft ? "│   " : "    "), i != m_children.size() - 1);
    }
}
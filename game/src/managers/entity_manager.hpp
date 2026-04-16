#pragma once

#include <vector>
#include <memory>

struct RenderComponent {
    Vector2D position;
    std::unique_ptr<Text> text;
    std::string font { "./assets/roboto.ttf" };
    Window* window;

    RenderComponent(Vector2D pos, const char* texto, Window* w) 
        : position(pos), window(w) 
    {
        text = std::make_unique<Text>(position, texto, font.c_str(), window);
    };
};

struct EntityManager {
private:
    struct Entity {
        int id;
        RenderComponent* renderComponent;
        
        Entity(int id) : id(id), renderComponent(nullptr) {}
        
        void addRenderComponent(Vector2D pos, const char* texto, Window* w) {
            renderComponent = new RenderComponent(pos, texto, w);
        }
    };

    std::vector<std::unique_ptr<Entity>> entities;
    int nextID = 0;

public:

    Entity* createEntity() {
        entities.push_back(std::make_unique<Entity>(nextID++));
        return entities.back().get();
    }

    const std::vector<std::unique_ptr<Entity>>& getEntities() const {
        return entities;
    }
};

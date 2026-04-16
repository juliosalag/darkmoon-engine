#pragma once

#include <iostream>

#include "../managers/entity_manager.hpp"

struct RenderSystem {
public:
    void render(const EntityManager& manager) {
        for (const auto& e : manager.getEntities()) {
            if (e->renderComponent) {
                e->renderComponent->text->Draw();
            }
        }
    }
};
#include "game.hpp"
#include "./managers/entity_manager.hpp"
#include "./systems/render_system.hpp"

void Game::run(){
    auto w1 = Window(600, 500, "Prueba");
    w1.SetPosition(30, 60);

    auto w2 = Window(300, 450, "Debug", &w1);
    w2.SetPosition(660, 60);

    EntityManager manager;
    RenderSystem renderSystem;

    // Title
    auto* e1 = manager.createEntity();
    e1->addRenderComponent({0, 0}, "Debug Menu", &w2);
    auto& r1 = e1->renderComponent;
    r1->text->SetPosition({(w2.GetWidth() / 2) - static_cast<int>(r1->text->MeasureText() / 2), 10});

    // Subtitle
    auto* e2 = manager.createEntity();
    e2->addRenderComponent({0, 0}, "Carlitas Ñ", &w2);
    auto& r2 = e2->renderComponent;
    r2->text->SetPosition({(w2.GetWidth() / 2) - static_cast<int>(r2->text->MeasureText() / 2), 50});
    r2->text->SetColor(GRAY);
    r2->text->SetScale(0.5f);

    w1.Focus();

    while(!w1.ShouldClose() && !w2.ShouldClose()){

        // Draw game window
        w1.BeginDrawing(GRAY); w1.EndDrawing();
        
        // Draw debug window
        w2.BeginDrawing(BLACK);

        // Update text
        r1->text->SetPosition({(w2.GetWidth() / 2) - static_cast<int>(r1->text->MeasureText() / 2), 10});
        r2->text->SetPosition({(w2.GetWidth() / 2) - static_cast<int>(r2->text->MeasureText() / 2), 50});

        // Render System
        renderSystem.render(manager);
        
        w2.EndDrawing();

    }
}
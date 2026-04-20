#include "game.hpp"

#include  <cstdio>

// #include "./utils/memviewer.hpp"

void Game::run(){

    std::cout << "\n-----------------\n";
    std::cout << " Create Entities" << "\n";
    std::cout << "-----------------\n";

    auto& player = m_em.newEntity();
    m_em.addComponent<RenderComponent>(player, RenderComponent{});

    auto& enemy1 = m_em.newEntity();
    m_em.addComponent<RenderComponent>(enemy1, RenderComponent{});
    
    auto& enemy2 = m_em.newEntity();
    m_em.addComponent<RenderComponent>(enemy2, RenderComponent{});

    //std::cout << "Entidades vivas: " << m_em.aliveCount() << "\n";
    //std::cout << "IDs: " << player.getID() << ", "    << enemy1.getID() << ", "    << enemy2.getID() << "\n";

    using SYSCMPs = MP::TypeList<RenderComponent>;
    using SYSTAGs = MP::TypeList<>;

    m_em.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, RenderComponent&){
        std::cout  << "ID: "  << e.getID() << "\n";
    });

    /*
    auto w1 = Window(600, 500, "Game");
    w1.SetPosition(30, 60);

    auto w2 = Window(300, 450, "Debug", &w1);
    w2.SetPosition(660, 60);

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
        //r1->text->SetPosition({(w2.GetWidth() / 2) - static_cast<int>(r1->text->MeasureText() / 2), 10});
        //r2->text->SetPosition({(w2.GetWidth() / 2) - static_cast<int>(r2->text->MeasureText() / 2), 50});
        
        // Render System
        //renderSystem.render(manager);
        
        w2.EndDrawing();
        
    }
    */
}
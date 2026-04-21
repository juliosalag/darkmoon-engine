#include "game.hpp"

#include <cstdio>

// #include "./utils/memviewer.hpp"

void Game::run(){

    auto w1 = Window(600, 500, "Game");
    w1.SetPosition(30, 60);

    auto w2 = Window(300, 450, "Debug", &w1);
    w2.SetPosition(660, 60);

    w2.Focus();

    // Title
    auto& title = m_em.newEntity();
    auto& rt = m_em.addComponent<RenderComponent>(title, RenderComponent{{0, 0}, "Debug Menu", &w2});
    rt.text->SetPosition({(w2.GetWidth() / 2) - static_cast<int>(rt.text->MeasureText() / 2), 10});

    // Subtitle
    auto& subtitle = m_em.newEntity();
    auto& rt2 = m_em.addComponent<RenderComponent>(subtitle, RenderComponent{{0, 0}, "Carlitas Ñ", &w2});
    rt2.text->SetPosition({(w2.GetWidth() / 2) - static_cast<int>(rt2.text->MeasureText() / 2), 50});
    rt2.text->SetColor(GRAY);
    rt2.text->SetScale(0.5f);
    
    w1.Focus();
    
    while(!w1.ShouldClose() && !w2.ShouldClose()){
        
        // Draw game window
        w1.BeginDrawing(GRAY); w1.EndDrawing();
        
        // Draw debug window
        w2.BeginDrawing(BLACK);
        
        // Render System
        m_renderSystem.update(m_em, w2);
        
        w2.EndDrawing();
        
    }
}
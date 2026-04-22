#include "game.hpp"

#include <cstdio>

// #include "./utils/memviewer.hpp"

void Game::CreateStars(int numStars, Window& w){
    for(int i = 0; i < numStars; i++){
        auto& star = m_em.newEntity();
        int random = std::rand() % 3 + 1;
        m_em.addComponent<PhysicsComponent>(star, PhysicsComponent{ Vector2D{ static_cast<int>(std::rand() % w.GetWidth()), static_cast<int>(std::rand() % w.GetHeight()) }, Vector2D{ random, 0 }});
        m_em.addComponent<RenderComponent>(star, RenderComponent{WHITE, random, w});
    }
}

void Game::run(){

    auto w1 = Window(600, 500, "Game");
    w1.SetPosition(30, 60);

    auto w2 = Window(300, 450, "Debug", &w1);
    w2.SetPosition(660, 60);

    w1.Focus();
    CreateStars(150, w1);
    
    w2.Focus();
    DebugSystem db = DebugSystem(w2);
    
    w1.Focus();
    
    while(!w1.ShouldClose() && !w2.ShouldClose()){
        
        // Draw game window
        w1.BeginDrawing(BLACK); 

        m_physicsSystem.update(m_em, w1);
        m_renderSystem.update(m_em, w1);
        
        w1.EndDrawing();
        
        // Draw debug window
        w2.BeginDrawing(BLACK);
        
        db.update(m_em, w2);
        
        w2.EndDrawing();
        
    }
}
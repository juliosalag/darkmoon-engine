#include "game.hpp"

#include <cstdio>

// #include "./utils/memviewer.hpp"

void Game::CreateStars(int numStars, Window& w){
    for(int i = 0; i < numStars; i++){
        CreateStar(w);
    }
}

void Game::CreateStar(Window& w){
    auto& star = m_em.newEntity();
    int random = std::rand() % 3 + 1;
    m_em.addComponent<PhysicsComponent>(star, PhysicsComponent{ Vector2D{ static_cast<int>(std::rand() % w.GetWidth()), static_cast<int>(std::rand() % w.GetHeight()) }, Vector2D{ random, 0 }});
    m_em.addComponent<RenderComponent>(star, RenderComponent{WHITE, random, w});
}

void Game::DeleteRandomStar(Window&){
    if(m_em.aliveCount() > 0){
        std::size_t index = std::rand() % m_em.aliveCount();
        m_em.destroyEntityByID(index);
    }
}

struct Button {
    std::unique_ptr<Rectangle> rectangle {};
    std::unique_ptr<Text> text {};
    Window* w {};

    Button(Vector2D vertexA, Vector2D vertexB, Vector2D vertexC, Vector2D vertexD, Color color, const std::string& textButton, const char* fontPath, Color textColor, Window* window){
        rectangle = std::make_unique<Rectangle>(vertexA, vertexB, vertexC, vertexD, color, window);
        text = std::make_unique<Text>(Vector2D{}, textButton, fontPath, window);

        w = window;

        text->SetColor(textColor);
    }

    void Draw(){
        rectangle->Draw();
        CenterText();
        text->Draw();
    }

    bool isHover(){
        return (w->GetCursorPositionX() >= rectangle->GetVertexA().x) && (w->GetCursorPositionX() <= rectangle->GetVertexB().x) && (w->GetCursorPositionY() >= rectangle->GetVertexA().y) && (w->GetCursorPositionY() <= rectangle->GetVertexC().y);
    }

    bool isClicked(){
        return isHover() && w->IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    }

private:
    void CenterText() {
        float textW = text->MeasureText();
        float textH = text->GetTextHeight();

        float x = static_cast<float>(rectangle->GetVertexA().x);
        float y = static_cast<float>(rectangle->GetVertexA().y);
        float w = static_cast<float>(rectangle->GetVertexB().x - rectangle->GetVertexA().x);
        float h = static_cast<float>(rectangle->GetVertexC().y - rectangle->GetVertexA().y);

        text->SetPosition({
            static_cast<int>(x + (w - textW) / 2.f),
            static_cast<int>(y + (h - textH) / 2.f)
        });
    }
};

void Game::run(){

    auto w1 = Window(600, 500, "Game");
    w1.SetPosition(30, 60);

    auto w2 = Window(300, 450, "Debug", &w1);
    w2.SetPosition(660, 60);

    w1.Current();
    CreateStars(150, w1);
    
    w2.Current();
    DebugSystem db = DebugSystem(w2);

    // Line({0, 75}, {w.GetWidth(), 75}, GRAY, 1, &w).Draw();

    //Button but1 = Button({10, 75}, {200, 75}, {10, 110}, {200, 110}, GRAY, "Add Star", "./assets/roboto.ttf", WHITE, &w2);
    //Button but2 = Button({300, 75}, {500, 75}, {300, 110}, {500, 110}, WHITE, "Delete Star", "./assets/roboto.ttf", BLACK, &w2);
    
    w1.Focus();
    
    while(!w1.ShouldClose() && !w2.ShouldClose()){

        w1.Current();
        if(w1.IsKeyPressed(KEY_UP))
            CreateStar(w1);
        if(w1.IsKeyPressed(KEY_DOWN))
            DeleteRandomStar(w1);
        
        // Draw game window
        w1.BeginDrawing(BLACK); 

        m_physicsSystem.update(m_em, w1);
        m_renderSystem.update(m_em, w1);
        
        w1.EndDrawing();
        
        // Draw debug window
        w2.BeginDrawing(BLACK);
        
        db.update(m_em, w2);
        //but1.Draw();
        //but2.Draw();
        
        w2.EndDrawing();
        
    }
}
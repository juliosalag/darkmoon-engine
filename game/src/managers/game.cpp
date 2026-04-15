#include "game.hpp"

#include <memory>

struct Debug {
    Debug(Window* w) : window(w){ 

        // Title
        text.push_back(std::make_unique<Text>(Vector2D{0 ,0 }, "Debug Menu", "./assets/roboto.ttf", window));

        // Subtitle
        auto subtitle = std::make_unique<Text>(Vector2D{ 0, 0 }, "Carlitas Ñ", "./assets/roboto.ttf", window);
        subtitle->SetColor(GRAY);
        subtitle->SetScale(0.5f);
        text.push_back(std::move(subtitle));

        //text.push_back(subtitle);
        
    };

    void update() {
        // Title
        text.at(0)->SetPosition({(window->GetWidth() / 2) - static_cast<int>(text.at(0)->MeasureText() / 2), 10});

        // Subtitle
        text.at(1)->SetPosition({(window->GetWidth() / 2) - static_cast<int>(text.at(1)->MeasureText() / 2), 50});

        // Draw text
        for (auto& i: text){ i->Draw(); }
    };

    Window* window;
    std::vector<std::unique_ptr<Text>> text;
};

void Game::run(){
    auto w1 = Window(600, 500, "Prueba");
    w1.SetPosition(30, 60);

    auto w2 = Window(300, 450, "Debug", &w1);
    w2.SetPosition(660, 60);

    // Debug System
    Debug debug(&w2);

    w1.Focus();

    while(!w1.ShouldClose() && !w2.ShouldClose()){

        // Draw game window
        w1.BeginDrawing(GRAY); w1.EndDrawing();
        
        // Draw debug window
        w2.BeginDrawing(BLACK);

        debug.update();
        
        w2.EndDrawing();

    }
}
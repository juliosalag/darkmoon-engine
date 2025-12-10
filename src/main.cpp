#include "darkmoon.hpp"
#include "utils/keys.hpp"

int main() {

    DarkMoonEngine dm;

    auto w1 = Window(600, 500, "Prueba");
    w1.SetPosition(30, 60);

    auto w2 = Window(300, 450, "Debug", &w1);
    w2.SetPosition(660, 60);
    
    w1.Focus();
    
    // Initialize in W1
    
    auto pixel0 = Pixel({10, 10}, RED, 2, &w1);
    auto pixel = Pixel({330, 225}, RED, 4, &w1);
    auto line = Line({520, 220}, {560, 220}, GREEN, 1, &w1);
    auto triangle_lines = TriangleLines({400, 0}, {120, 30}, {30, 220}, RED, 1, &w1);

    // Fantasmico
    auto texture = Texture({10, 10}, {4, 4}, 0, "./assets/fantasmico.png", &w1);

    //auto texture = Texture({0, 0}, {1, 1}, 0, "./assets/defaultTexture.png", &w1, shader);

    w2.Focus();

    auto pixel2 = Pixel({130, 225}, GREEN, 2, &w2);

    w1.Focus();
    
    while(!w1.ShouldClose() && !w2.ShouldClose()){

        w1.Focus();

        if(w1.IsKeyDown(KEY_W))
            texture.SetPosition({texture.GetPosition().x, texture.GetPosition().y - 2});
        if(w1.IsKeyDown(KEY_S))
            texture.SetPosition({texture.GetPosition().x, texture.GetPosition().y + 2});
        if(w1.IsKeyDown(KEY_A))
            texture.SetPosition({texture.GetPosition().x - 2, texture.GetPosition().y});
        if(w1.IsKeyDown(KEY_D))
            texture.SetPosition({texture.GetPosition().x + 2, texture.GetPosition().y});
        
        w1.BeginDrawing(GRAY);

        texture.Draw();
        pixel0.Draw();
        pixel.Draw();
        line.Draw();
        triangle_lines.Draw();

        w1.EndDrawing();

        // ------------- //

        w2.BeginDrawing(BLACK);

        pixel2.Draw();

        w2.EndDrawing();
    }

    return 0;
}
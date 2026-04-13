#include "darkmoon.hpp"

int main() {

    DarkMoonEngine dm;

    auto w1 = Window(600, 500, "Prueba");
    w1.SetPosition(30, 60);

    auto w2 = Window(300, 450, "Debug", &w1);
    w2.SetPosition(660, 60);
    
    w1.Focus();

    // Load items in w1

    w2.Focus();

    Text label({ 0, 0 }, "Debug Menu", "./assets/roboto.ttf", &w2);
    Text label2({ 20, 100 }, "Carlitas Ñ", "./assets/roboto.ttf", &w2);
    label.SetColor(RED);

    std::cout << "Tamano de Label: " << label.MeasureText() << "\n";

    label2.SetScale(0.5f);

    while(!w1.ShouldClose() && !w2.ShouldClose()){

        w1.BeginDrawing(GRAY);
        w1.EndDrawing();
        
        w2.BeginDrawing(BLACK);

            label.Draw();
            label2.Draw();

        w2.EndDrawing();

    }

    return 0;
}
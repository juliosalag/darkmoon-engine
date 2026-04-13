#include "darkmoon.hpp"

int main() {

    DarkMoonEngine dm;

    auto w1 = Window(600, 500, "Prueba");
    w1.SetPosition(30, 60);

    Text label({ 20, 50 }, "./assets/roboto.ttf", 32.f, &w1);
    label.SetColor({ 255, 255, 255, 255 });

    while(!w1.ShouldClose()){

        w1.BeginDrawing(GRAY);

        label.Draw("Hola mundo");

        w1.EndDrawing();

    }

    return 0;
}
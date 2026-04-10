#include "darkmoon.hpp"

int main() {

    DarkMoonEngine dm;

    auto w1 = Window(600, 500, "Prueba");
    w1.SetPosition(30, 60);

    while(!w1.ShouldClose()){

        w1.BeginDrawing(GRAY);

        w1.EndDrawing();

    }

    return 0;
}
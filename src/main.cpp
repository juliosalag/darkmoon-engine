#include "darkmoon.hpp"

int main() {
    DarkMoon::DarkMoonEngine engine;

    if (engine.InitWindow(800, 600, "DarkMoon Engine")) {
        while (!engine.WindowShouldClose()) {
            
            // Draw
            engine.Draw(D_WHITE);
        }

        engine.CloseWindow();
    }

    return 0;
}
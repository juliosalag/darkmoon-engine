#include <darkmoon/darkmoon.hpp>

int main() {
    DarkMoonEngine dm{};

    Window win = Window(800, 600, "Window Title");

    while(!win.ShouldClose()){
        win.BeginDrawing(BLACK);
        win.EndDrawing();
    }
}
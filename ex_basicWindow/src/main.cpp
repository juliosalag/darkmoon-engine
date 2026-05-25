#include <darkmoon/darkmoon.hpp>

int main(){
    DarkMoonEngine dm{};

    Window win = Window(800, 600, "My First Window");

    while(!win.ShouldClose()){
        win.BeginDrawing(BLACK);
        win.EndDrawing();
    }
}
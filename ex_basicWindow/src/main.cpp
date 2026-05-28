#include <darkmoon/darkmoon.hpp>

int main(){
    DarkMoonEngine dm{};

    Window win = Window(800, 600, "My First Window");
    win.SetTargetFPS(120); // By default is 60
    //win.SetIcon("./assets/fantasmico.png");

    while(!win.ShouldClose()){
        win.BeginDrawing(BLACK);
        win.EndDrawing();
    }
}
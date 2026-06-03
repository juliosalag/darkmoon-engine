#include <darkmoon/darkmoon.hpp>

int main(){
    DarkMoonEngine dm{};

    Window win = Window(800, 600, "My First Window");
    win.SetTargetFPS(120); // By default is 60
    //win.SetIcon("./assets/fantasmico.png");

    std::cout << dm.GetMonitors().size() << "\n";

    while(!win.ShouldClose()){

        if(win.IsKeyPressed(KEY_1))
            win.SetFullscreen(dm.GetMonitors().at(1));
        if(win.IsKeyPressed(KEY_2))
            win.SetBorderless();
        if(win.IsKeyPressed(KEY_3))
            win.SetWindowed();

        win.BeginDrawing(BLACK);
        win.EndDrawing();
    }
}
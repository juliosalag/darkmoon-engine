#include <darkmoon/darkmoon.hpp>

//#include <chrono>

// TODO: int to float

int main(){
    DarkMoonEngine dm{};

    Window win = Window(800, 600, "My First Window");

    win.SetTargetFPS(120);
    win.SetPosition({10, 50});

    Pixel pix = Pixel({static_cast<float>(win.GetWidth() / 2), static_cast<float>(win.GetHeight() / 2)}, WHITE, 5, &win);

    constexpr static float speed = 200.0f;

    while(!win.ShouldClose()){
        //auto beg = std::chrono::high_resolution_clock::now();

        if(win.IsKeyDown(KEY_UP))
            pix.SetPosition({pix.GetPosition().x, pix.GetPosition().y - speed * win.GetDeltaTime()});
        if(win.IsKeyDown(KEY_DOWN))
            pix.SetPosition({pix.GetPosition().x, pix.GetPosition().y + speed * win.GetDeltaTime()});
        if(win.IsKeyDown(KEY_LEFT))
            pix.SetPosition({pix.GetPosition().x - speed * win.GetDeltaTime(), pix.GetPosition().y});
        if(win.IsKeyDown(KEY_RIGHT))
            pix.SetPosition({pix.GetPosition().x + speed * win.GetDeltaTime(), pix.GetPosition().y});

        if(win.IsKeyPressed(KEY_1))
            win.SetTargetFPS(win.GetTargetFPS() - 10);
        if(win.IsKeyPressed(KEY_2))
            win.SetTargetFPS(win.GetTargetFPS() + 10);
        
        win.BeginDrawing(BLACK);

        Text({10, 10}, std::to_string(win.GetFPS()) + " FPS", "./assets/roboto.ttf", &win).Draw();
        Text({10, 50}, std::to_string(win.GetDeltaTime() * 1000).substr(0, 5) + " ms", "./assets/roboto.ttf", &win).Draw();

        pix.Draw();

        win.EndDrawing();

        //auto end = std::chrono::high_resolution_clock::now();
        //std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - beg).count() << "\n";

    }
}
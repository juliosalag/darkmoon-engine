#include <darkmoon/darkmoon.hpp>

int main() {
    DarkMoonEngine dm{};

    Window win = Window(800, 600, "My First Cam");
    win.SetTargetFPS(60);

    float playerX = win.GetWidth() / 2.f;
    float playerY = win.GetHeight() / 2.f;
    Vector2Df positionPlayer {playerX, playerY};
    
    Camera2D cam;
    cam.zoom = 1.5f;
    cam.offset = {win.GetWidth() / 2.f, win.GetHeight() / 2.f};
    cam.target = {playerX, playerY};

    while(!win.ShouldClose()){

        // Camera movement
        if (win.IsKeyDown(KEY_RIGHT)) cam.target.x += 200.f * win.GetDeltaTime();
        if (win.IsKeyDown(KEY_LEFT))  cam.target.x -= 200.f * win.GetDeltaTime();
        if (win.IsKeyDown(KEY_DOWN))  cam.target.y += 200.f * win.GetDeltaTime();
        if (win.IsKeyDown(KEY_UP))    cam.target.y -= 200.f * win.GetDeltaTime();

        // Player movement
        if (win.IsKeyDown(KEY_D)) positionPlayer.x += 200.f * win.GetDeltaTime();
        if (win.IsKeyDown(KEY_A)) positionPlayer.x -= 200.f * win.GetDeltaTime();
        if (win.IsKeyDown(KEY_S)) positionPlayer.y += 200.f * win.GetDeltaTime();
        if (win.IsKeyDown(KEY_W)) positionPlayer.y -= 200.f * win.GetDeltaTime();

        // Zoom
        if (win.IsKeyDown(KEY_O)) cam.zoom -= 0.1f;
        if (win.IsKeyDown(KEY_P)) cam.zoom += 0.1f;

        win.BeginDrawing(BLACK);

        // without camera - screen coordinates (HUD)
        Rectangle({10, 10}, 160, 50, RED, &win).Draw();
        Text({20, 20}, std::to_string(cam.ScreenToWorld(win.GetCursorPosition()).x + cam.ScreenToWorld(win.GetCursorPosition()).y), "./assets/roboto.ttf", &win).Draw();
        
        // with camera - world coordinates
        Rectangle(positionPlayer, 20, 20, GREEN, &win).Draw(cam); // player
        Rectangle({300, 100}, 100, 50, WHITE, &win).Draw(cam);    // wall
        Rectangle({350, 350}, 20, 2000, WHITE, &win).Draw(cam);   // wall

        win.EndDrawing();
    }
}
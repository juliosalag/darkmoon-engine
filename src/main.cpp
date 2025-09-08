#include <darkmoon.hpp>

#include <glm/glm.hpp>

int main() {
    DarkMoonEngine dm;

    Window w = Window(400, 300, "Game");
    w.SetPosition(10, 10);

    auto shader = dm.CreateShader("assets/shaders/basicshader.vs", "assets/shaders/basicshader.fs");

    while(!w.ShouldClose()){

        // Logic //

        // Render //

        w.BeginDrawing();
        dm.ClearBackground(GRAY); // Change to window

        w.EndDrawing();
    }

    return 0;
}
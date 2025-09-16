#include "managers/window.hpp"
#include <darkmoon.hpp>

int main() {
    DarkMoonEngine dm;

    auto w = dm.CreateInitWindow(600, 450, "Game");
    w.SetPosition(30, 60);

    // Create shaders here: after create w and before create w2

    auto w2 = dm.CreateSharedWindow(300, 450, "Debug");
    w2.SetPosition(660, 60);

    w.Focus();

    while(!w.ShouldClose() && !w2.ShouldClose()){

        // Logic //

        // Render //

        // Window 1 //

        dm.BeginDrawing(w, GRAY);

        dm.DrawLine({300, 225}, {600, 450}, RED);
        dm.DrawLine({0, 0}, {300, 225}, BLACK, 4);
        dm.DrawPixel({310, 215}, WHITE, 5);
        dm.DrawPixel({315, 225}, RED, 2);
        
        dm.EndDrawing(w);

        // Window 2 //
        
        dm.BeginDrawing(w2);

        dm.DrawLine({150, 225}, {300, 225}, GREEN);
        dm.DrawLine({150, 225}, {150, 350}, BLUE);
        dm.DrawLine({150, 0}, {150, 225}, WHITE);

        dm.EndDrawing(w2);

    }

    return 0;
}
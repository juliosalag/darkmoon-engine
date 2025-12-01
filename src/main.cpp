#include "managers/window.hpp"
#include "resources/resource_shader.hpp"
#include "utils/color.hpp"
#include <darkmoon.hpp>
#include <iostream>

#include <chrono>


int main() {
    DarkMoonEngine dm;

    auto w = dm.CreateInitWindow(600, 450, "Game");
    w.SetPosition(30, 60);
    
    // Create shaders here: after create w and before create w2
    
    auto w2 = dm.CreateSharedWindow(300, 450, "Debug");
    w2.SetPosition(660, 60);
    
    dm.FocusWindow(w);
    
    while(!w.ShouldClose() && !w2.ShouldClose()){
        
        // ----- //
        // Logic //
        // ----- //

        
        
        // ------ //
        // Render //
        // ------ //
        
        // Window 1 //

        dm.BeginDrawing(w, GRAY);

        // Pixel //

        static auto pixel = dm.CreatePixel({330, 225}, GREEN, 1);
        pixel.Draw();

        dm.DrawPixel({315, 225}, RED, 2);

        // Line //

        static auto line = dm.CreateLine({520, 220}, {560, 220}, GREEN, 1);
        line.Draw();

        dm.DrawLine({320, 220}, {360, 220}, WHITE, 2);

        dm.EndDrawing(w);

        // Window 2 //
        
        dm.BeginDrawing(w2);
        dm.EndDrawing(w2);
    }

    return 0;
}
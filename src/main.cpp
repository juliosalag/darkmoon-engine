
#include "managers/window.hpp"
#include "resources/resource_shader.hpp"
#include "utils/color.hpp"
#include "utils/keys.hpp"
#include <darkmoon.hpp>
#include <iostream>

#include <chrono>


int main() {
    DarkMoonEngine dm;

    auto w = Window(600, 450, "Game");
    w.SetPosition(30, 60);

    dm.LoadBasicShaders();
    
    // Create shaders here: after create w and before create w2

    auto w2 = Window(300, 450, "Debug", w.GetWindow());
    w2.SetPosition(660, 60);

    //w.Focus();
    
    //auto w2 = dm.CreateSharedWindow(300, 450, "Debug");
    //w2.SetPosition(660, 60);
    
    dm.FocusWindow(w);

    // Initialize
    static auto pixel = dm.CreatePixel({330, 225}, GREEN, 1);
    static auto line = dm.CreateLine({520, 220}, {560, 220}, GREEN, 1);

    
    while(!w.ShouldClose()&& !w2.ShouldClose()){
        
        // ----- //
        // Logic //
        // ----- //

        w.PollEvents();
        
        //std::cout << pixel.GetPosition().x << " - " << pixel.GetPosition().y << "\n";
        
        if(w.IsKeyPressed(KEY_W)){
            std::cout << "W\n";
            //pixel.SetPosition({pixel.GetPosition().x, pixel.GetPosition().y - 1}, w.GetSize());
        }

        
        if(w.IsKeyPressed(KEY_S)){
            std::cout << "S\n";
            //pixel.SetPosition({pixel.GetPosition().x, pixel.GetPosition().y + 1}, w.GetSize());
        } 

        if(w.IsKeyPressed(KEY_A)){
            std::cout << "A\n";
            //pixel.SetPosition({pixel.GetPosition().x - 1, pixel.GetPosition().y}, w.GetSize());
        } 

        if(w.IsKeyPressed(KEY_D)){
            std::cout << "D\n";
            //pixel.SetPosition({pixel.GetPosition().x + 1, pixel.GetPosition().y}, w.GetSize());
        } 


        // ------ //
        // Render //
        // ------ //
        
        // Window 1 //

        dm.BeginDrawing(w, GRAY);

        /*
        // Pixel //
        pixel.Draw();
        
        dm.DrawPixel({315, 225}, RED, 2);
        
        // Line //
        
        line.Draw();
        
        dm.DrawLine({320, 220}, {360, 220}, WHITE, 2);
        */
        dm.EndDrawing(w);

        // Window 2 //
        
        dm.BeginDrawing(w2);
        dm.EndDrawing(w2);
    }

    return 0;
}
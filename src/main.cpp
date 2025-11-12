#include "managers/window.hpp"
#include "resources/resource_shader.hpp"
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
        
        // Logic //
        
        // Render //
        
        
        // Window 1 //
        
        //dm.BeginDrawing(w, GRAY);
        
        /*

        const int N = 100000; // número de repeticiones

        double total1 = 0.0;
        double total2 = 0.0;

        for (int i = 0; i < N; i++) {

            // ------------------------------------------------------------------- //
            auto start1 = std::chrono::high_resolution_clock::now();

            static auto pixel = dm.CreatePixel({330, 225}, GREEN, 2);
            pixel.Draw();

            auto end1 = std::chrono::high_resolution_clock::now();
            // ------------------------------------------------------------------- //
            auto start2 = std::chrono::high_resolution_clock::now();

            dm.DrawPixel({315, 225}, RED, 2);

            auto end2 = std::chrono::high_resolution_clock::now();
            // ------------------------------------------------------------------- //

            std::chrono::duration<double, std::milli> elapsed1 = end1 - start1;
            std::chrono::duration<double, std::milli> elapsed2 = end2 - start2;

            total1 += elapsed1.count();
            total2 += elapsed2.count();
        }

        // Promedio
        double avg1 = total1 / N;
        double avg2 = total2 / N;

        std::cout << "Pixel 1: " << avg1 << " ms - ";
        std::cout << "Pixel 2: " << avg2 << " ms\n";
        */

        /*

        Pixel 1: 0.00486003 ms - Pixel 2: 0.00830631 ms

        */

        dm.BeginDrawing(w, GRAY);

        static auto pixel = dm.CreatePixel({330, 225}, GREEN, 2);
        pixel.Draw();

        dm.DrawPixel({315, 225}, RED, 2);

        dm.EndDrawing(w);

        // Window 2 //
        
        dm.BeginDrawing(w2);
        dm.EndDrawing(w2);
    }

    return 0;
}
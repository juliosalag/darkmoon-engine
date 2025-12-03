#include "utils/color.hpp"
#include <darkmoon.hpp>

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
    //auto pixel = Pixel({330, 225}, GREEN, 1, w, dm.GetBasicShader2D());
    //auto line = dm.CreateLine({520, 220}, {560, 220}, GREEN, 1);
    
    static auto pixel = Pixel({330, 225}, RED, 4, &w, dm.GetBasicShader2D());
    static auto line = Line({520, 220}, {560, 220}, GREEN, 1, &w, dm.GetBasicShader2D());
    static auto triangle_lines = TriangleLines({400, 0}, {120, 30}, {30, 220}, RED, 1, &w, dm.GetBasicShader2D());

    dm.FocusWindow(w2);

    static auto pixel2 = Pixel({130, 225}, GREEN, 1, &w2, dm.GetBasicShader2D());

    dm.FocusWindow(w);
    
    
    while(!w.ShouldClose()&& !w2.ShouldClose()){
        
        // ----- //
        // Logic //
        // ----- //
        
        w.PollEvents();
        
        //std::cout << pixel.GetPosition().x << " - " << pixel.GetPosition().y << "\n";
        
        if(w.IsKeyDown(KEY_W)){
            //std::cout << "W\n";
            pixel.SetPosition({pixel.GetPosition().x, pixel.GetPosition().y - 1});
        }
        
        if(w.IsKeyDown(KEY_S)){
            //std::cout << "S\n";
            pixel.SetPosition({pixel.GetPosition().x, pixel.GetPosition().y + 1});
        } 
        
        if(w.IsKeyDown(KEY_A)){
            //std::cout << "A\n";
            pixel.SetPosition({pixel.GetPosition().x - 1, pixel.GetPosition().y});
        } 
        
        if(w.IsKeyDown(KEY_D)){
            //std::cout << "D\n";
            pixel.SetPosition({pixel.GetPosition().x + 1, pixel.GetPosition().y});
        } 
        
        // --------------- //

        w2.PollEvents();
        
        if(w2.IsKeyDown(KEY_W)){
            //std::cout << "W\n";
            pixel2.SetPosition({pixel2.GetPosition().x, pixel2.GetPosition().y - 1});
        }
        
        if(w2.IsKeyDown(KEY_S)){
            //std::cout << "S\n";
            pixel2.SetPosition({pixel2.GetPosition().x, pixel2.GetPosition().y + 1});
        } 
        
        if(w2.IsKeyDown(KEY_A)){
            //std::cout << "A\n";
            pixel2.SetPosition({pixel2.GetPosition().x - 1, pixel2.GetPosition().y});
        } 
        
        if(w2.IsKeyDown(KEY_D)){
            //std::cout << "D\n";
            pixel2.SetPosition({pixel2.GetPosition().x + 1, pixel2.GetPosition().y});
        } 
        
        // ------ //
        // Render //
        // ------ //
        
        // Window 1 //
        
        dm.BeginDrawing(w, GRAY);
        
        /*
        // Pixel //
        */
        line.Draw();
        dm.DrawTriangle({0, 0}, {120, 30}, {30, 220}, {163, 73, 164, 255});
        triangle_lines.Draw();
        
        dm.DrawRectangle({200, 200}, {400, 200}, {200, 400}, {400, 400}, {0, 255, 255, 255});
        dm.DrawRectangleLines({200, 200}, {400, 200}, {200, 400}, {400, 400}, {0, 0, 255, 255}, 1);
        
        dm.DrawPixel({315, 225}, RED, 2);
        /*
        // Line //
        
        line.Draw();
        
        dm.DrawLine({320, 220}, {360, 220}, WHITE, 2);
        */
        pixel.Draw();

        dm.EndDrawing(w);

        // Window 2 //
        
        dm.BeginDrawing(w2, BLACK);

        pixel2.Draw();

        dm.EndDrawing(w2);
    }

    return 0;
}
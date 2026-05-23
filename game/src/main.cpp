#include <darkmoon/darkmoon.hpp>

int main(){
    DarkMoonEngine dm{};

    Window win = Window(800, 600, "ABBA collisions");

    win.SetTargetFPS(120);
    win.SetPosition({20, 50});

    // Debug Text
    Text txt_fps = Text({10, 10}, std::to_string(win.GetFPS()) + " FPS", "./assets/roboto.ttf", &win);
    Text txt_dt  = Text({10, 50}, std::to_string(win.GetDeltaTime() * 1000).substr(0, 5) + " ms", "./assets/roboto.ttf", &win);

    float offset = 30.0f;

    while(!win.ShouldClose()){
    
        // DRAWING
        
        win.BeginDrawing(BLACK);

        // DEBUG TEXT
        Rectangle({1, 1}, 
                  {txt_dt.MeasureText() + offset, 1}, 
                  {1, txt_fps.GetTextHeight() + txt_dt.GetTextHeight() + offset}, 
                  {txt_dt.MeasureText() + offset, txt_fps.GetTextHeight() + txt_dt.GetTextHeight() + offset}, BLACK, &win).Draw();

        RectangleLines({1, 1}, 
                       {txt_dt.MeasureText() + offset, 1}, 
                       {1, txt_fps.GetTextHeight() + txt_dt.GetTextHeight() + offset}, 
                       {txt_dt.MeasureText() + offset, txt_fps.GetTextHeight() + txt_dt.GetTextHeight() + offset}, WHITE, 1, &win).Draw();
        
        txt_fps.SetText(std::to_string(win.GetFPS()) + " FPS");
        txt_fps.Draw();
        
        txt_dt.SetText(std::to_string(win.GetDeltaTime() * 1000).substr(0, 5) + " ms");
        txt_dt.Draw();

        win.EndDrawing();
    }
}
#include <darkmoon/graphics.hpp>

int main() {
    DarkMoonEngine dm{};

    Window win = Window(800, 600, "ClipboardTest");

    std::string pathTexture {};

    while(!win.ShouldClose()){

        // Copy to cliboard
        if(win.IsKeyPressed(KEY_C))
            win.SetClipboardString("Hello from DarkMoon!");

        // Read the cliboard
        if(win.IsKeyPressed(KEY_V)){
            const char* text = win.GetClipboardString();
            if(text)
                std::cout << "Clipboard: " << text << "\n";
        }

        // Read the path from dropped file
        if(win.IsFileDropped()){
            std::cout << win.GetLastDroppedPath() << "\n";
            if(win.GetLastDroppedPath().ends_with(".png")){
                pathTexture = win.GetLastDroppedPath();
            }
            win.ClearDroppedPath();
        }

        win.BeginDrawing(BLACK);

        // Load texture from path
        if(!pathTexture.empty())
            Texture({win.GetWidth() / 2, win.GetHeight() / 2}, {10, 10}, 0, pathTexture.c_str(), &win).Draw();

        win.EndDrawing();
    }
}
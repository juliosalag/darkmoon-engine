#include <darkmoon.hpp>

int main() {
    DarkMoonEngine dm; // Init GLFW

    /*
    Window w, w2 {}; // Excepcion

    w.focus();
    
    while(!w.shouldClose() && !w.shouldClose()){

        // Input

        // Logic

        BeginDrawing(w);
        dm.drawPixel();
        EndDrawing();

        BeginDrawing(w2);
        dm.drawPixel();
        EndDrawing();
    }
    */

    /*
    Window w = dm.GetActiveWindow();

    w.setBorderless(dm.GetActiveMonitor());

    dm.SetActiveWindow(w);
    dm.SetBorderless();

    Window w2;

    w2.Hide();
    w2.Show();

    dm.SetActiveWindow(w2);
    */

    if(dm.InitWindow(800, 600, "DarkMoon Engine")){ // Create and active

        dm.SetWindowIcon("./assets/defaultTexture.png");

        while(!dm.WindowShouldClose()){
            
            // Prueba: Pantalla Completa / Ventana
            if(dm.IsKeyPressed(GLFW_KEY_9))
                dm.SetFullscreen();
            else if(dm.IsKeyPressed(GLFW_KEY_0))
                dm.SetWindowed();

            // Prueba: Opacidad
            if(dm.IsKeyPressed(GLFW_KEY_7))
                dm.SetWindowOpacity(dm.GetWindowOpacity() - 0.01f);
            else if(dm.IsKeyPressed(GLFW_KEY_8))
                dm.SetWindowOpacity(dm.GetWindowOpacity() + 0.01f);
 
            // Prueba: Render
            dm.BeginDrawing();
            dm.ClearBackground(GRAY);
            dm.EndDrawing();
        }
    }


    /*
    if(prueba.InitWindow()){
        while(!prueba.WindowShouldClose()){

        }
        prueba.CloseWindow();
    }

    if(dm.initWindow(800, 600, "DarkMoon Engine") ){
        while(!dm.windowShouldClose()) {
            dm.beginDrawing();

            dm.clearBackground(GRAY);

            dm.endDrawing();
        }

        dm.closeWindow();
    }
    */

    return 0;
}
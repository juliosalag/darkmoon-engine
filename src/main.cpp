#include <darkmoon.hpp>

int main() {
    DarkMoonEngine dm;

    Window w = Window(400, 300, "Game");
    Window w2 = Window(400, 300, "Debug");

    w.SetPosition(10, 10);
    w2.SetPosition(420, 10);

    w.Focus();

    while(!w.ShouldClose() && !w2.ShouldClose()){

        // Logic //

        if(glfwGetKey(w.GetWindow(), GLFW_KEY_0) || glfwGetKey(w2.GetWindow(), GLFW_KEY_0))
            w2.Hide();
        else if(glfwGetKey(w.GetWindow(), GLFW_KEY_9) || glfwGetKey(w2.GetWindow(), GLFW_KEY_9))
            w2.Show();
        
        // Render //

        w.BeginDrawing();
        dm.ClearBackground(GRAY);
        w.EndDrawing();

        // -----------------------------------------

        w2.BeginDrawing();
        dm.ClearBackground(BLACK);
        w2.EndDrawing();
    }

    return 0;
}
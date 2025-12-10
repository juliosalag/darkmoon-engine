#include "2D/basic/pixel.hpp"
#include <darkmoon.hpp>

int main() {

    DarkMoonEngine dm;

    auto w = Window(400, 300, "Prueba");

    const std::string basicVertex = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;

        void main(){
            gl_Position = vec4(aPos, 1.0);
        }
    )";

    const std::string basicFragment = R"(
        #version 330 core
        out vec4 FragColor;
        uniform vec4 customColor;

        void main(){
            FragColor = customColor;
        }
    )";

    auto shader = dm.CreateShader(basicVertex, basicFragment);
    auto pixel = Pixel({20, 20}, GREEN, 2, &w, shader);

    w.SetPosition(30, 60);
    
    while(!w.ShouldClose()){
        
        w.BeginDrawing(GRAY);

        dm.DrawPixel({10, 10}, RED, &w, 3, shader);
        pixel.Draw();

        w.EndDrawing();
    }

    return 0;
}
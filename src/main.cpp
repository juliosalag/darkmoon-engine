#include <darkmoon.hpp>

int main() {
    DarkMoonEngine dm;
    
    if(dm.initWindow(800, 600, "DarkMoon Engine") ){
        while(!dm.windowShouldClose()) {
            dm.beginDrawing();

            dm.clearBackground(GRAY);

            dm.endDrawing();
        }

        dm.closeWindow();
    }

    return 0;
}
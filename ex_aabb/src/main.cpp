#include <darkmoon/darkmoon.hpp>

constexpr static float speed = 300.0f;

struct Entity {
    Rectangle* collisionBox {};
    Vector2Df velocity {};
};

int main(){
    DarkMoonEngine dm{};

    Window win = Window(800, 600, "AABB collisions");

    win.SetTargetFPS(120);
    win.SetPosition({20, 50});

    // Player
    Entity player = Entity();
    auto rect = Rectangle({100, 100}, 50, 50, GREEN, &win);
    player.collisionBox = &rect;

    // Wall
    Rectangle wall = Rectangle({static_cast<float>(win.GetWidth() / 2), 0}, 30, static_cast<float>(win.GetHeight()), WHITE, &win);

    // Debug Text
    Text txt_fps = Text({10, 10}, std::to_string(win.GetFPS()) + " FPS", "./assets/roboto.ttf", &win);
    Text txt_dt  = Text({10, 50}, std::to_string(win.GetDeltaTime() * 1000).substr(0, 5) + " ms", "./assets/roboto.ttf", &win);

    float offset = 30.0f;

    while(!win.ShouldClose()){
        
        // INPUT

        if(win.IsKeyDown(KEY_UP))
            player.velocity.y = - speed * win.GetDeltaTime();
        if(win.IsKeyDown(KEY_DOWN))
            player.velocity.y = + speed * win.GetDeltaTime();
        if(win.IsKeyDown(KEY_LEFT))
            player.velocity.x = - speed * win.GetDeltaTime();
        if(win.IsKeyDown(KEY_RIGHT))
            player.velocity.x = + speed * win.GetDeltaTime();

        // COLLISIONS

        // AABB -> Axis-Aligned Bounding Box
        //
        // left < right &&
        // right > left &&
        // top < bottom &&
        // bottom > top

        Vector2Df nextPos = player.collisionBox->GetPosition() + player.velocity;

        if(nextPos.x < wall.GetPosition().x + wall.GetWidth() &&
           nextPos.x + player.collisionBox->GetWidth() > wall.GetPosition().x &&
           nextPos.y < wall.GetPosition().y + wall.GetHeight() &&
           nextPos.y + player.collisionBox->GetHeight() > wall.GetPosition().y)
        {
            player.collisionBox->SetColor(RED);
            //player.velocity = {};
        }
        else{
            player.collisionBox->SetColor(GREEN);
        }

        player.collisionBox->SetPosition(player.collisionBox->GetPosition() + player.velocity);
        player.velocity = {};

        // DRAWING
        
        win.BeginDrawing(BLACK);

        // Wall
        wall.Draw();
        
        // Player
        player.collisionBox->Draw();

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
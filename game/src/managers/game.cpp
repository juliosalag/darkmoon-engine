#include "game.hpp"

#include "./utils/memviewer.hpp"
#include "./utils/slotmap.hpp"
#include  <cstdio>

struct NameComponent {
    char name[8] {};
};

void Game::run(){

    Slotmap<NameComponent, 4> names;
    MemoryViewer MV{names};

    NameComponent prueba = NameComponent{"cucu"};

    [[maybe_unused]]auto key0 = names.push_back( NameComponent{"Prueba"} );
    key0 = names.push_back( NameComponent{"Prueba"} );
    key0 = names.push_back( NameComponent{"Prueba"} );
    auto key = names.push_back(prueba);

    MV.printMemory();

    names.erase(key);
    
    MV.printMemory();

    std::printf("Full traversal\n");
    //for(auto it = names.begin(); it != names.end(); it++){
    for(auto const& cmp : names){
        std::printf("Item: %s\n", cmp.name);
    }


    /*
    auto w1 = Window(600, 500, "Game");
    w1.SetPosition(30, 60);

    auto w2 = Window(300, 450, "Debug", &w1);
    w2.SetPosition(660, 60);
    */

    /*
    // Title
    auto* e1 = manager.createEntity();
    e1->addRenderComponent({0, 0}, "Debug Menu", &w2);
    auto& r1 = e1->renderComponent;
    r1->text->SetPosition({(w2.GetWidth() / 2) - static_cast<int>(r1->text->MeasureText() / 2), 10});

    // Subtitle
    auto* e2 = manager.createEntity();
    e2->addRenderComponent({0, 0}, "Carlitas Ñ", &w2);
    auto& r2 = e2->renderComponent;
    r2->text->SetPosition({(w2.GetWidth() / 2) - static_cast<int>(r2->text->MeasureText() / 2), 50});
    r2->text->SetColor(GRAY);
    r2->text->SetScale(0.5f);
    */

    /*
    w1.Focus();

    while(!w1.ShouldClose() && !w2.ShouldClose()){

        // Draw game window
        w1.BeginDrawing(GRAY); w1.EndDrawing();
        
        // Draw debug window
        w2.BeginDrawing(BLACK);

        // Update text
        //r1->text->SetPosition({(w2.GetWidth() / 2) - static_cast<int>(r1->text->MeasureText() / 2), 10});
        //r2->text->SetPosition({(w2.GetWidth() / 2) - static_cast<int>(r2->text->MeasureText() / 2), 50});

        // Render System
        //renderSystem.render(manager);
        
        w2.EndDrawing();

    }
    */
}
#pragma once

#include "../utils/types.hpp"

struct RenderSystem {
public:

    using SYSCMPs = MP::TypeList<RenderComponent>;
    using SYSTAGs = MP::TypeList<>;

    void update(EM& manager, Window& w) {
        manager.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, RenderComponent& r){
            //std::cout << e.getID() << "\n";

            if(e.getID() == static_cast<size_t>(0)){
                r.text->SetPosition({(w.GetWidth() / 2) - static_cast<int>(r.text->MeasureText() / 2), 10});
                //std::cout << r.text->GetPosition().x << " | " << r.text->GetPosition().y << "\n";
            }
            else if(e.getID() == static_cast<size_t>(1)){
                r.text->SetPosition({(w.GetWidth() / 2) - static_cast<int>(r.text->MeasureText() / 2), 50});
            }

            r.text->Draw();
        });
    }

};
#pragma once

#include "../utils/types.hpp"

struct RenderSystem {
public:

    using SYSCMPs = MP::TypeList<RenderComponent>;
    using SYSTAGs = MP::TypeList<>;

    void update(EM& manager, Window& w) {
        manager.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, RenderComponent& r){
            if(e.hasComponent<PhysicsComponent>())
                r.pixel->SetPosition(manager.getComponent<PhysicsComponent>(e).position);

            Pixel({w.GetWidth() / 2, w.GetHeight() / 2}, RED, 4, &w).Draw();

            r.pixel->Draw();
        });
    }

};
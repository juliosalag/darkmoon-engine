#pragma once

#include "../utils/types.hpp"

struct PhysicsSystem {
public:
    using SYSCMPs = MP::TypeList<PhysicsComponent>;
    using SYSTAGs = MP::TypeList<>;

    void update(EM& manager, Window& w) {
        manager.forEach<SYSCMPs, SYSTAGs>([&](Entity&, PhysicsComponent& p){
            p.position.x += p.velocity.x;

            if(p.position.x >= w.GetWidth()){
                p.position.x = 0;
                p.position.y = static_cast<int>(std::rand() % w.GetHeight());
            }

        });
    }
};
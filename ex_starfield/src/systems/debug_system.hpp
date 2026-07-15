#pragma once

#include "../utils/types.hpp"

struct DebugSystem {
public:

    using SYSCMPs = MP::TypeList<RenderComponent, PhysicsComponent>;
    using SYSTAGs = MP::TypeList<>;

private:
    // TextComponents
    std::unique_ptr<Text> m_title, m_subtitle;

public:
    DebugSystem(Window& w){
        m_title = std::make_unique<Text>(Vector2D{ 0 , 0 }, "Debug Menu", "./assets/roboto.ttf", &w);
        m_subtitle = std::make_unique<Text>(Vector2D{ 0 , 0 }, "Entities: ", "./assets/roboto.ttf", &w);
        m_subtitle->SetColor(GRAY);
        m_subtitle->SetScale(0.5f);

    }

    void update(EM& manager, Window& w) {

        m_title->SetPosition({(w.GetWidth() / 2) - static_cast<int>(m_title->MeasureText() / 2), 10});
        m_subtitle->SetPosition({(w.GetWidth() / 2) - static_cast<int>(m_subtitle->MeasureText() / 2), 50});
        std::string ent = "Entities: " + std::to_string(manager.aliveCount());
        m_subtitle->SetText(ent);

        m_title->Draw();
        m_subtitle->Draw();

        Line({0, 75}, {w.GetWidth(), 75}, GRAY, 1, &w).Draw();

        int padding = 130;

        manager.forEach<SYSCMPs, SYSTAGs>([&](Entity& e, RenderComponent& , PhysicsComponent& p){
            Text txt = Text({20, padding}, "Entity ID " + std::to_string(e.getID()) + ": Position --> " + std::to_string(p.position.x) + " | " + std::to_string(p.position.y), "./assets/roboto.ttf", &w);
            txt.SetScale(0.7f);
            txt.Draw();

            padding += 30;
        });
    }

};
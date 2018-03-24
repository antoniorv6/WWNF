#pragma once

#include <SFML/Graphics.hpp>
#include "State.hpp"
#include "Game.hpp"
#include "Trampa.hpp"
#include "Placer.hpp"
#include "HUD.hpp"
#include "FichaTrampa.hpp"
#include "Enemigo.hpp"
#include <vector>

namespace Zenon {

    class SplashState : public State {
    public:
        SplashState(GameDataRef l_data, std::vector<FichaTrampa*> l_fichaTrampa,int l_dinero);

        void Init();

        void HandleInput();
        void Update(float dt);
        void Draw(float dt);

    private:
        GameDataRef m_data;
        int m_trampa;
        sf::Texture _backgroundTexture;
        sf::Sprite _background;
        std::vector<Trampa*> m_trampas;
        std::vector<Placer*> m_placer;
        HUD * m_hud;
        bool m_noCompruebes;
        std::vector<FichaTrampa*> m_trampasSel;
        sf::Sprite m_mouseConstruct;
        bool m_hideCursor;
        std::vector<Enemigo*> m_enemys;
        sf::Clock m_enemy_dead;
        int  m_disponible;
        sf::Text m_textoDinero;
    };
}
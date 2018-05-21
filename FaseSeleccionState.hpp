#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "State.hpp"
#include "Game.hpp"
#include "FichaTrampa.hpp"
#include "HUD.hpp"
#include <vector>

namespace Zenon {

    class FaseSeleccionState : public State {
    public:
        FaseSeleccionState(GameDataRef l_data);
        void Init();
        void InitOptions();
        void HandleInput();
        void Update(float dt);
        void Draw(float dt);
        void ChangeState();
    private:
        GameDataRef m_data;
        std::vector<FichaTrampa *> m_trampasdisp;
        std::vector<FichaTrampa *> m_trampasSel;
        sf::Sprite m_boton;
        sf::Sprite m_background;
        sf::Sprite m_bgMoney;
        
        sf::Text m_introText;
        sf::Text m_advertiseText;
        
        int m_dineroJugador;
        int m_random;

        //Sistema de recompensas
        sf::Sprite m_emptyMap;
        std::vector<sf::Sprite> m_onBottom;
        std::vector<sf::Sprite> m_offBottom;
        std::vector<bool> m_boolVector;
        std::vector<sf::Sprite> m_mapOptions;
        int m_numBool = 3;
        bool m_downMoney=false;
        sf::Text m_moneyText;
        int op1 = 50;
        int op2 = 700;
        int bottX = 630;
        int bottY = 800;
        int m_Sum;
        int m_whSum;
        
        sf::Music * m_theme;
    };
}
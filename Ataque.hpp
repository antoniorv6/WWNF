#pragma once
#include "Game.hpp"
#include "Enemigo.hpp"
#include "Bala.hpp"
#include <vector>
#include <SFML/Graphics.hpp>

namespace Zenon{

    class Ataque{
    public: 
        Ataque(GameDataRef l_data, sf::Vector2f l_position);
        void Update(float dt);
        void Draw();
        void FindTarget(std::vector<Enemigo *> &l_enemys);
        bool IsAttacking();
        void Attack();

    private:
        GameDataRef m_data;
        sf::Sprite m_mainSprite;
        bool m_is_attacking;
        Enemigo * m_target;
        std::vector<Bala *> m_bala;
        sf::Clock shoot_time;
        sf::Vector2f m_direccion_sentido;

      //  sf::Vector2f m_positionTurret;

    };
    
}
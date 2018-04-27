#pragma once
#include "Game.hpp"
#include <vector>
#include <SFML/Graphics.hpp>

namespace Zenon {

    class Bala {
    public:
        Bala(GameDataRef l_data, sf::Vector2f l_position, sf::Vector2f l_enemigo, float l_angulo, int l_power, int l_id,int l_tipo);
        ~Bala();
        void Update(float dt);
        void Draw();
        void Shoot(float angulo, float incr_x, float incr_y);
        const sf::Sprite &GetSprite() const;
        void setOcupada();
        bool getOcupada();
        int GetPower();
        int GetType();
        int WhoShooted();
        sf::Vector2f GetOrigin();


    private:
        GameDataRef m_data;
        sf::Sprite m_mainSprite;
        sf::Vector2f m_positionBullet;
        sf::Vector2f m_direccion;
        float m_angulo;
        bool m_shoot;
        bool m_ocupada;
        int m_power;
        int m_who;
        int m_type;
        

    };

}
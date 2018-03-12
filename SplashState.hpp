#pragma once

#include <SFML/Graphics.hpp>
#include "State.hpp"
#include "Game.hpp"
#include <vector>

namespace Zenon {

    class SplashState : public State {
    public:
        SplashState(GameDataRef data);

        void Init();

        void HandleInput();
        void Update(float dt);
        void Draw(float dt);

        std::vector<sf::Vector2f> CalculateBQ
        (
                const sf::Vector2f &start,
                const sf::Vector2f &end,
                const sf::Vector2f &startControl,
                const sf::Vector2f &endControl,
                const std::size_t nSegments
                );

        sf::VertexArray ToVertex(std::vector<sf::Vector2f> l_points);

    private:
        GameDataRef _data;

        sf::Clock _clock;

        sf::Texture _backgroundTexture;
        sf::Sprite _background;
        std::vector<SplashState*> states;

    };
}
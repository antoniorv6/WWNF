

#include "Enemy.hpp"
#include "MathHelper.hpp"

namespace Zenon {

    class Doxy : public Enemy {
    public:
        Doxy(GameDataRef l_data, Enemy::TYPE l_type, sf::Vector2f l_position, const std::vector<Enemy*>& l_neighbors, const std::vector<Bezier>& l_routes, int l_path);
        ~Doxy();

        void Update(float dt);
        void CheckRoutes();
        int GetCurrentWP();
        Bezier GetPath();

    private:

        Bezier m_path;
        std::vector<Bezier> m_routes;
        int m_currentWP = 1;
        int l_rNumber = 0;
        bool m_pathCompleted = false;

        sf::Vector2f m_resultant;
        std::vector<sf::Vector2f> m_resultantAcc;


    };
}




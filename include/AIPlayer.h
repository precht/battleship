#ifndef AI_PLAYER_H_
#define AI_PLAYER_H_

#include "Player.h"
#include "ShootStrategy.h"

#include <utility>
#include <memory>

namespace battleship
{

    class AIPlayer : public Player
    {
    public:
        AIPlayer(std::unique_ptr<ShootStrategy> strategy_ptr);
        ~AIPlayer() override = default;

        void setUpShips() override;
        std::pair<int, int> shoot() override;

    private:
        std::unique_ptr<ShootStrategy> strategy_ptr_;

        static const int MAX_ATTEMPTS = 50;
    };

}

#endif // !AI_PLAYER_H_

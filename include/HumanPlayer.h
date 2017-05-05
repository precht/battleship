#ifndef HUMAN_PLAYER_H_
#define HUMAN_PLAYER_H_

#include "Player.h"
#include "UI.h"

#include <utility>
#include <memory>

namespace battleship
{

    class HumanPlayer : public Player
    {
    public:
        HumanPlayer(std::shared_ptr<UI> ui);
        ~HumanPlayer() override = default;

        void setUpShips() override;
        std::pair<int, int> shoot() override;

    private:
        std::shared_ptr<UI> ui_;
    };

}

#endif // !HUMAN_PLAYER_H_

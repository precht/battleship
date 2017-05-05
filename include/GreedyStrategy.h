#ifndef GREEDY_STRATEGY_H_
#define GREEDY_STRATEGY_H_

#include "ShootStrategy.h"
#include "ShipsGrid.h"

#include <utility>
#include <vector>
#include <unordered_set>
#include <memory>

namespace battleship
{

    class GreedyStrategy : public ShootStrategy
    {
    public:
        GreedyStrategy() = default;
        ~GreedyStrategy() override = default;

        int chooseShip(std::unique_ptr<std::vector<int>> ships_lengths) override;
        std::pair<int,int> chooseSquare(std::unique_ptr<std::unordered_set<std::pair<int,int>, SquareHash>> squares) override;
    };

}

#endif // !GREEDY_STRATEGY_H_

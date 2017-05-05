#ifndef SHOOT_STRATEGY_H_
#define SHOOT_STRATEGY_H_

#include "ShipsGrid.h"

#include <utility>
#include <vector>
#include <unordered_set>
#include <memory>

namespace battleship
{

    class ShootStrategy
    {
    public:
        virtual ~ShootStrategy() = default;

        virtual int chooseShip(std::unique_ptr<std::vector<int>> ships_lengths) = 0;
        virtual std::pair<int,int> chooseSquare(std::unique_ptr<std::unordered_set<std::pair<int,int>, SquareHash>> squares) = 0;
    };

}

#endif // !SHOOT_STRATEGY_H_


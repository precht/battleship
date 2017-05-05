#ifndef SHOOT_STRATEGY_MOCK_H_
#define SHOOT_STRATEGY_MOCK_H_

#include "ShootStrategy.h"
#include "ShipsGrid.h"

#include "gmock/gmock.h"

namespace battleship_test
{

class MockShootStrategy : public battleship::ShootStrategy
{
public:
    MOCK_METHOD1(chooseShipProxy, int(std::vector<int>* ships_lengths));
    MOCK_METHOD1(chooseSquareProxy, std::pair<int,int>(std::unordered_set<std::pair<int,int>, battleship::SquareHash>* squares));

    // we cannot mock mehtod that take unique_ptr, thus we will delegate ptr to method that can take it
    int chooseShip(std::unique_ptr<std::vector<int>> ships_lengths) override;
    std::pair<int, int> chooseSquare(std::unique_ptr<std::unordered_set<std::pair<int,int>,
                                     battleship::SquareHash>> squares) override;
};

} // battleship_test

#endif // SHOOT_STRATEGY_MOCK_H_

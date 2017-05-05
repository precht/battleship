#include "test/ShootStrategy_mock.h"


int battleship_test::MockShootStrategy::chooseShip(std::unique_ptr<std::vector<int>> ships_lengths)
{
    return chooseShipProxy(ships_lengths.get());
}

std::pair<int, int> battleship_test::MockShootStrategy::chooseSquare(
        std::unique_ptr<std::unordered_set<std::pair<int,int>, battleship::SquareHash>> squares)
{
    return chooseSquareProxy(squares.get());

}

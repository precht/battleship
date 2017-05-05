#include "test/Player_mock.h"
#include "Ship.h"
#include "exceptions.h"

#include <memory>
#include <vector>
#include <utility>

using battleship::Ship;

using std::pair;
using std::vector;
using std::make_pair;
using std::make_unique;


void battleship_test::MockPlayer::mockSetUpShips()
{
    auto v = Ship::makeVectorPtr({ {2,2} });
    primary_grid_.setShipLocation(move(v));

    v = Ship::makeVectorPtr({ {6,3}, {6,4} });
    primary_grid_.setShipLocation(move(v));

    v = Ship::makeVectorPtr({ {3,8}, {4,8}, {5,8} });
    primary_grid_.setShipLocation(move(v));
}

void battleship_test::MockPlayer::mockSetUpShipsWithArgs(std::initializer_list<pair<int, int>>&& init_list)
{
    if (init_list.size() != 6)
        throw battleship::BattleshipRuntimeError("MockPlayer::mockSetUpShipsWithArgs: wrong initializer_list size.");

    auto it = init_list.begin();
    primary_grid_.setShipLocation(Ship::makeVectorPtr({ *it }));
    primary_grid_.setShipLocation(Ship::makeVectorPtr({ *(it + 1), *(it + 2) }));
    primary_grid_.setShipLocation(Ship::makeVectorPtr({ *(it + 3), *(it + 4), *(it + 5) }));
}

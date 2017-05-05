#ifndef PLAYER_MOCK_H_
#define PLAYER_MOCK_H_

#include "Player.h"
#include "gmock/gmock.h"

#include <initializer_list>

namespace battleship_test
{

class MockPlayer : public battleship::Player
{
public:
    MOCK_METHOD0(setUpShips, void());
    MOCK_METHOD0(shoot, std::pair<int, int>());

    // this method always sets ships at locations:
    // (2,2),  (6,3)(6,4),  (3,8)(4,8)(5,8)
    void mockSetUpShips();
    void mockSetUpShipsWithArgs(std::initializer_list<std::pair<int,int>>&& l);
};

} // battleship_test

#endif // PLAYER_MOCK_H_

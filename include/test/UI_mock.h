#ifndef UI_MOCK_H_
#define UI_MOCK_H_

#include "UI.h"
#include "Player.h"

#include "gmock/gmock.h"
#include <string>
#include <memory>

namespace battleship_test
{

class MockUI : public battleship::UI
{
public:
    MOCK_METHOD0(create, void());
    MOCK_METHOD0(destroy, void());

    MOCK_METHOD1(displayPlayer, void(const battleship::Player& player));
    MOCK_METHOD2(displayPlayers, void(const battleship::Player& player, const battleship::Player& opponent));
    MOCK_METHOD1(displayMessage, void(const std::string& message));

    MOCK_METHOD0(cleanScreen, void());

    MOCK_METHOD0(chooseShip, int());
    MOCK_METHOD0(chooseSquare, std::pair<int, int>());
    MOCK_METHOD1(askQuestion, bool(const std::string& question));
};

} // battleship_test

#endif // UI_MOCK_H_

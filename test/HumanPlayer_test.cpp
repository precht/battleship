#include "HumanPlayer.h"
#include "test/UI_mock.h"

#include "gtest/gtest.h"
#include <memory>

using namespace battleship;
using battleship_test::MockUI;

struct HumanPlayerTest : public ::testing::Test
{
    HumanPlayer p;
    HumanPlayerTest()
        : p(std::make_shared<MockUI>())
    { }
};

TEST_F(HumanPlayerTest, dummy) { }

TEST_F(HumanPlayerTest, logic_errors)
{
    EXPECT_THROW(p.shoot(), BattleshipLogicError) << "cannot shoot before setting ships location.";
}

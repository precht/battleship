#include "AIPlayer.h"
#include "test/ShootStrategy_mock.h"

#include "gtest/gtest.h"
#include <memory>

using namespace battleship;
using battleship_test::MockShootStrategy;

struct AIPlayerTest : public ::testing::Test
{
    AIPlayer p;
    AIPlayerTest()
        : p(std::make_unique<MockShootStrategy>())
    { }
};

TEST_F(AIPlayerTest, dummy) { }

TEST_F(AIPlayerTest, logic_errors)
{
    EXPECT_THROW(p.shoot(), BattleshipLogicError) << "cannot shoot before setting ships location.";
}

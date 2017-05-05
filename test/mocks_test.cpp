#include "test/UI_mock.h"
#include "test/Player_mock.h"
#include "test/ShootStrategy_mock.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <utility>

using namespace battleship_test;
using ::testing::Invoke;
using std::make_pair;


TEST(MocksTest, MockUI)
{
    (void)MockUI();
}

TEST(MocksTest, MockPlayer)
{
    MockPlayer mp;
    mp.mockSetUpShipsWithArgs({ {0,0},  {3,3},{3,4},  {7,9},{7,8},{7,7} });
    auto& g = mp.getPrimaryGird();
    EXPECT_EQ(g.getShip(1).getOccupiedSquares()->at(0), make_pair(0,0));
    EXPECT_EQ(g.getShip(2).getOccupiedSquares()->at(0), make_pair(3,3));
    EXPECT_EQ(g.getShip(2).getOccupiedSquares()->at(1), make_pair(3,4));
    EXPECT_EQ(g.getShip(3).getOccupiedSquares()->at(0), make_pair(7,7));
    EXPECT_EQ(g.getShip(3).getOccupiedSquares()->at(1), make_pair(7,8));
    EXPECT_EQ(g.getShip(3).getOccupiedSquares()->at(2), make_pair(7,9));
}

TEST(MocksTest, MockShootStrategy)
{
    (void)MockShootStrategy();
}

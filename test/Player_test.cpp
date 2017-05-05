#include "Player.h"
#include "exceptions.h"
#include "test/Player_mock.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <set>
#include <utility>

using namespace battleship;
using battleship_test::MockPlayer;

using ::testing::Invoke;
using ::testing::Return;
using std::make_pair;
using std::pair;

struct PlayerTest : public ::testing::Test
{
    MockPlayer p;

    PlayerTest()
    {
        // Let setUpShips() put ships in locations hardcoded in mockSetUpShips():
        // (2,2),  (6,3)(6,4),  (3,8)(4,8)(5,8)
        EXPECT_CALL(p, setUpShips())
                .WillRepeatedly(Invoke(&p, &MockPlayer::mockSetUpShips));
        p.setUpShips();
    }
};

TEST_F(PlayerTest, dummy) { }

TEST_F(PlayerTest, initial_state)
{
    MockPlayer p1;
    EXPECT_FALSE(p1.canShoot()) << "cannot shoot before setting ships";

    // grids should be filled with empty squares
    auto& pg = p1.getPrimaryGird();
    auto& sg = p1.getSecondaryGrid();
    for (auto a = 0; a < 10; a++)
    {
        for (auto b = 0; b < 10; b++)
        {
            EXPECT_EQ(sg.at(make_pair(a,b)), ST_EMPTY);
            EXPECT_EQ(pg.at(make_pair(a,b)), ST_EMPTY);
        }
    }
}

TEST_F(PlayerTest, primary_grid)
{
    auto& grid = p.getPrimaryGird();

    // let's shoot all player's ships
    // note that takeShoot concerns opponent's shots and canShoot concern player's shots
    // canShoot method is checked wheater it correctly reads when all ships are sunk

    EXPECT_TRUE(p.canShoot());

    // single
    EXPECT_EQ(grid.at({2,2}), ST_SINGLE);
    EXPECT_EQ(p.takeShot({2,2}), SR_SUNK);
    EXPECT_EQ(grid.at({2,2}), ST_SUNK);

    // double
    EXPECT_EQ(grid.at({6,3}), ST_DOUBLE);
    EXPECT_EQ(grid.at({6,4}), ST_DOUBLE);

    EXPECT_EQ(p.takeShot({6,3}), SR_HIT);
    EXPECT_EQ(grid.at({6,3}), ST_HIT);

    EXPECT_EQ(p.takeShot({6,4}), SR_SUNK);
    EXPECT_EQ(grid.at({6,3}), ST_SUNK);
    EXPECT_EQ(grid.at({6,4}), ST_SUNK);

    // triple
    EXPECT_EQ(grid.at({3,8}), ST_TRIPLE);
    EXPECT_EQ(grid.at({4,8}), ST_TRIPLE);
    EXPECT_EQ(grid.at({5,8}), ST_TRIPLE);

    EXPECT_EQ(p.takeShot({5,8}), SR_HIT);
    EXPECT_EQ(grid.at({5,8}), ST_HIT);

    EXPECT_EQ(p.takeShot({3,8}), SR_HIT);
    EXPECT_EQ(grid.at({3,8}), ST_HIT);

    EXPECT_TRUE(p.canShoot()) << " still one ship not sunk";

    EXPECT_EQ(p.takeShot({4,8}), SR_SUNK);
    EXPECT_EQ(grid.at({3,8}), ST_SUNK);
    EXPECT_EQ(grid.at({4,8}), ST_SUNK);
    EXPECT_EQ(grid.at({5,8}), ST_SUNK);

    EXPECT_FALSE(p.canShoot()) << "all ships are sunk";

    // shoot other squares
    std::set<pair<int,int>> d({ {2,2}, {6,3}, {6,4}, {3,8}, {4,8}, {5,8} });
    for (int a = 0; a < 10; a++)
        for (int b = 0; b < 10; b++)
            if (d.find({a,b}) == d.end())
            {
                EXPECT_EQ(grid.at({a,b}), ST_EMPTY);
                EXPECT_EQ(p.takeShot({a,b}), SR_MISS);
                EXPECT_EQ(grid.at({a,b}), ST_MISS);
            }
}

TEST_F(PlayerTest, secondary_grid)
{
    auto& grid = p.getSecondaryGrid();

    // let's shoot all opponent's ships
    // assume that opponent's ships are translated by [1,1] compared to player's ships
    // with mocked shoot() method I cannot test nextRound() and canShot() method for counting shots in the round

    EXPECT_CALL(p, shoot())
            .Times(6)
            .WillOnce(Return(make_pair(3,3)))
            .WillOnce(Return(make_pair(7,4)))
            .WillOnce(Return(make_pair(7,5)))
            .WillOnce(Return(make_pair(4,9)))
            .WillOnce(Return(make_pair(6,9)))
            .WillOnce(Return(make_pair(5,9)));

    EXPECT_TRUE(p.canShoot());

    // single
    p.update(p.shoot(), SR_SUNK);
    EXPECT_EQ(grid.at({3,3}), ST_SUNK);

    // double
    p.nextRound();
    p.update(p.shoot(), SR_HIT);
    EXPECT_EQ(grid.at({7,4}), ST_HIT);

    p.update(p.shoot(), SR_SUNK);
    EXPECT_EQ(grid.at({7,4}), ST_SUNK);
    EXPECT_EQ(grid.at({7,5}), ST_SUNK);

    // triple
    p.nextRound();
    p.update(p.shoot(), SR_HIT);
    EXPECT_EQ(grid.at({4,9}), ST_HIT);

    p.update(p.shoot(), SR_HIT);
    EXPECT_EQ(grid.at({6,9}), ST_HIT);

    p.nextRound();
    p.update(p.shoot(), SR_SUNK);
    EXPECT_EQ(grid.at({4,9}), ST_SUNK);
    EXPECT_EQ(grid.at({5,9}), ST_SUNK);
    EXPECT_EQ(grid.at({6,9}), ST_SUNK);

    // update other squares
    std::set<pair<int,int>> d({ {3,3}, {7,4}, {7,5}, {4,9}, {5,9}, {6,9} });
    for (int a = 0; a < 10; a++)
        for (int b = 0; b < 10; b++)
            if (d.find({a,b}) == d.end())
            {
                EXPECT_EQ(grid.at({a,b}), ST_EMPTY);
                p.update({a,b}, SR_MISS);
                EXPECT_EQ(grid.at({a,b}), ST_MISS);
            }

    EXPECT_FALSE(p.canShoot()) << "no available range any more";
}

TEST_F(PlayerTest, set_up_errors)
{
    MockPlayer mp;

    // Following errors should not be thrown by Player but by the classes that Player is using

    // Invalid square
    ASSERT_THROW(mp.mockSetUpShipsWithArgs({ {-1,0},  {3,3},{3,4},  {7,9},{7,8},{7,7} }), InvalidCoordinateError);
    ASSERT_THROW(mp.mockSetUpShipsWithArgs({ {0,-1},  {3,3},{3,4},  {7,9},{7,8},{7,7} }), InvalidCoordinateError);
    ASSERT_THROW(mp.mockSetUpShipsWithArgs({ {10,0},  {3,3},{3,4},  {7,9},{7,8},{7,7} }), InvalidCoordinateError);
    ASSERT_THROW(mp.mockSetUpShipsWithArgs({ {0,10},  {3,3},{3,4},  {7,9},{7,8},{7,7} }), InvalidCoordinateError);


    // Invalid ship location
    ASSERT_THROW(mp.mockSetUpShipsWithArgs({ {0,0},  {1,1},{1,2},  {7,9},{7,8},{7,7} }), InvalidShipLocationError);
    ASSERT_THROW(mp.mockSetUpShipsWithArgs({ {0,0},  {7,5},{8,5},  {7,3},{7,4},{7,5} }), InvalidShipLocationError);
    ASSERT_THROW(mp.mockSetUpShipsWithArgs({ {0,0},  {3,3},{3,3},  {7,3},{7,4},{7,5} }), InvalidShipLocationError);
    ASSERT_THROW(mp.mockSetUpShipsWithArgs({ {0,0},  {3,3},{3,5},  {7,3},{7,4},{7,5} }), InvalidShipLocationError);
    ASSERT_THROW(mp.mockSetUpShipsWithArgs({ {0,0},  {3,3},{4,4},  {7,3},{7,4},{7,5} }), InvalidShipLocationError);
    ASSERT_THROW(mp.mockSetUpShipsWithArgs({ {0,0},  {3,3},{3,4},  {8,7},{9,8},{9,9} }), InvalidShipLocationError);
    ASSERT_THROW(mp.mockSetUpShipsWithArgs({ {0,0},  {3,3},{3,4},  {8,8},{9,8},{9,9} }), InvalidShipLocationError);
}

TEST_F(PlayerTest, logic_errors)
{
    MockPlayer p1;
    EXPECT_THROW(p1.nextRound(), BattleshipLogicError) << "cannot start next round before setting ships";
    p.takeShot({2,2});
    EXPECT_THROW(p.takeShot({2,2}), BattleshipRuntimeError) << "trying to shoot the same sqaure twice";
}

TEST_F(PlayerTest, runtime_errors)
{
    p.update({0,0}, SR_SUNK);
    EXPECT_THROW(p.update({0,0}, SR_SUNK), BattleshipRuntimeError) << "trying to update the same sqaure twice";
    EXPECT_THROW(p.update({2,2}, SR_SUNK), BattleshipRuntimeError) << "trying to update different ships with same size";
    EXPECT_THROW(p.update({1,1}, SR_HIT), BattleshipRuntimeError) << "trying to hit different ship too close to other";

    p.update({7,9}, SR_HIT);
    p.update({8,9}, SR_HIT);
    p.update({9,9}, SR_SUNK);

    p.update({6,4}, SR_HIT);
    p.update({7,4}, SR_HIT);
    EXPECT_THROW(p.update({8,4}, SR_SUNK), BattleshipRuntimeError) << "trying to update different ships with same size";
}

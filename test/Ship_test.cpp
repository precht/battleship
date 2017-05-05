#include "Ship.h"
#include "exceptions.h"

#include "gtest/gtest.h"
#include <vector>
#include <memory>
#include <utility>

using namespace battleship;
using std::vector;
using std::pair;
using std::make_pair;
using std::unique_ptr;
using std::make_unique;
using std::move;
using std::initializer_list;


TEST(ShipTest, dummy)
{
    (void)Ship();
}

TEST(ShipTest, initial_state)
{
    Ship s;
    EXPECT_FALSE(s.canShoot());
    EXPECT_FALSE(s.isSunk());
    EXPECT_EQ(s.getLength(), 0);
    EXPECT_EQ(s.getRange(), 0);
}

TEST(ShipTest, single_funnel_ship)
{
    // Set occupied squares
    Ship s;
    auto v = Ship::makeVectorPtr({ make_pair(0,0) });
    s.setOccupiedSquares(move(v));

    EXPECT_EQ(s.getLength(), 1);
    EXPECT_EQ(s.getRange(), 2);

    // Check if returned vecotr is correct
    v = s.getOccupiedSquares();
    ASSERT_TRUE(v) << "returned nullptr";
    ASSERT_EQ(v->size(), (size_t)1);
    EXPECT_EQ(v->at(0), make_pair(0,0));

    // Check shots per round
    EXPECT_TRUE(s.canShoot());
    s.shoot();
    EXPECT_FALSE(s.canShoot());
    s.nextRound();
    EXPECT_TRUE(s.canShoot());

    // Check sinking
    ASSERT_FALSE(s.isSunk());
    s.takeShot();
    EXPECT_TRUE(s.isSunk());
    EXPECT_FALSE(s.canShoot());

    // Check if nextRound resets sunk state
    s.nextRound();
    EXPECT_TRUE(s.isSunk());
    EXPECT_FALSE(s.canShoot());
}

TEST(ShipTest, double_funnel_ship)
{
    // Set occipied squares
    Ship s;
    auto v = Ship::makeVectorPtr({ make_pair(8,9), make_pair(9,9) });
    s.setOccupiedSquares(move(v));

    EXPECT_EQ(s.getLength(), 2);
    EXPECT_EQ(s.getRange(), 3);

    // Check if returned vecotr is correct
    v = s.getOccupiedSquares();
    ASSERT_TRUE(v) << "returned nullptr";
    ASSERT_EQ(v->size(), (size_t)2);
    EXPECT_EQ(v->at(0), make_pair(8,9));
    EXPECT_EQ(v->at(1), make_pair(9,9));

    // Check shots per round
    // When shot twice then in next round is waiting
    EXPECT_TRUE(s.canShoot());
    s.shoot();
    EXPECT_TRUE(s.canShoot());
    s.shoot();
    EXPECT_FALSE(s.canShoot());
    s.nextRound();
    EXPECT_FALSE(s.canShoot());
    s.nextRound();
    EXPECT_TRUE(s.canShoot());
    s.shoot();
    EXPECT_TRUE(s.canShoot());
    s.nextRound();
    EXPECT_TRUE(s.canShoot());

    // Check sinking
    ASSERT_FALSE(s.isSunk());
    s.takeShot();
    ASSERT_FALSE(s.isSunk());
    s.takeShot();
    EXPECT_TRUE(s.isSunk());

    // Check if nextRound resets sunk state
    EXPECT_FALSE(s.canShoot());
    s.nextRound();
    EXPECT_TRUE(s.isSunk());
    EXPECT_FALSE(s.canShoot());

}

TEST(ShipTest, triple_funnel_ship)
{
    // Set occipied squares
    Ship s;
    auto v = Ship::makeVectorPtr({ make_pair(1,2), make_pair(1,3), make_pair(1,4) });
    s.setOccupiedSquares(move(v));

    EXPECT_EQ(s.getLength(), 3);
    EXPECT_EQ(s.getRange(), 4);
    EXPECT_FALSE(s.isSunk());

    // Check if returned vecotr is correct
    v = s.getOccupiedSquares();
    ASSERT_TRUE(v) << "returned nullptr";
    ASSERT_EQ(v->size(), (size_t)3);
    EXPECT_EQ(v->at(0), make_pair(1,2));
    EXPECT_EQ(v->at(1), make_pair(1,3));
    EXPECT_EQ(v->at(2), make_pair(1,4));

    // Check shots per round
    // When shot twice then in next round is waiting
    EXPECT_TRUE(s.canShoot());
    s.shoot();
    EXPECT_TRUE(s.canShoot());
    s.shoot();
    EXPECT_FALSE(s.canShoot());
    s.nextRound();
    EXPECT_FALSE(s.canShoot());
    s.nextRound();
    EXPECT_TRUE(s.canShoot());
    s.shoot();
    EXPECT_TRUE(s.canShoot());
    s.nextRound();
    EXPECT_TRUE(s.canShoot());

    // Check sinking
    ASSERT_FALSE(s.isSunk());
    s.takeShot();
    ASSERT_FALSE(s.isSunk());
    s.takeShot();
    ASSERT_FALSE(s.isSunk());
    s.takeShot();
    EXPECT_TRUE(s.isSunk());

    // Check if nextRound resets sunk state
    EXPECT_FALSE(s.canShoot());
    s.nextRound();
    EXPECT_TRUE(s.isSunk());
    EXPECT_FALSE(s.canShoot());
}

TEST(ShipTest, reset_single_to_double)
{
    // single
    Ship s;
    auto v = Ship::makeVectorPtr({ make_pair(0,0) });
    s.setOccupiedSquares(move(v));

    // double
    v = Ship::makeVectorPtr({ make_pair(2,3), make_pair(2,4) });
    s.setOccupiedSquares(move(v));

    EXPECT_EQ(s.getLength(), 2);
    EXPECT_EQ(s.getRange(), 3);

    v = s.getOccupiedSquares();
    ASSERT_TRUE(v) << "returned nullptr";
    ASSERT_EQ(v->size(), (size_t)2);
    EXPECT_EQ(v->at(0), make_pair(2,3));
    EXPECT_EQ(v->at(1), make_pair(2,4));
}

TEST(ShipTest, reset_double_to_triple)
{
    // double
    Ship s;
    auto v = Ship::makeVectorPtr({ make_pair(8,9), make_pair(9,9) });
    s.setOccupiedSquares(move(v));

    // triple
    v = Ship::makeVectorPtr({ make_pair(2,8), make_pair(3,8), make_pair(4,8) });
    s.setOccupiedSquares(move(v));

    EXPECT_EQ(s.getLength(), 3);
    EXPECT_EQ(s.getRange(), 4);

    v = s.getOccupiedSquares();
    ASSERT_TRUE(v) << "returned nullptr";
    ASSERT_EQ(v->size(), (size_t)3);
    EXPECT_EQ(v->at(0), make_pair(2,8));
    EXPECT_EQ(v->at(1), make_pair(3,8));
    EXPECT_EQ(v->at(2), make_pair(4,8));
}

TEST(ShipTest, reset_triple_to_single)
{
    // triple
    Ship s;
    auto v = Ship::makeVectorPtr({ make_pair(1,2), make_pair(1,3), make_pair(1,4) });
    s.setOccupiedSquares(move(v));

    // single
    v = Ship::makeVectorPtr({ make_pair(2,3) });
    s.setOccupiedSquares(move(v));

    EXPECT_EQ(s.getLength(), 1);
    EXPECT_EQ(s.getRange(), 2);

    v = s.getOccupiedSquares();
    ASSERT_TRUE(v) << "returned nullptr";
    ASSERT_EQ(v->size(), (size_t)1);
    EXPECT_EQ(v->at(0), make_pair(2,3));
}

TEST(ShipTest, logic_errors)
{
    Ship s;

    EXPECT_THROW(s.getOccupiedSquares(), BattleshipLogicError) << "cannot get squares without setting them first";

    auto v = Ship::makeVectorPtr({ make_pair(0, 0) });
    s.setOccupiedSquares(move(v));

    try { s.shoot(); } catch(...) { }
    EXPECT_FALSE(s.canShoot());
    EXPECT_THROW(s.shoot(), BattleshipLogicError) << "cannot shoot when canShoot() returns false";

    try { s.takeShot(); } catch(...) { }
    EXPECT_TRUE(s.isSunk());
    EXPECT_THROW(s.takeShot(), BattleshipLogicError) << "cannot hit sunk ship";

    v = nullptr;
    EXPECT_THROW(s.setOccupiedSquares(move(v)), BattleshipLogicError) << "passed nullptr as argument";
}

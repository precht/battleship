#include "ShipsGrid.h"
#include "exceptions.h"

#include "gtest/gtest.h"

#include <utility>
#include <vector>
#include <memory>
#include <algorithm>
#include <set>

using namespace battleship;
using std::vector;
using std::pair;
using std::make_pair;
using std::unique_ptr;
using std::make_unique;
using std::move;

TEST(ShipsGridTest, dummy)
{
    (void)ShipsGrid();
}

TEST(ShipsGridTest, initial_state)
{
    ShipsGrid g;
    for (int a = 0; a < 10; ++a)
        for (int b = 0; b < 10; ++b)
            EXPECT_EQ(g.at(make_pair(a,b)), ST_EMPTY) << "squares should be empty";

    // before setting occupied squares all ships has length 0
    for (auto& x : g.getAllShips())
        EXPECT_EQ(x.getLength(), 0);
}

TEST(ShipsGridTest, single_ship_occupied_squares)
{
    ShipsGrid g;
    auto v = Ship::makeVectorPtr({ {0,9} });
    g.setShipLocation(move(v));

    auto t = g.getShip(1).getOccupiedSquares();
    ASSERT_TRUE(t) << "returned nullptr";

    EXPECT_EQ(t->at(0), make_pair(0,9)) << "single-ship has wrong occupied squares";
    EXPECT_EQ(g.at({0,9}), ST_SINGLE) << "single-ship not added to grid table";

    // Check other squares
    for (int a = 0; a < 10; ++a)
        for (int b = 0; b < 10; ++b)
            if (a != 0 || b != 9)
                EXPECT_EQ(g.at({a,b}), ST_EMPTY) << "set wrong ship location";

    // Change ship location and check one again

    v = Ship::makeVectorPtr({ {3,4} });
    g.setShipLocation(move(v));

    t = g.getShip(1).getOccupiedSquares();
    ASSERT_TRUE(t) << "returned nullptr";

    EXPECT_EQ(t->at(0), make_pair(3,4));
    EXPECT_EQ(g.at({3,4}), ST_SINGLE);

    for (int a = 0; a < 10; ++a)
        for (int b = 0; b < 10; ++b)
            if (a != 3 || b != 4)
                EXPECT_EQ(g.at({a,b}), ST_EMPTY) << "set wrong ship location";
}

TEST(ShipsGridTest, double_ship_occupied_squares)
{
    ShipsGrid g;
    auto v = Ship::makeVectorPtr({ {9,0}, {9,1} });
    g.setShipLocation(move(v));

    auto t = g.getShip(2).getOccupiedSquares();
    ASSERT_TRUE(t) << "returned nullptr";

    // Check ship's occupied squares and ship's location on table
    EXPECT_EQ(t->at(0), make_pair(9,0));
    EXPECT_EQ(g.at({9,0}), ST_DOUBLE);

    EXPECT_EQ(t->at(1), make_pair(9,1));
    EXPECT_EQ(g.at({9,1}), ST_DOUBLE);

    // Check other squares
    for (int a = 0; a < 10; ++a)
        for (int b = 0; b < 10; ++b)
            if (a != 9 || (b != 0 && b != 1))
                EXPECT_EQ(g.at({a,b}), ST_EMPTY) << "set wrong ship location";

    // Change ship location and check one again

    v = Ship::makeVectorPtr({ {5,6}, {5,5} });
    g.setShipLocation(move(v));

    t = g.getShip(2).getOccupiedSquares();
    ASSERT_TRUE(t) << "returned nullptr";

    EXPECT_EQ(t->at(0), make_pair(5,5));
    EXPECT_EQ(g.at({5,5}), ST_DOUBLE);

    EXPECT_EQ(t->at(1), make_pair(5,6));
    EXPECT_EQ(g.at({5,6}), ST_DOUBLE);

    for (int a = 0; a < 10; ++a)
        for (int b = 0; b < 10; ++b)
            if (a != 5 || (b != 5 && b != 6))
                EXPECT_EQ(g.at({a,b}), ST_EMPTY) << "set wrong ship location";
}


TEST(ShipsGridTest, triple_ship_occupied_squares)
{
    ShipsGrid g;
    auto v = Ship::makeVectorPtr({ {7,9}, {9,9}, {8,9} });
    g.setShipLocation(move(v));

    auto t = g.getShip(3).getOccupiedSquares();
    ASSERT_TRUE(t) << "returned nullptr";

    // Check ship's occupied squares and ship's location on table
    EXPECT_EQ(t->at(0), make_pair(7,9));
    EXPECT_EQ(g.at({7,9}), ST_TRIPLE);

    EXPECT_EQ(t->at(1), make_pair(8,9));
    EXPECT_EQ(g.at({8,9}), ST_TRIPLE);

    EXPECT_EQ(t->at(2), make_pair(9,9));
    EXPECT_EQ(g.at({9,9}), ST_TRIPLE);

    // Check other squares
    for (int a = 0; a < 10; ++a)
        for (int b = 0; b < 10; ++b)
            if ((a != 7 && a != 8 && a != 9) || b != 9)
                EXPECT_EQ(g.at({a,b}), ST_EMPTY) << "set wrong ship location";

    // Change ship location and check one again

    v = Ship::makeVectorPtr({ {6,3}, {6,4}, {6,2} });
    g.setShipLocation(move(v));

    t = g.getShip(3).getOccupiedSquares();
    ASSERT_TRUE(t) << "returned nullptr";

    // Check ship's occupied squares and ship's location on table
    EXPECT_EQ(t->at(0), make_pair(6,2));
    EXPECT_EQ(g.at({6,2}), ST_TRIPLE);

    EXPECT_EQ(t->at(1), make_pair(6,3));
    EXPECT_EQ(g.at({6,3}), ST_TRIPLE);

    EXPECT_EQ(t->at(2), make_pair(6,4));
    EXPECT_EQ(g.at({6,4}), ST_TRIPLE);

    // Check other squares
    for (int a = 0; a < 10; ++a)
        for (int b = 0; b < 10; ++b)
            if (a != 6 || (b != 2 && b != 3 && b != 4))
                EXPECT_EQ(g.at({a,b}), ST_EMPTY) << "set wrong ship location";
}

TEST(ShipsGridTest, get_all_ships)
{
    ShipsGrid g;
    auto v = Ship::makeVectorPtr({ {0,9} });
    g.setShipLocation(move(v));

    v = Ship::makeVectorPtr({ {9,0}, {9,1} });
    g.setShipLocation(move(v));

    v = Ship::makeVectorPtr({ {7,9}, {8,9}, {9,9} });
    g.setShipLocation(move(v));

    auto& t = g.getAllShips();
    EXPECT_EQ(t.size(), (size_t)3);
    for(auto& x : t)
        EXPECT_GT(x.getLength(), 0);
}

TEST(ShipsGridTest, incorrect_occupied_squeres)
{
    ShipsGrid g;
    auto v = Ship::makeVectorPtr({ {-1,0} });
    EXPECT_THROW(g.setShipLocation(move(v)), InvalidCoordinateError) << "negative row.";

    v = Ship::makeVectorPtr({ {0,-2} });
    EXPECT_THROW(g.setShipLocation(move(v)), InvalidCoordinateError) << "negative column.";

    v = Ship::makeVectorPtr({ {10,9} });
    EXPECT_THROW(g.setShipLocation(move(v)), InvalidCoordinateError) << "row too big.";

    v = Ship::makeVectorPtr({ {9,11} });
    EXPECT_THROW(g.setShipLocation(move(v)), InvalidCoordinateError) << "column too big.";

    v = Ship::makeVectorPtr({ {0, 1}, {0, 1} });
    EXPECT_THROW(g.setShipLocation(move(v)), InvalidShipLocationError) << "the same square twice.";

    v = Ship::makeVectorPtr({ {1, 1}, {1, 3} });
    EXPECT_THROW(g.setShipLocation(move(v)), InvalidShipLocationError) << "squeres not connected.";

    v = Ship::makeVectorPtr({ {1, 1}, {2, 2} });
    EXPECT_THROW(g.setShipLocation(move(v)), InvalidShipLocationError) << "skew squeres.";

    v = Ship::makeVectorPtr({ {1, 1}, {1, 2}, {2, 2} });
    EXPECT_THROW(g.setShipLocation(move(v)), InvalidShipLocationError) << "L shaped squeres.";

    v = Ship::makeVectorPtr({ {1, 1}, {1, 2}, {2, 3} });
    EXPECT_THROW(g.setShipLocation(move(v)), InvalidShipLocationError) << "L shaped squeres.";

    v = Ship::makeVectorPtr({});
    EXPECT_THROW(g.setShipLocation(move(v)), BattleshipRuntimeError) << "ships size is 0.";

    v = Ship::makeVectorPtr({ {1,1}, {1,2}, {1,3}, {1,4} });
    EXPECT_THROW(g.setShipLocation(move(v)), BattleshipRuntimeError) << "ships size is too big.";

    // check that no squares were set
    EXPECT_EQ(g.getShip(1).getLength(), 0);
    EXPECT_EQ(g.getShip(2).getLength(), 0);
    EXPECT_EQ(g.getShip(3).getLength(), 0);
    for (int a = 0; a < 10; ++a)
        for (int b = 0; b < 10; ++b)
                EXPECT_EQ(g.at({a,b}), ST_EMPTY) << "set ship location when throwing error";
}

TEST(ShipsGridTest, incorrect_location_ships_to_close)
{
    ShipsGrid g;
    auto v = Ship::makeVectorPtr({ {3,3}, {4,3} });
    g.setShipLocation(move(v));

    // Ships too close

    v = Ship::makeVectorPtr({ {2,2} });
    EXPECT_THROW(g.setShipLocation(move(v)), InvalidShipLocationError);

    v = Ship::makeVectorPtr({ {3,2} });
    EXPECT_THROW(g.setShipLocation(move(v)), InvalidShipLocationError);

    v = Ship::makeVectorPtr({ {5,2} });
    EXPECT_THROW(g.setShipLocation(move(v)), InvalidShipLocationError);

    v = Ship::makeVectorPtr({ {5,3} });
    EXPECT_THROW(g.setShipLocation(move(v)), InvalidShipLocationError);

    v = Ship::makeVectorPtr({ {5,4} });
    EXPECT_THROW(g.setShipLocation(move(v)), InvalidShipLocationError);

    v = Ship::makeVectorPtr({ {4,4} });
    EXPECT_THROW(g.setShipLocation(move(v)), InvalidShipLocationError);

    v = Ship::makeVectorPtr({ {2,4} });
    EXPECT_THROW(g.setShipLocation(move(v)), InvalidShipLocationError);

    v = Ship::makeVectorPtr({ {2,3} });
    EXPECT_THROW(g.setShipLocation(move(v)), InvalidShipLocationError);

    v = Ship::makeVectorPtr({ {5,6}, {5,4}, {5,5} });
    EXPECT_THROW(g.setShipLocation(move(v)), InvalidShipLocationError) << "triple ship too close";

    // check that no squares were set
    EXPECT_EQ(g.getShip(1).getLength(), 0);
    EXPECT_EQ(g.getShip(3).getLength(), 0);
    for (int a = 0; a < 10; ++a)
        for (int b = 0; b < 10; ++b)
            if ((a != 3 && a != 4) || b != 3)
                EXPECT_EQ(g.at({a,b}), ST_EMPTY) << "set ship location when throwing error";
}

TEST(ShipsGridTest, take_shot)
{
    ShipsGrid g;

    auto v = Ship::makeVectorPtr({ {2,2} });
    g.setShipLocation(move(v));
    v = Ship::makeVectorPtr({ {6,3}, {6,4} });
    g.setShipLocation(move(v));
    v = Ship::makeVectorPtr({ {3,8}, {4,8}, {5,8} });
    g.setShipLocation(move(v));

    // shoot all ships

    // single
    EXPECT_EQ(g.takeShot({2,2}), SR_SUNK);
    EXPECT_EQ(g.at({2,2}), ST_SUNK);

    // double
    EXPECT_EQ(g.takeShot({6,3}), SR_HIT);
    EXPECT_EQ(g.at({6,3}), ST_HIT);

    EXPECT_EQ(g.takeShot({6,4}), SR_SUNK);
    EXPECT_EQ(g.at({6,3}), ST_SUNK);
    EXPECT_EQ(g.at({6,4}), ST_SUNK);

    // triple
    EXPECT_EQ(g.takeShot({5,8}), SR_HIT);
    EXPECT_EQ(g.at({5,8}), ST_HIT);

    EXPECT_EQ(g.takeShot({3,8}), SR_HIT);
    EXPECT_EQ(g.at({3,8}), ST_HIT);
    EXPECT_EQ(g.at({5,8}), ST_HIT);

    EXPECT_EQ(g.takeShot({4,8}), SR_SUNK);
    EXPECT_EQ(g.at({3,8}), ST_SUNK);
    EXPECT_EQ(g.at({4,8}), ST_SUNK);
    EXPECT_EQ(g.at({5,8}), ST_SUNK);

    // shoot other squares
    std::set<pair<int,int>> d({ {2,2}, {6,3}, {6,4}, {3,8}, {4,8}, {5,8} });
    for (int a = 0; a < 10; a++)
        for (int b = 0; b < 10; b++)
            if (d.find({a,b}) == d.end())
            {
                EXPECT_EQ(g.at({a,b}), ST_EMPTY);
                EXPECT_EQ(g.takeShot({a,b}), SR_MISS);
                EXPECT_EQ(g.at({a,b}), ST_MISS);
            }
}

TEST(ShipsGridTest, incorrect_take_shot)
{
    ShipsGrid g;

    // out of allowed range

    EXPECT_THROW(g.takeShot({-1,0}), InvalidCoordinateError);
    EXPECT_THROW(g.takeShot({0,-2}), InvalidCoordinateError);
    EXPECT_THROW(g.takeShot({10,0}), InvalidCoordinateError);
    EXPECT_THROW(g.takeShot({0,11}), InvalidCoordinateError);

    // check if nothing was set
    for (int a = 0; a < 10; a++)
        for (int b = 0; b < 10; b++)
            EXPECT_EQ(g.at({a,b}), ST_EMPTY);
}








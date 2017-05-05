#include "Grid.h"
#include "Ship.h"
#include "exceptions.h"

#include "gtest/gtest.h"

#include <vector>
#include <memory>
#include <utility>
#include <set>
#include <unordered_set>

using namespace battleship;
using std::vector;
using std::unordered_set;
using std::pair;
using std::make_pair;
using std::unique_ptr;
using std::make_unique;
using std::move;
using std::set;


TEST(GridTest, dummy)
{
    (void)Grid();
}

TEST(GridTest, hash)
{
    set<size_t> s;
    battleship::SquareHash hash;
    // Check if hashes for (0,0)...(9,9) are unique
    for (int a = 0; a < 10; ++a)
        for (int b = 0; b < 10; ++b)
            // insert return pair which second value is false when element already existed
            EXPECT_TRUE(s.insert(hash(make_pair(a,b))).second) << "hashes are not unique";
}

TEST(GridTest, unordered_set)
{
    unordered_set<pair<int,int>, battleship::SquareHash> s;

    // Insert all possible elements into set
    for (int a = 0; a < 10; ++a)
        for (int b = 0; b < 10; ++b)
            s.insert(make_pair(a,b));

    // Check if all elements are in set
    for (int a = 0; a < 10; ++a)
        for (int b = 0; b < 10; ++b)
            EXPECT_NE(s.find(make_pair(a,b)), s.end()) << "missing an element";
}

TEST(GridTest, initial_state)
{
    Grid g;
    for (int a = 0; a < 10; ++a)
        for (int b = 0; b < 10; ++b)
            EXPECT_EQ(g.at(make_pair(a,b)), ST_EMPTY) << "initial squares should be empty";
}

TEST(GridTest, square_update)
{
    Grid g;
    g.update({ 0, 0 }, SR_HIT);
    EXPECT_EQ(g.at(make_pair(0,0)), ST_HIT) << "hit update not working";

    g.update({ 5, 5 }, SR_MISS);
    EXPECT_EQ(g.at(make_pair(5,5)), ST_MISS) << "missed update not working";

    g.update({ 0, 9 }, SR_SUNK);
    EXPECT_EQ(g.at(make_pair(0,9)), ST_SUNK) << "single-funnel-ship sinking not working";

    g.update({ 9, 0 }, SR_HIT);
    EXPECT_EQ(g.at(make_pair(9,0)), ST_HIT);
    g.update({ 9, 1 }, SR_SUNK);
    EXPECT_EQ(g.at(make_pair(9,0)), ST_SUNK) << "double-funnel-ship sinking not working";
    EXPECT_EQ(g.at(make_pair(9,1)), ST_SUNK) << "double-funnel-ship sinking not working";

    g.update({ 7, 9 }, SR_HIT);
    EXPECT_EQ(g.at(make_pair(7,9)), ST_HIT);
    g.update({ 9, 9 }, SR_HIT);
    EXPECT_EQ(g.at(make_pair(9,9)), ST_HIT);
    g.update({ 8, 9 }, SR_SUNK);
    EXPECT_EQ(g.at(make_pair(7,9)), ST_SUNK) << "triple-funnel-ship sinking not working";
    EXPECT_EQ(g.at(make_pair(8,9)), ST_SUNK) << "triple-funnel-ship sinking not working";
    EXPECT_EQ(g.at(make_pair(9,9)), ST_SUNK) << "triple-funnel-ship sinking not working";
}

TEST(GridTest, available_range_not_exit_borders)
{
    vector<pair<int,int>> tested_sq ({ {0,0}, {0,1}, {1,0}, {1,1}, {8,0}, {9,0}, {8,1}, {9,1},
                                       {8,8}, {9,8}, {8,9}, {9,9}, {0,8}, {0,9}, {1,8}, {1,9} });

    for (auto x : tested_sq)
    {
        Grid g;
        auto v = make_unique<vector<pair<int,int>>>();
        v->push_back(x);
        Ship s;
        s.setOccupiedSquares(move(v));
        auto r = g.getAvailableRange(s);
        ASSERT_TRUE(r) << "return nullptr";
        for (auto p : *r)
        {
            EXPECT_GE(p.first, 0) << "negative value";
            EXPECT_GE(p.second, 0) << "negative value";
            EXPECT_LE(p.first, 9) << "value over 9";
            EXPECT_LE(p.second, 9) << "value over 9";
        }
    }
}

TEST(GridTest, single_ship_available_range)
{
    Grid g;
    Ship s;

    auto v = Ship::makeVectorPtr({ make_pair(3,4) });
    s.setOccupiedSquares(move(v));

    unordered_set<pair<int,int>, SquareHash> r;
    for (int a = 1; a <= 5; a++)
        for (int b = 2; b <= 6; b++)
            r.insert(make_pair(a,b));

    ASSERT_TRUE(g.getAvailableRange(s)) << "cannot return nullptr";
    EXPECT_EQ(*g.getAvailableRange(s), r) << "wrong initial (full) range";

    auto d = make_pair(1,2);
    r.erase(d);
    g.update(d, SR_HIT);
    ASSERT_TRUE(g.getAvailableRange(s)) << "cannot return nullptr";
    EXPECT_EQ(*g.getAvailableRange(s), r) << "wrong range after update";

    d = make_pair(5,4);
    r.erase(d);
    g.update(d, SR_MISS);
    ASSERT_TRUE(g.getAvailableRange(s)) << "cannot return nullptr";
    EXPECT_EQ(*g.getAvailableRange(s), r) << "wrong range after update";

    d = make_pair(3,4);
    r.erase(d);
    g.update(d, SR_SUNK);
    ASSERT_TRUE(g.getAvailableRange(s)) << "cannot return nullptr";
    EXPECT_EQ(*g.getAvailableRange(s), r) << "wrong range after update";

    d = make_pair(3,7);
    r.erase(d);
    g.update(d, SR_HIT);
    ASSERT_TRUE(g.getAvailableRange(s)) << "cannot return nullptr";
    EXPECT_EQ(*g.getAvailableRange(s), r) << "wrong range after update";
}

TEST(GridTest, double_ship_available_range)
{
    Grid g;
    Ship s;

    auto v = Ship::makeVectorPtr({ make_pair(3,4), make_pair(4,4) });
    s.setOccupiedSquares(move(v));

    unordered_set<pair<int, int>, SquareHash> r;
    for (int a = 0; a <= 7; a++)
        for (int b = 1; b <= 7; b++)
            r.insert(make_pair(a,b));

    ASSERT_TRUE(g.getAvailableRange(s)) << "cannot return nullptr";
    EXPECT_EQ(*g.getAvailableRange(s), r) << "wrong initial (full) range";

    auto d = make_pair(2,6);
    r.erase(d);
    g.update(d, SR_HIT);
    ASSERT_TRUE(g.getAvailableRange(s)) << "cannot return nullptr";
    EXPECT_EQ(*g.getAvailableRange(s), r) << "wrong range after update";

    d = make_pair(7,7);
    r.erase(d);
    g.update(d, SR_MISS);
    ASSERT_TRUE(g.getAvailableRange(s)) << "cannot return nullptr";
    EXPECT_EQ(*g.getAvailableRange(s), r) << "wrong range after update";

    d = make_pair(4,4);
    r.erase(d);
    g.update(d, SR_SUNK);
    ASSERT_TRUE(g.getAvailableRange(s)) << "cannot return nullptr";
    EXPECT_EQ(*g.getAvailableRange(s), r) << "wrong range after update";
}

TEST(GridTest, triple_ship_available_test)
{
    Grid g;
    Ship s;

    auto v = Ship::makeVectorPtr({ make_pair(5,3), make_pair(5,4), make_pair(5,5) });
    s.setOccupiedSquares(move(v));

    unordered_set<pair<int, int>, SquareHash> r;
    for (int a = 1; a <= 9; a++)
        for (int b = 0; b <= 9; b++)
            r.insert(make_pair(a,b));

    // "choose" all squares from range
    for (int a = 9; a >= 1; a--)
    {
        for (int b = 0; b <= 9; b++)
        {
            auto d = make_pair(a,b);
            r.erase(d);
            g.update(d, SR_MISS);
            ASSERT_TRUE(g.getAvailableRange(s)) << "cannot return nullptr";
            EXPECT_EQ(*g.getAvailableRange(s), r) << "wrong range after update";
        }
    }

    EXPECT_TRUE(g.getAvailableRange(s)->empty()) << "should be empty";
}

TEST(GridTest, errors_incorrect_ship_state)
{
    Ship s;
    Grid g;

    EXPECT_THROW(g.getAvailableRange(s), BattleshipLogicError) << "ship without set position";

    // cannot update twice the same square
    g.update(make_pair(5,5), SR_MISS);
    EXPECT_THROW(g.update(make_pair(5,5), SR_HIT), BattleshipRuntimeError);
    EXPECT_THROW(g.update(make_pair(5,5), SR_SUNK), BattleshipRuntimeError);
    EXPECT_THROW(g.update(make_pair(5,5), SR_MISS), BattleshipRuntimeError);

    // cannot get range for sunk ship

    // single
    Ship s1;
    auto v = Ship::makeVectorPtr({ make_pair(3,4) });
    s1.setOccupiedSquares(move(v));
    s1.takeShot();
    EXPECT_THROW(g.getAvailableRange(s1), BattleshipLogicError);

    // double
    Ship s2;
    v = Ship::makeVectorPtr({ make_pair(3,4), make_pair(4,4) });
    s2.setOccupiedSquares(move(v));
    s2.takeShot();
    s2.takeShot();
    EXPECT_THROW(g.getAvailableRange(s2), BattleshipLogicError);

    // triple
    Ship s3;
    v = Ship::makeVectorPtr({ make_pair(5,3), make_pair(5,4), make_pair(5,5) });
    s3.setOccupiedSquares(move(v));
    s3.takeShot();
    s3.takeShot();
    s3.takeShot();
    EXPECT_THROW(g.getAvailableRange(s3), BattleshipLogicError);
}

TEST(GridTest, errors_square_out_of_range)
{
    Grid g;

    // square position out of allowed range
    auto p = make_pair(-1,0);
    EXPECT_THROW(g.at(p), InvalidCoordinateError);
    EXPECT_THROW(g.update(p, SR_MISS), InvalidCoordinateError) ;

    p = make_pair(0,-1);
    EXPECT_THROW(g.at(p), InvalidCoordinateError);
    EXPECT_THROW(g.update(p, SR_HIT), InvalidCoordinateError);

    p = make_pair(10,0);
    EXPECT_THROW(g.at(p), InvalidCoordinateError);
    EXPECT_THROW(g.update(p, SR_SUNK), InvalidCoordinateError);

    p = make_pair(0,10);
    EXPECT_THROW(g.at(p), InvalidCoordinateError);
    EXPECT_THROW(g.update(p, SR_SUNK), InvalidCoordinateError);
}

TEST(GridTest, errors_wrong_input_from_other_player)
{
    Grid g;

    g.update({7, 7}, SR_HIT);
    g.update({7, 8}, SR_HIT);
    g.update({7, 9}, SR_SUNK);
    EXPECT_THROW(g.update({6,7}, SR_SUNK),  BattleshipRuntimeError) << "ships too close or ship too long";

    g.update({3, 9}, SR_SUNK);
    EXPECT_THROW(g.update({2, 7}, SR_SUNK), BattleshipRuntimeError) << "cannot sunk ship with the same length twice";

    g.update({0, 6}, SR_HIT);
    g.update({0, 8}, SR_HIT);
    EXPECT_THROW(g.update({0,7}, SR_SUNK), BattleshipRuntimeError) << "cannot sunk ship with the same length twice";
}

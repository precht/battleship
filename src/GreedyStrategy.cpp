#include "GreedyStrategy.h"

#include <random>
#include <algorithm>

using std::unique_ptr;
using std::reference_wrapper;
using std::vector;
using std::pair;
using std::unordered_set;


int battleship::GreedyStrategy::chooseShip(unique_ptr<vector<int>> ships_lengths)
{
    if (ships_lengths->empty())
        throw BattleshipRuntimeError("GreedyStrategy::chooseShip: no ship to choose.");
    std::sort(ships_lengths->begin(), ships_lengths->end());
    return *ships_lengths->rbegin();
}

pair<int,int> battleship::GreedyStrategy::chooseSquare(unique_ptr<unordered_set<pair<int,int>, SquareHash>> squares)
{
    if (squares->empty())
        throw BattleshipRuntimeError("GreedyStrategy::chooseSquare: no square to choose.");
    unsigned position = std::random_device{}() % squares->size();
    auto it = squares->begin();
    while(position--) ++it;
    return *it;
}

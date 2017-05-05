#include "RandomStrategy.h"

#include <random>

using std::unique_ptr;
using std::reference_wrapper;
using std::vector;
using std::pair;
using std::unordered_set;


int battleship::RandomStrategy::chooseShip(unique_ptr<vector<int>> ships_lengths)
{
    if (ships_lengths->empty())
                throw BattleshipRuntimeError("RandomStrategy::chooseShip: no ship to choose.");
    return ships_lengths->at(std::random_device{}() % ships_lengths->size());
}

pair<int,int> battleship::RandomStrategy::chooseSquare(unique_ptr<unordered_set<pair<int,int>, SquareHash>> squares)
{
    if (squares->empty())
        throw BattleshipRuntimeError("RandomStrategy::chooseSquare: no square to choose.");
    unsigned position = std::random_device{}() % squares->size();
    auto it = squares->begin();
    while(position--) ++it;
    return *it;
}

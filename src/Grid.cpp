#include "Grid.h"
#include "exceptions.h"

#include <stack>
#include <iostream>

using std::unordered_set;
using std::unique_ptr;
using std::pair;
using std::make_unique;
using std::stack;


std::size_t battleship::SquareHash::operator()(const pair<int, int>& pii) const
{
    // multiply the first value by 16. it is now greater than 9
    // and can be sum with the second value always creating unique hash
    return ((unsigned)pii.first << 4) + (unsigned)pii.second;
}

battleship::Grid::Grid()
{
    for (int a = 0; a < 10; ++a)
        for (int b = 0; b < 10; ++b)
            table_[a][b] = SquareType::ST_EMPTY;
}

unique_ptr<unordered_set<pair<int,int>, battleship::SquareHash>> battleship::Grid::getAvailableRange(
        const Ship& ship) const
{
    if (ship.isSunk())
        throw BattleshipLogicError("Grid::getAvailableRange: the ship is sunk");
    if (ship.getLength() == 0)
        throw BattleshipLogicError("Grid::getAvailableRange: the ship is not yet placed on the grid");

    auto r = make_unique<unordered_set<pair<int, int>, battleship::SquareHash>>();
    auto s = ship.getOccupiedSquares();
    int range = ship.getRange();

    for (int i = 0; i < ship.getLength(); i++)
    {
        const pair<int, int> p = (*s)[i];

        for (int a = -range; a <= range; ++a)
            for (int b = -range; b <= range; ++b)
            {
                if (p.first + a <= 9 && p.first + a >= 0 && p.second + b <= 9 && p.second + b >= 0)
                    if (table_[p.first + a][p.second + b] == ST_EMPTY) r->insert({ p.first + a, p.second + b });
            }
    }

    return r;
}

battleship::SquareType battleship::Grid::at(pair<int,int> square) const
{
    if (square.second > 9 || square.second < 0 || square.first > 9 || square.first < 0) 
        throw InvalidCoordinateError("Grid::at: coordinates out of allowed range.");

    return table_[square.first][square.second];
}

void battleship::Grid::update(std::pair<int,int> square, ShotResult result)
{
    if (at(square) != ST_EMPTY)
        throw BattleshipRuntimeError("Grid::update: you cannot update the same square twice.");

    // missed
    if (result == SR_MISS)
    {
        table_[square.first][square.second] = ST_MISS;
        return;
    }

    stack<pair<int, int>> st;
    unordered_set<pair<int,int>, battleship::SquareHash> visited;

    st.push(square);
    visited.insert(square);
    bool sunk_ship_too_close = false;

    while (!st.empty())
    {
        auto p = st.top();
        st.pop();

        for (int a = -1; a <= 1; a++)
        {
            for (int b = -1; b <= 1; b++)
            {
                const int fst = p.first + a;
                const int snd = p.second + b;
                if (fst < 0 || snd < 0 || fst >= 10 || snd >= 10 || visited.count({ fst, snd }))
                    continue;
                if (table_[fst][snd] == ST_SUNK)
                {
                    sunk_ship_too_close = true;
                    break;
                }
                if (table_[fst][snd] == ST_HIT)
                {
                    st.push({ fst, snd });
                    visited.insert({ fst, snd });
                }
            }
        }
    }

    if (sunk_ship_too_close || visited.size() > Ship::MAX_LENGTH)
        throw BattleshipRuntimeError("Grid::update: obtained ships too close or too long ship.");

    // hit
    if (result == SR_HIT)
    {
        table_[square.first][square.second] = ST_HIT;
        return;
    }

    // sunk
    if (sunk_ships_.count(visited.size()))
        throw BattleshipRuntimeError("Grid::update: already sunk ship with the same length.");

    sunk_ships_.insert(visited.size());
    for (auto p : visited)
        table_[p.first][p.second] = ST_SUNK;
}

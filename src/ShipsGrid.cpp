#include "ShipsGrid.h"

#include <algorithm>
#include <cassert>

using std::unique_ptr;
using std::vector;
using std::array;
using std::pair;

const battleship::Ship& battleship::ShipsGrid::getShip(int length) const
{
    if (length <= 0 || length > Ship::MAX_LENGTH)
        throw BattleshipRuntimeError("ShipsGrid::takeShot: wrong length of the ship.");
    return ships_[length - 1];
}

const array<battleship::Ship,3>& battleship::ShipsGrid::getAllShips() const
{
    return ships_;
}

battleship::ShotResult battleship::ShipsGrid::takeShot(pair<int,int> square)
{
    if (square.first < 0 || square.second < 0 || square.first >= 10 || square.second >= 10)
        throw InvalidCoordinateError("ShipsGrid::takeShot: square out of allowed range.");

    auto st = table_[square.first][square.second];

    if (st == ST_HIT || st == ST_MISS || st == ST_SUNK)
        throw BattleshipRuntimeError("ShipsGrid::takeShot: already shot at this position.");

    if (st == ST_EMPTY)
    {
        table_[square.first][square.second] = ST_MISS;
        return SR_MISS;
    }

    Ship* s = nullptr;
    if (st == ST_SINGLE) s = &ships_[0];
    else if (st == ST_DOUBLE) s = &ships_[1];
    else if (st == ST_TRIPLE) s = &ships_[2];
    else assert(false); // s is nullptr;

    s->takeShot();
    table_[square.first][square.second] = ST_HIT;

    if (s->isSunk())
    {
        auto t = *s->getOccupiedSquares();
        for (auto p : t)
            table_[p.first][p.second] = ST_SUNK;
    }

    return s->isSunk() ? SR_SUNK : SR_HIT;
}

void battleship::ShipsGrid::setShipLocation(unique_ptr<vector<pair<int,int>>> occupied_squares)
{
    // check if passed squares are correct
    if (!occupied_squares)
        throw BattleshipLogicError("ShipsGrid::setShipLocation:: passed nullptr as argument.");

    const auto length = occupied_squares->size();

    if (length <= 0 || length > Ship::MAX_LENGTH)
        throw BattleshipRuntimeError("ShipsGrid::setShipLocation:: wrong ship length.");

    // sort input
    std::sort(occupied_squares->begin(), occupied_squares->end(), [](auto& l, auto& r) {
        return (l.first == r.first) ? (l.second < r.second) : (l.first < r.first);
    });

    // check range
    if (occupied_squares->front().first < 0 || occupied_squares->front().second < 0
            || occupied_squares->back().first >= 10 || occupied_squares->back().second >= 10)
        throw InvalidCoordinateError("ShipsGrid::setShipLocation:: square coordinates out of allowed range.");

    // check if are connected and if shape is correct
    auto fst = occupied_squares->back().first - occupied_squares->front().first;
    auto snd = occupied_squares->back().second - occupied_squares->front().second;
    if ( ((size_t)fst + 1 != length || snd != 0) && (fst != 0 || (size_t)snd + 1 != length) )
        throw InvalidShipLocationError("ShipsGrid::setShipLocation: wrong ship's location shape.");

    SquareType st;
    if (length == 1) st = ST_SINGLE;
    else if (length == 2) st = ST_DOUBLE;
    else if (length == 3) st = ST_TRIPLE;
    else assert(false); // unsuported length of ship

    // check if not too close ot another ship
    for (auto p : *occupied_squares)
        for (int a = -1; a <= 1; a++)
            for (int b = -1; b <= 1; b++)
            {
                int x = p.first + a;
                int y = p.second + b;
                // make sure x and y don't exceed table_ range
                x = (x < 0) ? 0 : (x >= 10 ? 9 : x);
                y = (y < 0) ? 0 : (y >= 10 ? 9 : y);

                if (table_[x][y] != ST_EMPTY && table_[x][y] != st)
                    throw InvalidShipLocationError("ShipsGrid::setShipLocation: Ship too close to another one.");
            }

    // clear previous ship's position from table_ if there exists
    if (ships_[length - 1].getLength() > 0)
    {
        auto v = ships_[length - 1].getOccupiedSquares();
        for (auto x : *v) table_[x.first][x.second] = ST_EMPTY;
    }

    // set position in table_
    for (auto p : *occupied_squares)
        table_[p.first][p.second] = st;

    ships_[length - 1].setOccupiedSquares(move(occupied_squares));
}

void battleship::ShipsGrid::shoot(int ship_length)
{
    if (ship_length < 1 || ship_length > 3)
        throw BattleshipRuntimeError("ShipsGrid::shoot: ship length out of range.");

    for (auto& s : ships_)
        if (s.getLength() != ship_length)
            s.pause();

    ships_[ship_length - 1].shoot();
}

void battleship::ShipsGrid::nextRound()
{
    for (auto& x : ships_)
        x.nextRound();
}

void battleship::ShipsGrid::pauseShips(const std::vector<int>& ships_lengths)
{
    if (ships_lengths.size() > ships_.size())
        throw BattleshipRuntimeError("ShipsGrid::pauseShips: wrong number of ships.");

    for (auto x : ships_lengths)
        if (x <= 0 || x > Ship::MAX_LENGTH)
            throw BattleshipRuntimeError("ShipsGrid::pauseShips: wrong ship length.");

    for (auto x : ships_lengths)
        ships_[x - 1].pause();
}

#ifndef GRID_H_
#define GRID_H_

#include "Ship.h"

#include <utility>
#include <vector>
#include <memory>
#include <array>
#include <unordered_set>
#include <set>

namespace battleship
{

    enum ShotResult
    {
        SR_HIT,
        SR_SUNK,
        SR_MISS
    };

    enum SquareType
    {
        ST_SINGLE = '1',
        ST_DOUBLE = '2',
        ST_TRIPLE = '3',
        ST_EMPTY = ' ',
        ST_MISS = '-',
        ST_SUNK = 'x',
        ST_HIT = '+'
    };

    struct SquareHash
    {
        std::size_t operator()(const std::pair<int, int>& pii) const;
    };

    class Grid
    {
    public:
        static const int SIZE = 10;

        Grid();
        virtual ~Grid() = default;

        // get all squares that ship passed as argument
        // ship's location is stored in it
        // available range should be read from table_
        std::unique_ptr<std::unordered_set<std::pair<int, int>, SquareHash>> getAvailableRange(
                const Ship& ship) const;

        // get square value from table_
        SquareType at(std::pair<int, int> square) const;

        // change value in table_ at specified position to result
        // each square can be updated once
        void update(std::pair<int, int> square, ShotResult result);

    protected:
        std::array<std::array<SquareType, 10>, 10> table_;
        std::set<int> sunk_ships_;
    };

}

#endif // !GRID_H_

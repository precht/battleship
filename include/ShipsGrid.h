#ifndef SHIPS_GRID_H_
#define SHIPS_GRID_H_

#include "Ship.h"
#include "Grid.h"
#include "exceptions.h"

#include <utility>
#include <vector>
#include <memory>
#include <array>

namespace battleship
{

    class ShipsGrid : public Grid
    {
    public:
        ShipsGrid() = default;
        ~ShipsGrid() override = default;

        const Ship& getShip(int length) const;
        const std::array<Ship, 3>& getAllShips() const;

        // If there was no a ship at this square mark it as ST_MISS and return SR_MISSED
        // If there was a ship hit it and change square type. Return SR_HIT or SR_SUNK depending on ship status
        // If ship was hit square type should be ST_HIT, but if ship was sunk all ship's squares should be ST_SUNK
        ShotResult takeShot(std::pair<int, int> square);

        // do two thinds: set ship location in table_ and pass occupied_squares to ship using it's
        // setOccupiedSquares() method. ship size is stored as vector size.
        void setShipLocation(std::unique_ptr<std::vector<std::pair<int,int>>> occupied_squares);

        // let ship with speciifed length perform a shot
        void shoot(int ship_length);

        void nextRound();

        void pauseShips(const std::vector<int>& ships_lengths);

    private:
        // index is the ship's length - 1
        std::array<Ship, 3> ships_;

    };

}

#endif // !SHIPS_GRID_H_

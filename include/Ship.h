#ifndef SHIP_H_
#define SHIP_H_

#include <utility>
#include <vector>
#include <memory>
#include <array>

namespace battleship
{

    class Ship
    {
    public:
        // Hardcoded properties depending on ship's length
        static const int MAX_LENGTH = 3;
        static const int PAUSING_AFTER_SHOTS = 2;
        // Indexes are ship lengths: 0, 1, 2, 3
        static const std::array<int, 4> RANGES;
        static const std::array<int, 4> MAX_SHOTS;

        // this function just returns ptr to vector initialized with list
        static std::unique_ptr<std::vector<std::pair<int,int>>> makeVectorPtr(
                std::initializer_list<std::pair<int,int>>&& init_list);

        Ship() = default;
        ~Ship() = default;

        int getRange() const;
        int getLength() const;
        int getHits() const;
        bool canShoot() const;
        bool isSunk() const;
        bool isPausing() const;

        // this method return squares order increasingly be first then by second value
        std::unique_ptr<std::vector<std::pair<int, int>>> getOccupiedSquares() const;

        // inform ship that next round has started
        // shots counter should be set to zero
        // if is_pausing is true it should be set to false
        // if is pausing is false then its new value depends on the number of shots in the ending round
        void nextRound();

        // increase shots number in actual round by 1
        void shoot();

        // increase hits number, if total number of taken hits equals to ship size then the ship should be sunk
        void takeShot();

        // pause this ship until the next round
        void pause();

        // remembers where ship was located. squares should be ordered increasingly by first then by second value
        // note that occupied_suqares should be sorted in ShipGrid::setShipLocation
        void setOccupiedSquares(std::unique_ptr<const std::vector<std::pair<int, int>>> occupied_squares);

    private:
        std::unique_ptr<const std::vector<std::pair<int, int>>> occupied_squares_;
        mutable int shots_counter_ = 0;
        int hits_counter_ = 0;
        bool is_pausing_ = false;
    };
}

#endif // !SHIP_H_

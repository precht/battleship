#ifndef PLAYER_H_
#define PLAYER_H_

#include "Ship.h"
#include "Grid.h"
#include "ShipsGrid.h"

#include <utility>
#include <vector>

namespace battleship
{

    class Player
    {
    public:
        Player() = default;
        virtual ~Player() = default;

        bool canShoot() const;
        bool mayShootNextRounds() const;
        int getHits() const;
        const ShipsGrid& getPrimaryGird() const;
        const Grid& getSecondaryGrid() const;

        // inform player that next round has started
        void nextRound();

        // take opponents shot and return it's result
        ShotResult takeShot(std::pair<int,int> square);

        // update information about player's previous shot
        void update(std::pair<int,int> square, ShotResult result);

        // set up ships using passed values
        void setUpShips(const std::vector<int>& single_args,
                        const std::vector<int>& double_args,
                        const std::vector<int>& triple_args);

        void pauseShips(const std::vector<int>& ships_lengths);

        virtual void setUpShips() = 0;
        virtual std::pair<int,int> shoot() = 0;

    protected:
        // Primary grid stres ships and its locations and remembers opponents shots
        ShipsGrid primary_grid_;
        // Secondary grid stores player's shots and remember opponent's ships locations
        Grid secondary_grid_;
    };

}

#endif // !PLAYER_H_

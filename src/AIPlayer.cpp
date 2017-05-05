#include "AIPlayer.h"
#include "Grid.h"
#include "exceptions.h"

#include <random>
#include <cassert>

using std::unique_ptr;
using std::vector;
using std::array;
using std::pair;
using std::make_unique;
using std::move;

battleship::AIPlayer::AIPlayer(std::unique_ptr<battleship::ShootStrategy> strategy_ptr)
    : Player()
    , strategy_ptr_(move(strategy_ptr))
{ }

void battleship::AIPlayer::setUpShips()
{
    enum Direction { UP, DOWN, LEFT, RIGHT };
    std::random_device rd;

    // triple
    bool success = false;
    int attempt = 0;
    int fst;
    int snd;
    while (!success && attempt++ < MAX_ATTEMPTS)
    {
        fst = rd() % Grid::SIZE;
        snd = rd() % Grid::SIZE;
        auto v = Ship::makeVectorPtr({ {fst, snd} });

        switch((Direction)(rd() % 4))
        {
        case UP:
            v->push_back( {fst, snd - 1} );
            v->push_back( {fst, snd - 2} );
            break;
        case DOWN:
            v->push_back( {fst, snd + 1} );
            v->push_back( {fst, snd + 2} );
            break;
        case LEFT:
            v->push_back( {fst - 1, snd} );
            v->push_back( {fst - 2, snd} );
            break;
        case RIGHT:
            v->push_back( {fst + 1, snd} );
            v->push_back( {fst + 2, snd} );
            break;
        default:
            assert(false); // should not happen
        }

        try
        {
            primary_grid_.setShipLocation(move(v));
            success = true;
        }
        catch (const InvalidCoordinateError&) { }
        catch (const InvalidShipLocationError&) { }
    }

    // double
    success = false;
    attempt = 0;
    while (!success && attempt++ < MAX_ATTEMPTS)
    {
        fst = rd() % Grid::SIZE;
        snd = rd() % Grid::SIZE;
        auto v = Ship::makeVectorPtr({ {fst, snd} });

        switch((Direction)(rd() % 4))
        {
        case UP:    v->push_back( {fst, snd - 1} ); break;
        case DOWN:  v->push_back( {fst, snd + 1} ); break;
        case LEFT:  v->push_back( {fst - 1, snd} ); break;
        case RIGHT: v->push_back( {fst + 1, snd} ); break;
        default:
            assert(false); // should not happen
        }

        try
        {
            primary_grid_.setShipLocation(move(v));
            success = true;
        }
        catch (const InvalidCoordinateError&) { }
        catch (const InvalidShipLocationError&) { }
    }

    // single
    success = false;
    attempt = 0;
    while (!success && attempt++ < MAX_ATTEMPTS)
    {
        fst = rd() % Grid::SIZE;
        snd = rd() % Grid::SIZE;
        auto v = Ship::makeVectorPtr({ {fst, snd} });

        try
        {
            primary_grid_.setShipLocation(move(v));
            success = true;
        }
        catch (const InvalidCoordinateError&) { }
        catch (const InvalidShipLocationError&) { }
    }
}

std::pair<int, int> battleship::AIPlayer::shoot()
{
    auto v = make_unique<vector<int>>();

    for (auto& s : primary_grid_.getAllShips())
    {
        if (s.getLength() == 0)
            throw BattleshipLogicError("AIPlayer::shoot: cannot shoot before setting ships locations.");
        if (s.canShoot() && !(secondary_grid_.getAvailableRange(s)->empty()))
            v->push_back(s.getLength());
    }

    int length = strategy_ptr_->chooseShip(move(v));
    primary_grid_.shoot(length);
    auto& s = primary_grid_.getShip(length);



    return strategy_ptr_->chooseSquare(move(secondary_grid_.getAvailableRange(s)));
}

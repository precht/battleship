#include "Ship.h"
#include "exceptions.h"

using std::vector;
using std::pair;
using std::unique_ptr;
using std::array;
using std::make_unique;
using std::make_pair;
using std::move;

const array<int, 4> battleship::Ship::RANGES {{ 0, 2, 3, 4 }};
const array<int, 4> battleship::Ship::MAX_SHOTS {{ 0, 1, 2, 2 }};

unique_ptr<vector<pair<int,int>>> battleship::Ship::makeVectorPtr(std::initializer_list<pair<int,int>>&& init_list)
{
    return make_unique<vector<pair<int,int>>>(move(init_list));
}

int battleship::Ship::getRange() const
{
    return occupied_squares_ ? RANGES[occupied_squares_->size()] : 0;
}

int battleship::Ship::getLength() const
{
    return occupied_squares_ ? occupied_squares_->size() : 0;
}

int battleship::Ship::getHits() const
{
    return hits_counter_;
}

bool battleship::Ship::canShoot() const
{
    return occupied_squares_ && !(is_pausing_ || (shots_counter_ >= MAX_SHOTS[occupied_squares_->size()]) || isSunk());
}

bool battleship::Ship::isSunk() const
{
    return occupied_squares_ ? (hits_counter_ >= (int)occupied_squares_->size()) : false;
}

bool battleship::Ship::isPausing() const
{
    return is_pausing_;
}

unique_ptr<vector<pair<int, int>>> battleship::Ship::getOccupiedSquares() const
{
    if (!occupied_squares_) throw BattleshipLogicError("Ship::getOccupiedSquares: location not set");
    return make_unique<vector<pair<int,int>>>(*occupied_squares_);
}

void battleship::Ship::nextRound()
{
    if (!occupied_squares_)
        throw BattleshipLogicError("Ship::nextRound: cannot start next round before setting ship location");

    is_pausing_ = shots_counter_ >= PAUSING_AFTER_SHOTS;
    shots_counter_ = 0;
}

void battleship::Ship::shoot()
{
    if (!canShoot()) throw BattleshipLogicError("Ship::shoot: cannot shoot.");
    shots_counter_++;
}

void battleship::Ship::takeShot()
{
    if (!occupied_squares_) throw BattleshipLogicError("Ship::takeShot: cannot take shot before setting location.");
    if (isSunk()) throw BattleshipLogicError("Ship::takeShot: ship already sunk.");
    hits_counter_++;
}

void battleship::Ship::pause()
{
    is_pausing_ = true;
}

void battleship::Ship::setOccupiedSquares(std::unique_ptr<const std::vector<std::pair<int, int>>> occupied_squares)
{
    // check if passed squares are correct
    if (!occupied_squares)
        throw BattleshipLogicError("Ship::setOccupiedSquares: passed nullptr as argument.");

    // NOTE: it is ShipsGrid job to make sure that occupied_suqares are correct!
    occupied_squares_ = move(occupied_squares);
}

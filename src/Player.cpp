#include "Player.h"

using std::pair;
using std::vector;
using std::make_pair;

bool battleship::Player::canShoot() const
{
    bool can_shoot = false;

    for (auto& x : primary_grid_.getAllShips())
        can_shoot |= x.canShoot() && !(secondary_grid_.getAvailableRange(x)->empty());

    return can_shoot;
}

bool battleship::Player::mayShootNextRounds() const
{
    bool may_shoot = false;

    for (auto& s : primary_grid_.getAllShips())
        may_shoot |= (!s.isSunk()) && (s.canShoot() || s.isPausing())
                                   && !(secondary_grid_.getAvailableRange(s)->empty());
    return may_shoot;
}

int battleship::Player::getHits() const
{
    int hits = 0;
    for (auto& s : primary_grid_.getAllShips())
        hits += s.getHits();
    return hits;
}

const battleship::ShipsGrid& battleship::Player::getPrimaryGird() const
{
    return primary_grid_;
}

const battleship::Grid& battleship::Player::getSecondaryGrid() const
{
    return secondary_grid_;
}

void battleship::Player::nextRound()
{
    primary_grid_.nextRound();
}

battleship::ShotResult battleship::Player::takeShot(pair<int, int> square)
{
    return primary_grid_.takeShot(square);
}

void battleship::Player::update(pair<int, int> square, battleship::ShotResult result)
{
    secondary_grid_.update(square, result);
}

void battleship::Player::setUpShips(const vector<int>& single_args,
                                    const vector<int>& double_args,
                                    const vector<int>& triple_args)
{
    if (single_args.size() != 2 || double_args.size() != 4 || triple_args.size() != 6)
        throw InvalidShipLocationError("Player::setUpShips: wrong size of input locations.");

    auto v = Ship::makeVectorPtr({ { single_args[0], single_args[1] } });
    primary_grid_.setShipLocation(move(v));

    v = Ship::makeVectorPtr({ { double_args[0], double_args[1] },
                              { double_args[2], double_args[3] } });
    primary_grid_.setShipLocation(move(v));

    v = Ship::makeVectorPtr({ { triple_args[0], triple_args[1] },
                              { triple_args[2], triple_args[3] },
                              { triple_args[4], triple_args[5] } });
    primary_grid_.setShipLocation(move(v));
}

void battleship::Player::pauseShips(const std::vector<int>& ships_lengths)
{
    primary_grid_.pauseShips(ships_lengths);
}

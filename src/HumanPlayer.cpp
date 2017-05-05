#include "HumanPlayer.h"
#include "exceptions.h"

#include <iostream>

using std::unique_ptr;
using std::vector;
using std::pair;
using std::unordered_set;

battleship::HumanPlayer::HumanPlayer(std::shared_ptr<battleship::UI> ui)
    : Player()
    , ui_(ui)
{ }

void battleship::HumanPlayer::setUpShips()
{
    // checks if ships coordinates are correct and ships are in proper locations are performed by ShipsGrid

    ui_->displayMessage("Set up your ships:\n");

    // single
    ui_->displayMessage("First, set up single ship.");
    bool success = false;
    while (!success)
    {
        ui_->displayMessage("You must specify ship location.");
        auto p = ui_->chooseSquare();
        try
        {
            primary_grid_.setShipLocation(Ship::makeVectorPtr({ p }));
            success = true;
        }
        catch (const InvalidShipLocationError&)
        {
            ui_->displayMessage("Wrong ship location, try again.");
        }
        catch (const InvalidCoordinateError&)
        {
            ui_->displayMessage("Square coordinates out of range, try again.");
        }
    }

    // double
    ui_->displayMessage("Now, set up double ship.");
    success = false;
    while (!success)
    {
        ui_->displayMessage("You must specify ship location.");
        auto p1 = ui_->chooseSquare();
        auto p2 = ui_->chooseSquare();
        try
        {
            primary_grid_.setShipLocation(Ship::makeVectorPtr({ p1, p2 }));
            success = true;
        }
        catch (const InvalidShipLocationError&)
        {
            ui_->displayMessage("Wrong ship location, try again.");
        }
        catch (const InvalidCoordinateError&)
        {
            ui_->displayMessage("Square coordinates out of range, try again.");
        }
    }

    // triple
    ui_->displayMessage("Finally, set up triple ship.");
    success = false;
    while (!success)
    {
        ui_->displayMessage("You must specify ship location.");
        auto p1 = ui_->chooseSquare();
        auto p2 = ui_->chooseSquare();
        auto p3 = ui_->chooseSquare();
        try
        {
            primary_grid_.setShipLocation(Ship::makeVectorPtr({ p1, p2, p3 }));
            success = true;
        }
        catch (const InvalidShipLocationError&)
        {
            ui_->displayMessage("Wrong ship location, try again.");
        }
        catch (const InvalidCoordinateError&)
        {
            ui_->displayMessage("Square coordinates out of range, try again.");
        }
    }
}

std::pair<int, int> battleship::HumanPlayer::shoot()
{
    // Checks wheater choosen square is within allowed range must be performed by player
    // because ships and it's shoot are stored in separate grids.

    int length = 0;
    int count = 0;
    for (auto& s : primary_grid_.getAllShips())
    {
        if (s.getLength() == 0)
            throw BattleshipLogicError("AIPlayer::shoot: cannot shoot before setting ships locations.");
        else if (s.canShoot())
        {
            count++;
            length = s.getLength();
        }
    }

    unique_ptr<unordered_set<pair<int,int>, battleship::SquareHash>> range = nullptr;

    // if there is only one ship to shoot it's already remembered in length variable
    if (count == 1)
    {
        range = secondary_grid_.getAvailableRange(primary_grid_.getShip(length));
        if (range->empty())
            throw BattleshipRuntimeError("HumanPlayer::shoot: None of the ships has non-empty available range.");
    }
    else
    {
        while (true)
        {
            ui_->displayMessage("Choose ship to perform shot.");
            length = ui_->chooseShip();
            range = secondary_grid_.getAvailableRange(primary_grid_.getShip(length));

            if (primary_grid_.getShip(length).canShoot() && !range->empty())
                break;

            ui_->displayMessage("This ship cannot shoot, try again.");
        }
    }

    // display possible targets
//    ui_->displayMessage("Available targets");
//    for (const auto& x : rangeSet)
//        ui_->displayMessage("(" + x.first + ", " + x.second + "),  ");

    pair<int,int> t {0,0};
    while (true)
    {
        ui_->displayMessage("Choose target for ship " + std::to_string(length) + ":");
        t = ui_->chooseSquare();

        if (range->count(t))
            break;

        ui_->displayMessage("Wrong target, try again.");
    }

    primary_grid_.shoot(length);

    return t;
}

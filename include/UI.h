#ifndef UI_H_
#define UI_H_

#include "Player.h"

#include <utility>
#include <string>
#include <memory>

namespace battleship
{

    class UI
    {
    public:
        virtual ~UI() = default;

        virtual void create() = 0;
        virtual void destroy() = 0;

        virtual void displayPlayer(const Player& player) = 0;
        virtual void displayPlayers(const Player& first, const Player& second) = 0;
        virtual void displayMessage(const std::string& message) = 0;

        virtual void cleanScreen() = 0;

        virtual int chooseShip() = 0;
        virtual std::pair<int, int> chooseSquare() = 0;
        virtual bool askQuestion(const std::string& question) = 0;
    };

}

#endif // !UI_H_

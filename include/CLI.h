#ifndef CLI_H_
#define CLI_H_

#include "UI.h"
#include "Player.h"

#include <utility>
#include <string>
#include <memory>

namespace battleship
{

    class CLI : public UI
    {
    public:
        CLI() = default;
        ~CLI() override = default;

        void create() override;
        void destroy() override;

        void displayPlayer(const Player& player) override;
        void displayPlayers(const Player& player, const Player& opponent) override;
        void displayMessage(const std::string& message) override;

        void cleanScreen();

        int chooseShip() override;
        std::pair<int, int> chooseSquare() override;
        bool askQuestion(const std::string& question) override;

    private:
        static const std::string HEADER_;
        static const std::string LINE_;
        static const std::string INTERSPACE_;
    };

}

#endif // !CLI_H_

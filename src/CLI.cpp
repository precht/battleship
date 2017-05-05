#include "CLI.h"
#include "Grid.h"

#include <iostream>
#include <utility>
#include <string>
#include <cstdlib>
#include <cctype>

using std::cout;
using std::cin;
using std::endl;
using std::make_pair;
using std::string;

void battleship::CLI::create()
{ }

void battleship::CLI::destroy()
{ }

const std::string battleship::CLI::HEADER_ {{ \
        "    Primary grid:                                        Secondary grid:\n"\
        "y\\x| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |        y\\x| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |"\
}};
const std::string battleship::CLI::LINE_ {{ \
        "\n---+---+---+---+---+---+---+---+---+---+---+        ---+---+---+---+---+---+---+---+---+---+---+\n"\
}};
const std::string battleship::CLI::INTERSPACE_ {{ "         " }};

void battleship::CLI::displayPlayer(const battleship::Player& player)
{
    auto& pg = player.getPrimaryGird();
    auto& sg = player.getSecondaryGrid();

    cout << HEADER_;
    cout << LINE_;
    for (int y = 0; y < 10; y++)
    {
        cout << ' ' << y << " |";
        for (int x = 0; x < 10; x++)
            cout << ' ' << (char)pg.at(make_pair(x,y)) << " |";
        cout << INTERSPACE_ << y << " |";
        for (int x = 0; x < 10; x++)
            cout << ' ' << (char)sg.at(make_pair(x,y)) << " |";
        cout << LINE_;
    }
    cout << endl;
}

void battleship::CLI::displayPlayers(const battleship::Player& player, const battleship::Player& opponent)
{
    cout << "\nPlayer:\n";
    displayPlayer(player);
    cout << "Opponent:\n";
    displayPlayer(opponent);
    cout << endl;
}

void battleship::CLI::displayMessage(const std::string& message)
{
    cout << message << endl;
}

void battleship::CLI::cleanScreen()
{
#ifdef _WIN32
    std::system("cls");
#else
    // Assume POSIX
    std::system ("clear");
#endif
}

int battleship::CLI::chooseShip()
{
    int n;
    cout << "Choose your ship (enter one integer number - ship's length): ";
    cin >> n;
    cout << endl;
    return n;
}

std::pair<int, int> battleship::CLI::chooseSquare()
{
    int fst, snd;
    cout << "Choose square (enter two integer numbers - x and y): ";
    cin >> fst >> snd;
    cout << endl;
    return make_pair(fst, snd);
}

bool battleship::CLI::askQuestion(const string& question)
{
    char c;
    while (true)
    {
        cout << question << " (enter 'Y/y' for yes or 'N/n' for no): ";
        cin >> c;
        cout << endl;
        c = std::tolower(c);
        if (c == 'y' || c == 'n')
            break;
        cout << "Entered wrong character, try again." << endl;
    }
    return c == 'y';
}

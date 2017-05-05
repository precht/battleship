#include "CLI.h"
#include "AIPlayer.h"
#include "RandomStrategy.h"
#include "HumanPlayer.h"
#include "GameLogic.h"

#include <iostream>

using namespace battleship;


int main(int argc, char **argv)
{
    try
    {
        GameLogic g(argc, argv, std::make_shared<CLI>());
        g.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    catch(...)
    {
        std::cerr << "unknown error." << std::endl;
    }
}


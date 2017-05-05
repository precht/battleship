#ifndef GAME_LOGIC_
#define GAME_LOGIC_

#include "Player.h"
#include "UI.h"

#include <boost/program_options.hpp>
#include <string>
#include <memory>

namespace battleship
{

    class GameLogic
    {
    public:
        GameLogic(int argc, char** argv, std::shared_ptr<UI> ui_ptr);
        ~GameLogic();

        void run();
        void loadGameFromFile();
        void saveGameToFile() const;

    private:
        const int AI_REACTION_TIME = 2000;

        std::shared_ptr<UI> ui_;
        int max_rounds_;
        int round_counter_ = 0;
        bool is_human_ = false;

        std::string input_name_;
        std::string output_name_;

        // the main player human player or ai player
        Player* main_player_ = nullptr;
        Player* opponent_player_ = nullptr;

        // description of all posible options that can be used
        boost::program_options::options_description& description_;

        // description of args order when entering without options names
        boost::program_options::positional_options_description& positional_;

        // description of save parameters
        boost::program_options::options_description& state_format_;

        // options entered by user
        boost::program_options::variables_map used_options_;

        boost::program_options::options_description& loadDescritpion();
        boost::program_options::positional_options_description& loadPositional();
        boost::program_options::options_description& loadStateFormat();
        void validateCmdlineOptions();
        void validateUsedOptions();
        void validateGameState();
        void initializePlayers();

        void updateUI();
    };

    // define used options
    #define HELP "help"
    #define ROUNDS "rounds"
    #define PLAYER "player"
    #define OPPONENT "opponent"
    #define SAVE "save"
    #define LOAD "load"

    #define DEFAULT_FILE ".battleship.autosave"
    #define HUMAN "human"
    #define RANDOM "random"
    #define GREEDY "greedy"

    // define long and short names of options
    #define ROUND_NUMBER "number-round"
    #define STATE_INFO "state-info"

    #define PLAYER_HITS "htis-player"
    #define PLAYER_SHIP_1 "ship-1-player"
    #define PLAYER_SHIP_2 "ship-2-player"
    #define PLAYER_SHIP_3 "ship-3-player"
    #define PLAYER_PAUSING_SHIPS "ships-pausing-player"

    #define OPPONENT_HITS "htis-opponent"
    #define OPPONENT_SHIP_1 "ship-1-opponent"
    #define OPPONENT_SHIP_2 "ship-2-opponent"
    #define OPPONENT_SHIP_3 "ship-3-opponent"
    #define OPPONENT_PAUSING_SHIPS "ships-pausing-opponent"

}

#endif // !GAME_LOGIC_



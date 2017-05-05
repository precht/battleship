#include "GameLogic.h"
#include "AIPlayer.h"
#include "RandomStrategy.h"
#include "GreedyStrategy.h"
#include "HumanPlayer.h"

#include <boost/filesystem.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <fstream>
#include <utility>
#include <ctime>
#include <chrono>

namespace po = boost::program_options;
namespace fs = boost::filesystem;
using std::string;
using std::make_unique;
using std::vector;
using std::pair;

battleship::GameLogic::GameLogic(int argc, char** argv, std::shared_ptr<UI> ui)
    : ui_(ui)
    , description_(loadDescritpion())
    , positional_(loadPositional())
    , state_format_(loadStateFormat())
{
    // parse and remember args
    po::store(po::command_line_parser(argc, argv).options(description_).positional(positional_).run(), used_options_);
    po::notify(used_options_);

    if (used_options_.count(HELP))
        return;

    validateCmdlineOptions();

    // load saved state from file if LOAD option
    if (used_options_.count(LOAD))
        loadGameFromFile();
    // otherways just create empty players
    else
        initializePlayers();
}

battleship::GameLogic::~GameLogic()
{
    delete main_player_;
    delete opponent_player_;
    delete &description_;
    delete &positional_;
    delete &state_format_;
}

void battleship::GameLogic::initializePlayers()
{
    // main players
    auto& str = used_options_[PLAYER].as<string>();
    if (str.compare(HUMAN) == 0)
    {
        main_player_ = new HumanPlayer(ui_);
        is_human_ = true;
    }
    else if (str.compare(RANDOM) == 0)
            main_player_ = new AIPlayer(make_unique<RandomStrategy>());
    else
            main_player_ = new AIPlayer(make_unique<GreedyStrategy>());

    // opponent player
    if (used_options_[OPPONENT].as<string>().compare(RANDOM) == 0)
        opponent_player_ = new AIPlayer(make_unique<RandomStrategy>());
    else
        opponent_player_ = new AIPlayer(make_unique<GreedyStrategy>());
}

po::options_description& battleship::GameLogic::loadDescritpion()
{
    auto& desc = *new po::options_description("Allowed options");
    desc.add_options()
            (HELP ",h", "produce help message")
            (ROUNDS ",r", po::value<int>(&max_rounds_), "set max rounds number, (>0), (<=20)")
            (OPPONENT ",o", po::value<string>(), "set opponent type: 'greedy', 'random'")
            (PLAYER ",p", po::value<string>()->default_value("human"), "set player type: 'human', 'greedy', 'random'")
            (LOAD ",l", po::value<string>(&input_name_)->implicit_value(DEFAULT_FILE), "load game state from file")
            (SAVE ",s", po::value<string>(&output_name_)->default_value(DEFAULT_FILE),
                     "set name for autosave.\nthe game will be save after each round.\nif name was left to default,"\
                     " the save will be deleted after normal game end")
    ;
    return desc;
}

po::positional_options_description& battleship::GameLogic::loadPositional()
{
    auto& p = *new po::positional_options_description();
    p.add(ROUNDS, 1);
    p.add(OPPONENT, 1);
    p.add(PLAYER, 1);
    return p;
}

boost::program_options::options_description&battleship::GameLogic::loadStateFormat()
{
    auto& sf = *new po::options_description("Save description");
    sf.add_options()
            (STATE_INFO, po::value<string>())
            (ROUND_NUMBER, po::value<int>())
            (PLAYER_HITS, po::value<vector<int>>())
            (PLAYER_SHIP_1, po::value<vector<int>>())
            (PLAYER_SHIP_2, po::value<vector<int>>())
            (PLAYER_SHIP_3, po::value<vector<int>>())
            (PLAYER_PAUSING_SHIPS, po::value<vector<int>>())
            (OPPONENT_HITS, po::value<vector<int>>())
            (OPPONENT_SHIP_1, po::value<vector<int>>())
            (OPPONENT_SHIP_2, po::value<vector<int>>())
            (OPPONENT_SHIP_3, po::value<vector<int>>())
            (OPPONENT_PAUSING_SHIPS, po::value<vector<int>>())
    ;
    return sf;
}

void battleship::GameLogic::validateCmdlineOptions()
{
    // if loading state from file there cannot be duplicated options
    if (used_options_.count(LOAD))
    {
        if (used_options_.count(ROUNDS)) throw ArgumentsError("conflict options: '--" LOAD "' and '--" ROUNDS "'.");
        if (used_options_.count(OPPONENT)) throw ArgumentsError("conflict options: '--" LOAD "' and '--" OPPONENT "'.");
//        if (used_options_.count(PLAYER)) throw ArgumentsError("conflict options: '--" LOAD "' and '--" PLAYER "'.");
    }
    else
        validateUsedOptions();
}

void battleship::GameLogic::validateUsedOptions()
{
    if (!used_options_.count(ROUNDS) || !used_options_.count(OPPONENT))
        throw ArgumentsError("must specify '--rounds' and '--opponent' arguments.");

    auto r = used_options_[ROUNDS].as<int>();
    if (r <= 0 || r > 20)
        throw ArgumentsError("the argument ('" + std::to_string(r) + "') for option '--" ROUNDS "' is invalid.");

    auto str = used_options_[OPPONENT].as<string>();
    if (str.compare(RANDOM) && str.compare(GREEDY))
        throw ArgumentsError("the argument ('" + str + "') for option '--" OPPONENT "' is invalid.");

    str = used_options_[PLAYER].as<string>();
    if (str.compare(RANDOM) && str.compare(GREEDY) && str.compare(HUMAN))
        throw ArgumentsError("the argument ('" + str + "') for option '--" PLAYER "' is invalid.");
}

void battleship::GameLogic::validateGameState()
{
    validateUsedOptions();

    if (   // make sure all necessary options are used
           !used_options_.count(STATE_INFO)
           || !used_options_.count(ROUNDS)
           || !used_options_.count(ROUND_NUMBER)
           || !used_options_.count(PLAYER_SHIP_1)
           || !used_options_.count(PLAYER_SHIP_2)
           || !used_options_.count(PLAYER_SHIP_3)
           || !used_options_.count(OPPONENT_SHIP_1)
           || !used_options_.count(OPPONENT_SHIP_2)
           || !used_options_.count(OPPONENT_SHIP_3)
           // make sure that there is even number of coordinates (because they are pairs)
           || used_options_[PLAYER_SHIP_1].as<vector<int>>().size() % 2 != 0
           || used_options_[PLAYER_SHIP_2].as<vector<int>>().size() % 2 != 0
           || used_options_[PLAYER_SHIP_3].as<vector<int>>().size() % 2 != 0
           || used_options_[OPPONENT_SHIP_1].as<vector<int>>().size() % 2 != 0
           || used_options_[OPPONENT_SHIP_2].as<vector<int>>().size() % 2 != 0
           || used_options_[OPPONENT_SHIP_3].as<vector<int>>().size() % 2 != 0
           || (used_options_.count(PLAYER_HITS) && used_options_[PLAYER_HITS].as<vector<int>>().size() % 2 != 0)
           || (used_options_.count(OPPONENT_HITS) && used_options_[OPPONENT_HITS].as<vector<int>>().size() % 2 != 0)
           )
        throw ArgumentsError("Invalid game state in file: '" + input_name_ + "'.");
}

void battleship::GameLogic::loadGameFromFile()
{
    // read game state from file
    std::ifstream file;
    file.open(input_name_);

    po::options_description op;
    op.add(description_).add(state_format_);
    po::store(po::parse_config_file(file, op), used_options_);
    po::notify(used_options_);

    file.close();
    validateGameState();

    // initialize game state

    initializePlayers();
    try
    {
        round_counter_ = used_options_[ROUND_NUMBER].as<int>();

        // set up ships
        main_player_->setUpShips(used_options_[PLAYER_SHIP_1].as<vector<int>>(),
                                 used_options_[PLAYER_SHIP_2].as<vector<int>>(),
                                 used_options_[PLAYER_SHIP_3].as<vector<int>>());
        opponent_player_->setUpShips(used_options_[OPPONENT_SHIP_1].as<vector<int>>(),
                                     used_options_[OPPONENT_SHIP_2].as<vector<int>>(),
                                     used_options_[OPPONENT_SHIP_3].as<vector<int>>());

        // simulate hits
        if (used_options_.count(PLAYER_HITS))
        {
            const auto& v = used_options_[PLAYER_HITS].as<vector<int>>();
            for (auto it = v.begin(); it != v.end(); it += 2)
            {
                pair<int,int> p = { *it, *(it + 1) };
                main_player_->update(p, opponent_player_->takeShot(p));
            }
        }
        if (used_options_.count(OPPONENT_HITS))
        {
            const auto& v = used_options_[OPPONENT_HITS].as<vector<int>>();
            for (auto it = v.begin(); it != v.end(); it += 2)
            {
                pair<int,int> p = { *it, *(it + 1) };
                opponent_player_->update(p, main_player_->takeShot(p));
            }
        }

        // pausing ships
        if (used_options_.count(PLAYER_PAUSING_SHIPS))
            main_player_->pauseShips(used_options_[PLAYER_PAUSING_SHIPS].as<vector<int>>());
        if (used_options_.count(OPPONENT_PAUSING_SHIPS))
            opponent_player_->pauseShips(used_options_[OPPONENT_PAUSING_SHIPS].as<vector<int>>());
    }
    catch (const BattleshipRuntimeError&)
    {
        throw ArgumentsError("Invalid game state in file: '" + input_name_ + "'.");
    }
    catch (const BattleshipLogicError&)
    {
        throw ArgumentsError("Invalid game state in file: '" + input_name_ + "'.");
    }
}

void battleship::GameLogic::saveGameToFile() const
{
    std::ofstream file;
    file.exceptions(std::ios_base::failbit | std::ios_base::badbit);
    try
    {
        file.open(output_name_);

        // save status
        // save actual time
        std::time_t now_c = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        struct tm *p = std::localtime(&now_c);
        file << STATE_INFO << " = "  << 1900 + p->tm_year << '-' << 1 + p->tm_mon << '-' << 1 + p->tm_mday
             << "_" << p->tm_hour << ":" << p->tm_min << ":" << p->tm_sec <<'\n';
        file << ROUND_NUMBER << " = " << round_counter_ << '\n';
        file << ROUNDS << " = " << max_rounds_ << '\n';
        file << OPPONENT << " = " << used_options_[OPPONENT].as<string>() << '\n';
        file << PLAYER << " = " << used_options_[PLAYER].as<string>() << '\n';

        // save ships
        auto v = main_player_->getPrimaryGird().getShip(1).getOccupiedSquares();
        for (auto t : *v)
            file << PLAYER_SHIP_1 << " = " << t.first << '\n' << PLAYER_SHIP_1 << " = " << t.second << '\n';
        if (main_player_->getPrimaryGird().getShip(1).isPausing())
            file << PLAYER_PAUSING_SHIPS << " = 1\n";

        v = main_player_->getPrimaryGird().getShip(2).getOccupiedSquares();
        for (auto t : *v)
            file << PLAYER_SHIP_2 << " = " << t.first << '\n' << PLAYER_SHIP_2 << " = " << t.second << '\n';
        if (main_player_->getPrimaryGird().getShip(2).isPausing())
            file << PLAYER_PAUSING_SHIPS << " = 2\n";

        v = main_player_->getPrimaryGird().getShip(3).getOccupiedSquares();
        for (auto t : *v)
            file << PLAYER_SHIP_3 << " = " << t.first << '\n' << PLAYER_SHIP_3 << " = " << t.second << '\n';
        if (main_player_->getPrimaryGird().getShip(3).isPausing())
            file << PLAYER_PAUSING_SHIPS << " = 3\n";

        v = opponent_player_->getPrimaryGird().getShip(1).getOccupiedSquares();
        for (auto t : *v)
            file << OPPONENT_SHIP_1 << " = " << t.first << '\n' << OPPONENT_SHIP_1 << " = " << t.second << '\n';
        if (opponent_player_->getPrimaryGird().getShip(1).isPausing())
            file << OPPONENT_PAUSING_SHIPS << " = 1\n";

        v = opponent_player_->getPrimaryGird().getShip(2).getOccupiedSquares();
        for (auto t : *v)
            file << OPPONENT_SHIP_2 << " = " << t.first << '\n' << OPPONENT_SHIP_2 << " = " << t.second << '\n';
        if (opponent_player_->getPrimaryGird().getShip(2).isPausing())
            file << OPPONENT_PAUSING_SHIPS << " = 2\n";

        v = opponent_player_->getPrimaryGird().getShip(3).getOccupiedSquares();
        for (auto t : *v)
            file << OPPONENT_SHIP_3 << " = " << t.first << '\n' << OPPONENT_SHIP_3 << " = " << t.second << '\n';
        if (opponent_player_->getPrimaryGird().getShip(3).isPausing())
            file << OPPONENT_PAUSING_SHIPS << " = 3\n";

        // save hits
        auto& mg = main_player_->getSecondaryGrid();
        for (int x = 0; x < 10; x++)
             for (int y = 0; y < 10; y++)
             if (mg.at({x,y}) != ST_EMPTY)
                 file << PLAYER_HITS << " = " << x << '\n' << PLAYER_HITS << " = " << y << '\n';

        auto& og = opponent_player_->getSecondaryGrid();
        for (int x = 0; x < 10; x++)
             for (int y = 0; y < 10; y++)
             if (og.at({x,y}) != ST_EMPTY)
                 file << OPPONENT_HITS << " = " << x << '\n' << OPPONENT_HITS << " = " << y << '\n';

        file.close();
    }
    catch (const std::ios_base::failure& e) { }
}

void battleship::GameLogic::updateUI()
{
    ui_->cleanScreen();
    ui_->displayMessage("Round no. " + std::to_string(round_counter_));

    if (is_human_)
        ui_->displayPlayer(*main_player_);
    else
        ui_->displayPlayers(*main_player_, *opponent_player_);
}

void battleship::GameLogic::run()
{
    if (used_options_.count(HELP))
    {
        std::cout << description_ << std::endl;
        return;
    }

    // set up ships
    if (!used_options_.count(LOAD))
    {
        if (is_human_)
        {
            ui_->cleanScreen();
            ui_->displayPlayer(*main_player_);
        }
        main_player_->setUpShips();
        opponent_player_->setUpShips();
    }

    while (++round_counter_ <= max_rounds_)
    {
        updateUI();
        // main player
        if (!main_player_->canShoot())
        {
            if (!main_player_->mayShootNextRounds())
            {
                ui_->displayMessage("You lost!\n");
                return;
            }
            else
                ui_->displayMessage("In this round you are pausing.");
        }
        else
        {
            if (!is_human_)
            {
                ui_->displayMessage("Player's turn...");
                std::this_thread::sleep_for(std::chrono::milliseconds{AI_REACTION_TIME});
            }
            auto p = main_player_->shoot();
            main_player_->update(p, opponent_player_->takeShot(p));
            updateUI();

            while(main_player_->canShoot()
                  && (!is_human_ || ui_->askQuestion("Do you want to shoot one more time in this round?")))
            {
                auto p = main_player_->shoot();
                auto sr = opponent_player_->takeShot(p);
                main_player_->update(p, sr);
                updateUI();
            }
        }

        // opponent player
        if (!opponent_player_->canShoot() && !opponent_player_->mayShootNextRounds())
        {
            ui_->displayMessage("You win!");
            return;
        }

        ui_->displayMessage("The opponent's turn...");
        std::this_thread::sleep_for(std::chrono::milliseconds{AI_REACTION_TIME});

        while (opponent_player_->canShoot())
        {
            auto p = opponent_player_->shoot();
            opponent_player_->update(p, main_player_->takeShot(p));
        }

        // next round
        main_player_->nextRound();
        opponent_player_->nextRound();
        saveGameToFile();
    }

    // count hits
    int main_hits = main_player_->getHits();
    int opponent_hits = opponent_player_->getHits();
    if (main_hits == opponent_hits)
        ui_->displayMessage("Draw, no one wins\n"\
                            "(after playing all rounds you and the opponent hit each other the same number of times)");
    else if (main_hits < opponent_hits)
        ui_->displayMessage("You win!\n(after playing all rounds you hit the opponent more times)");
    else
        ui_->displayMessage("You lost!\n(after playing all rounds the opponent hit you more times)");

    // delete save file if it was default name
    if (output_name_.compare(DEFAULT_FILE) == 0 && fs::exists(output_name_))
        fs::remove(output_name_);
}

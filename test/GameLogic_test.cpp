#include "GameLogic.h"
#include "test/UI_mock.h"

#include "gtest/gtest.h"
#include <memory>

using namespace battleship;
using battleship_test::MockUI;
using std::vector;
using std::string;

TEST(GameLogicTest, constructor_long_args_names)
{
    vector<string> v = { "app", "--rounds", "10", "--opponent", "greedy", "--player", "human" };

    vector<char*> argv;
    for (const auto& arg : v)
        argv.push_back((char*)arg.data());
    argv.push_back(nullptr);

    GameLogic g(argv.size() - 1, argv.data(), std::make_shared<MockUI>());
}

TEST(GameLogicTest, constructor_short_args_names)
{
    vector<string> v = { "app", "-r", "10", "-o", "greedy" };

    vector<char*> argv;
    for (const auto& arg : v)
        argv.push_back((char*)arg.data());
    argv.push_back(nullptr);

    GameLogic g(argv.size() - 1, argv.data(), std::make_shared<MockUI>());
}

TEST(GameLogicTest, constructor_no_args_names)
{
    vector<string> v = { "app", "10", "greedy", "random" };

    vector<char*> argv;
    for (const auto& arg : v)
        argv.push_back((char*)arg.data());
    argv.push_back(nullptr);

    GameLogic g(argv.size() - 1, argv.data(), std::make_shared<MockUI>());
}

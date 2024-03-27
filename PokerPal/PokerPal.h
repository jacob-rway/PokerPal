#pragma once
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

constexpr float WHITE_CHIP_VALUE = 0.01f;
constexpr float RED_CHIP_VALUE   = 0.05f;
constexpr float BLUE_CHIP_VALUE  = 0.10f;
constexpr float GREEN_CHIP_VALUE = 0.25f;
constexpr float BLACK_CHIP_VALUE = 1.00f;

const std::string CHIP_COLORS[] = { "white", "red", "blue", "green", "black" };

enum IntInputValidationOptions { MAIN_MENU, ENTER_CHIP_AMOUNTS, SET_POT };

enum StrInputValidationOptions { ADD_PLAYER, REMOVE_PLAYER, EDIT_PLAYER_CHIPS };

struct Player
{
    std::string name;
    int whiteChips;
    int redChips;
    int blueChips;
    int greenChips;
    int blackChips;

    Player()
        : name("NONE"), whiteChips(0), redChips(0), blueChips(0), greenChips(0),
        blackChips(0)
    {
    }
};

std::vector<Player> loadPlayerList();
std::vector<Player> playerList = loadPlayerList();

std::vector<Player> loadPlayerList()
{
    std::ifstream inFile("players.txt");
    std::vector<Player> players;
    players.push_back(Player()); // default player, used for error handling

    if (!inFile.is_open())
    {
        std::cerr << "ERROR: Missing 'players.txt' file!" << '\n';
    }
    else
    {
        std::string line;

        while (std::getline(inFile, line))
        {
            Player player;
            player.name = line;
            players.push_back(player);
        }

        inFile.close();
    }

    return players;
}

inline float calculateWinnings(const Player& player)
{
    float winnings = 0;
    winnings += player.whiteChips * WHITE_CHIP_VALUE;
    winnings += player.redChips   * RED_CHIP_VALUE;
    winnings += player.blueChips  * BLUE_CHIP_VALUE;
    winnings += player.greenChips * GREEN_CHIP_VALUE;
    winnings += player.blackChips * BLACK_CHIP_VALUE;

    return winnings;
}

inline Player getPlayer(const std::string& name)
{
    for (int i = 1; i < playerList.size(); i++)
    {
        if (playerList[i].name == name && playerList[i].name != "NONE")
        {
            return playerList[i];
        }
    }

    std::cerr << "ERROR: Player '" << name << "' not found!" << '\n';
    return playerList[0];
}

inline int getPlayerIndex(const std::string& name)
{
    for (int i = 1; i < playerList.size(); i++)
    {
        if (playerList[i].name == name && playerList[i].name != "NONE")
        {
            return i;
        }
    }

    std::cerr << "ERROR: Player '" << name << "' not found!" << '\n';
    return -1;
}

inline Player& getPlayerReference(const std::string& name)
{
    for (Player& player : playerList)
    {
        if (player.name == name && player.name != "NONE")
        {
            return player;
        }
    }

    std::cerr << "ERROR: Player '" + name + "' not found!" << '\n';
    return playerList[0];
}

inline bool playerExists(const std::string& name)
{
    for (int i = 1; i < playerList.size(); i++)
    {
        if (playerList[i].name == name)
        {
            return true;
        }
    }

    return false;
}

inline void printMenu()
{
    std::cout << "Choose an option:" << '\n';
    std::cout << "1. Add a Player" << '\n';
    std::cout << "2. Remove a Player" << '\n';
    std::cout << "3. Enter Player Chip Amounts" << '\n';
    std::cout << "4. Print Player Winnings" << '\n';
    std::cout << "5. Set Pot" << '\n';
    std::cout << "6. Exit & Save Player List" << '\n';
}

inline void printPlayers()
{
    std::cout << playerList.size() - 1 << " currently loaded players: ";
    for (int i = 1; i < playerList.size(); i++)
    {
        if (i < playerList.size() - 1)
        {
            std::cout << playerList[i].name << ", ";
        }
        else
        {
            std::cout << playerList[i].name << '\n' << '\n';
        }
    }
}

inline void printBanner()
{
    std::cout << "***********************************************" << '\n';
    std::cout << "          POKER PAL by JACOB RIDGEWAY          " << '\n';
    std::cout << "     A command-line based poker game tool.     " << '\n';
    std::cout << "               Developed 3/25/24               " << '\n';
    std::cout << "***********************************************" << '\n';
    std::cout << '\n';
}

int getIntegerInput(enum IntInputValidationOptions option)
{
    switch (option)
    {
    case MAIN_MENU:
    {
        int input;
        std::cin >> input;
        std::cout << '\n';

        while (std::cin.fail() || input <= 0 || input > 6)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "ERROR: Please enter a valid menu option: ";
            std::cin >> input;
        }

        return input;
    }

    case ENTER_CHIP_AMOUNTS:
    {
        int input;
        std::cin >> input;

        while (std::cin.fail() || input < 0)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr
                << "ERROR: Invalid amount! Enter a valid number of chips: ";
            std::cin >> input;
        }

        return input;
    }

    case SET_POT:
    {
        int input;
        std::cin >> input;

        while (std::cin.fail() || (input != 1 && input != 2))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "ERROR: Please enter a valid menu option: ";
            std::cin >> input;
        }

        return input;
    }
    }
}

std::string getStringInput(enum StrInputValidationOptions option)
{
    switch (option)
    {
    case ADD_PLAYER:
    {
        std::string input;
        std::cin >> input;

        while (input == "NONE" || playerExists(input))
        {
            std::cerr << "ERROR: Name is not allowed or taken! Please enter a "
                "different name: ";
            std::cin >> input;
        }

        return input;
    }

    case REMOVE_PLAYER:
    {
        std::string input;
        std::cin >> input;

        while (!playerExists(input))
        {
            std::cerr << "ERROR: Player not found! Please enter a valid name: ";
            std::cin >> input;
        }

        return input;
    }

    case EDIT_PLAYER_CHIPS:
    {
        std::string input;
        std::cin >> input;

        while (!playerExists(input))
        {
            std::cerr << "ERROR: Player not found! Please enter a valid name: ";
            std::cin >> input;
        }

        return input;
    }
    }
}

inline void printChipAmounts(const Player& player)
{
    std::cout << "Total chip amounts for " << player.name << ": " << '\n';

    std::cout << "White: " << player.whiteChips << " - $" << (player.whiteChips * WHITE_CHIP_VALUE) << '\n';
    std::cout << "Red: "   << player.redChips   << " - $" << (player.redChips   * RED_CHIP_VALUE)   << '\n';
    std::cout << "Blue: "  << player.blueChips  << " - $" << (player.blueChips  * BLUE_CHIP_VALUE)  << '\n';
    std::cout << "Green: " << player.greenChips << " - $" << (player.greenChips * GREEN_CHIP_VALUE) << '\n';
    std::cout << "Black: " << player.blackChips << " - $" << (player.blackChips * BLACK_CHIP_VALUE) << '\n';
}
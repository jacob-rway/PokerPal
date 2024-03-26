#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
// ReSharper disable All

const float WHITE_CHIP_VALUE = 0.01f;
const float RED_CHIP_VALUE = 0.05f;
const float BLUE_CHIP_VALUE = 0.10f;
const float GREEN_CHIP_VALUE = 0.25f;
const float BLACK_CHIP_VALUE = 1.00f;
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
        std::cerr << "ERROR: Missing 'players.txt' file!" << std::endl;
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

float calculateWinnings(const Player& player)
{
    float winnings = 0;
    winnings += player.whiteChips * WHITE_CHIP_VALUE;
    winnings += player.redChips * RED_CHIP_VALUE;
    winnings += player.blueChips * BLUE_CHIP_VALUE;
    winnings += player.greenChips * GREEN_CHIP_VALUE;
    winnings += player.blackChips * BLACK_CHIP_VALUE;

    return winnings;
}

Player getPlayer(const std::string& name)
{
    for (int i = 1; i < playerList.size(); i++)
    {
        if (playerList[i].name == name && playerList[i].name != "NONE")
        {
            return playerList[i];
        }
    }

    std::cerr << "ERROR: Player '" << name << "' not found!" << std::endl;
    return playerList[0];
}

int getPlayerIndex(const std::string& name)
{
    for (int i = 1; i < playerList.size(); i++)
    {
        if (playerList[i].name == name && playerList[i].name != "NONE")
        {
            return i;
        }
    }

    std::cerr << "ERROR: Player '" << name << "' not found!" << std::endl;
    return -1;
}

Player& getPlayerReference(const std::string& name)
{
    for (Player& player : playerList)
    {
        if (player.name == name && player.name != "NONE")
        {
            return player;
        }
    }

    std::cerr << "ERROR: Player '" + name + "' not found!" << std::endl;
    return playerList[0];
}

bool playerExists(const std::string& name)
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

void printMenu()
{
    std::cout << "Choose an option:" << std::endl;
    std::cout << "1. Add a Player" << std::endl;
    std::cout << "2. Remove a Player" << std::endl;
    std::cout << "3. Enter Player Chip Amounts" << std::endl;
    std::cout << "4. Print Player Winnings" << std::endl;
    std::cout << "5. Set Pot" << std::endl;
    std::cout << "6. Exit & Save Player List" << std::endl;
}

void printPlayers()
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
            std::cout << playerList[i].name << std::endl << std::endl;
        }
    }
}

void printBanner()
{
    std::cout << "***********************************************" << std::endl;
    std::cout << "          POKER PAL by JACOB RIDGEWAY          " << std::endl;
    std::cout << "     A command-line based poker game tool.     " << std::endl;
    std::cout << "               Developed 3/25/24               " << std::endl;
    std::cout << "***********************************************" << std::endl;
    std::cout << std::endl;
}

int getIntegerInput(enum IntInputValidationOptions option)
{
    switch (option)
    {
    case MAIN_MENU:
    {
        int input;
        std::cin >> input;
        std::cout << std::endl;

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

void printChipAmounts(const Player& player)
{
    std::cout << "Total chip amounts for " << player.name << ": "
        << std::endl;
    std::cout << "White: " << player.whiteChips << " - $" << (player.whiteChips * WHITE_CHIP_VALUE) << std::endl;
    std::cout << "Red: " << player.redChips << " - $" << (player.redChips * RED_CHIP_VALUE) << std::endl;
    std::cout << "Blue: " << player.blueChips << " - $" << (player.blueChips * BLUE_CHIP_VALUE) << std::endl;
    std::cout << "Green: " << player.greenChips << " - $" << (player.greenChips * GREEN_CHIP_VALUE) << std::endl;
    std::cout << "Black: " << player.blackChips << " - $" << (player.blackChips * BLACK_CHIP_VALUE) << std::endl;
}

int main()
{
    std::cout << std::fixed << std::setprecision(2); // set floating point precision
    std::cerr << std::fixed << std::setprecision(2); // set floating point precision

    bool exit = false;
    float potAmount = 0.0f;
    bool playerListChanged = false;

    printBanner();

    while (playerList.size() > 1 && !exit)
    {
        printPlayers();
        printMenu();

        int menuChoice = getIntegerInput(MAIN_MENU);

        switch (menuChoice)
        {
        case 1: // Add player
        {
            std::cout << "Enter the name of the new player (no spaces): ";
            std::string newPlrName = getStringInput(ADD_PLAYER);

            Player newPlayer;
            newPlayer.name = newPlrName;
            playerList.push_back(newPlayer);
        	playerListChanged = true;

			std::cout << std::endl;
            break;
        }

        case 2: // Remove player
        {
            std::cout << "Enter the name of the player to remove: ";
            std::string plrToDel = getStringInput(REMOVE_PLAYER);

            if (playerList.size() == 2)
            { // must account for NONE player at index 0, hence the 2
                std::cerr << "ERROR: Must be at least one player!" << std::endl;
            }
            else
            {
                playerList.erase(playerList.begin() + getPlayerIndex(plrToDel));
                playerListChanged = true;
            }

			std::cout << std::endl;	
            break;
        }

        case 3: // Enter player chip amounts
        {
            std::cout << "Enter the name of the player to edit: ";
            std::string plrToEdit = getStringInput(EDIT_PLAYER_CHIPS);

            if (playerExists(plrToEdit))
            {
                Player& player = getPlayerReference(plrToEdit);

                for (int i = 0; i < 5; i++)
                {
                    std::cout << "Enter the number of " << CHIP_COLORS[i] << " chips: ";
                    int chipAmount = getIntegerInput(ENTER_CHIP_AMOUNTS);

                    switch (i)
                    {
                    case 0:
                        player.whiteChips = chipAmount;
                        break;
                    case 1:
                        player.redChips = chipAmount;
                        break;
                    case 2:
                        player.blueChips = chipAmount;
                        break;
                    case 3:
                        player.greenChips = chipAmount;
                        break;
                    case 4:
                        player.blackChips = chipAmount;
                        break;
                    }
                }

                std::cout << std::endl;
                printChipAmounts(player);
            }

			std::cout << std::endl;
            break;
        }

        case 4: // Display player winnings
        {
            float totalWinnings = 0.0f;

            for (int i = 1; i < playerList.size(); i++)
            {
                Player player = playerList[i];
                float playerWinnings = calculateWinnings(player);
                std::cout << player.name << ": $" << playerWinnings << std::endl;

                totalWinnings += playerWinnings;
            }

			std::cout << std::endl;

            if (potAmount == 0)
            {
                std::cerr << "WARNING: No pot amount is currently set." << std::endl;
            }
            else if (potAmount < totalWinnings)
            {
                float difference = totalWinnings - potAmount;
                std::cerr << "WARNING: Total winnings exceed the pot amount by $"
                    << difference << "! Ensure chips haven't been overcounted."
                    << std::endl;
            }
            else if (potAmount > totalWinnings)
            {
                float difference = potAmount - totalWinnings;
                std::cerr << "WARNING: Total winnings are less than the pot amount by $"
                    << difference << "! Ensure chips haven't been undercounted."
                    << std::endl;
            }

            std::cout << "Total pot amount: $" << potAmount << std::endl;

			std::cout << std::endl;
            break;
        }

        case 5: // Set pot amount
        {
            std::cout << "Choose an option for setting the pot." << std::endl;
            std::cout << "1. Default - Multiplies the number of players by 10.25." << std::endl;
            std::cout << "2. Custom - Specify a custom amount." << std::endl;

            int potChoice = getIntegerInput(SET_POT);

            switch (potChoice)
            {
            case 1:
            {
                potAmount = (playerList.size() - 1) * 10.25f;

                break;    
            }

            case 2:
            {
                std::cout << "Enter the desired pot amount (xx.xx): ";
                std::cin >> potAmount;

                while (std::cin.fail() || potAmount < 0)
                {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cerr << "ERROR: Please enter a valid pot amount: ";
                    std::cin >> potAmount;
                }

                break;
            }
            }

            std::cout << "Pot set to $" << potAmount << std::endl;
        	std::cout << std::endl;	
            break;
        }

        case 6: // Terminate program
        {
            exit = true;

            if (playerListChanged)
            {
                std::ofstream outFile("players.txt");

                for (int i = 1; i < playerList.size(); i++)
                {
                    if (i == 1)
                    {
                        outFile << playerList[i].name;
                    }
                    else
                    {
                        outFile << std::endl << playerList[i].name;
                    }
                }
            }

            break;
        }
        }
    }
}
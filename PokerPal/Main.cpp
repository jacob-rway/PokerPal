#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
// ReSharper disable All

struct Player
{
    std::string name;
    int whiteChips;
    int redChips;
    int blueChips;
    int greenChips;
    int blackChips;

    Player() : name("NONE"), whiteChips(0), redChips(0), blueChips(0), greenChips(0), blackChips(0) {}
};

float calculateWinnings(const Player& player)
{
    float winnings = 0;
    winnings += player.whiteChips * 0.01f;
    winnings += player.redChips * 0.05f;
    winnings += player.blueChips * 0.10f;
    winnings += player.greenChips * 0.25f;
    winnings += player.blackChips * 1.0f;

    return winnings;
}

std::vector<Player> loadPlayerList()
{
    std::ifstream inFile("C:\\players.txt");
    std::vector<Player> players;
    players.push_back(Player()); // default player, used for error handling

    if (!inFile.is_open())
    {
        std::cout << "Error opening file: Missing 'players.txt' file!" << std::endl;
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

Player getPlayer(const std::vector<Player>& players, const std::string& name)
{
    for (int i = 1; i < players.size(); i++)
    {
        if (players[i].name == name && players[i].name != "NONE")
        {
            return players[i];
        }
    }

    std::cout << "Player '" << name << "' not found!" << std::endl;
    return players[0];
}

int getPlayerIndex(const std::vector<Player>& players, const std::string& name)
{
    for (int i = 1; i < players.size(); i++)
    {
        if (players[i].name == name && players[i].name != "NONE")
        {
            return i;
        }
    }

    std::cout << "Player '" << name << "' not found!" << std::endl;
    return -1;
}

Player& getPlayerReference(std::vector<Player>& players, const std::string& name)
{
    for (Player& player : players)
    {
        if (player.name == name && player.name != "NONE")
        {
            return player;
        }
    }

    std::cout << "Player '" + name + "' not found!" << std::endl;
    return players[0];
}

bool playerExists(const std::vector<Player>& players, const std::string& name)
{
    for (int i = 1; i < players.size(); i++)
    {
        if (players[i].name == name && players[i].name != "NONE")
        {
            return true;
        }
    }

    return false;
}

void printMenu()
{
    std::cout << "Choose an option:" << std::endl;
    std::cout << "1. Add a player" << std::endl;
    std::cout << "2. Remove a player" << std::endl;
    std::cout << "3. Enter player chip amounts" << std::endl;
    std::cout << "4. Print player winnings" << std::endl;
    std::cout << "5. Set pot" << std::endl;
    std::cout << "6. Exit" << std::endl;
}

void printPlayers(std::vector<Player> playerList)
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

int main()
{
    std::vector<Player> playerList = loadPlayerList();
    bool exit = false;
    float potAmount = 0.0f;

    while (playerList.size() > 1 || !exit)
    {

        printPlayers(playerList);
        printMenu();
        int choice;
        std::cin >> choice;

        while (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << std::endl << "Please enter a valid menu option." << std::endl;

            printPlayers(playerList);
            printMenu();
            std::cin >> choice;
        }

        std::cout << std::endl;

        switch (choice)
        {
        case 1: // Add player
        {
            std::ofstream outFile("C:\\players.txt", std::ios::app);

            std::string newPlrName;
            std::cout << "Enter the name of the new player (no spaces): ";
            std::cin >> newPlrName;

            while (newPlrName == "NONE" || playerExists(playerList, newPlrName))
            {
                std::cout << "Name is not allowed or taken! Please enter a different name: ";
                std::cin >> newPlrName;
            }

            Player newPlayer;
            newPlayer.name = newPlrName;
            playerList.push_back(newPlayer);

            outFile << std::endl << newPlayer.name;
            outFile.close();

            break;
        }

        case 2: // Remove player
        {
            std::ofstream outFile("C:\\players.txt");

            std::string plrToDel;
            std::cout << "Enter the name of the player to remove: ";
            std::cin >> plrToDel;

            if (getPlayer(playerList, plrToDel).name != "NONE")
            {
                if (playerList.size() == 2)
                { // must account for NONE player at index 0, hence the 2
                    std::cout << "Must be at least one player!" << std::endl;
                }
                else
                {
                    playerList.erase(playerList.begin() + getPlayerIndex(playerList, plrToDel));
                }
            }

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

            break;
        }

        case 3: // Enter player chip amounts
        {
            std::string chipColors[] = { "white", "red", "blue", "green", "black" };

            std::string plrToEdit;
            std::cout << "Enter the name of the player to edit: ";
            std::cin >> plrToEdit;

            if (getPlayer(playerList, plrToEdit).name != "NONE")
            {
                Player& player = getPlayerReference(playerList, plrToEdit);

                for (int i = 0; i < 5; i++)
                {
                    std::cout << "Enter the number of " << chipColors[i] << " chips: ";
                    int chipAmount;
                    std::cin >> chipAmount;

                    while (std::cin.fail())
                    {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Enter the number of " << chipColors[i] << " chips: ";
                        std::cin >> chipAmount;
                    }

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
                    default:
                        std::cout << "Entered amount must be a valid whole number!";
                        break;
                    }
                }

                std::cout << std::endl;
                std::cout << "Total chip amounts for " << player.name << ": " << std::endl;
                std::cout << "White: " << player.whiteChips << std::endl;
                std::cout << "Red: " << player.redChips << std::endl;
                std::cout << "Blue: " << player.blueChips << std::endl;
                std::cout << "Green: " << player.greenChips << std::endl;
                std::cout << "Black: " << player.blackChips << std::endl;
            }

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

            if (potAmount == 0)
            {
                std::cout << "WARNING: No pot amount is currently set." << std::endl;
            }
            else if (potAmount < totalWinnings)
            {
                std::cout << "WARNING: Total winnings exceed the pot amount! Ensure chips haven't been overcounted." << std::endl;
            }
            else if (potAmount > totalWinnings)
            {
                std::cout << "WARNING: Total winnings are less than the pot amount! Ensure chips haven't been undercounted." << std::endl;
            }

            std::cout << "Total pot amount: $" << potAmount << std::endl;
            break;
        }

        case 5: // Set pot amount
        {
            bool validMenuChoice = false;

            std::cout << "Choose an option for setting the pot." << std::endl;
            std::cout << "1. Default - Multiplies the number of players by 10.25." << std::endl;
            std::cout << "2. Custom - Specify a custom amount." << std::endl;

            int potChoice;

            while (!validMenuChoice)
            {
                std::cin >> potChoice;
                std::cout << std::endl;

                while (std::cin.fail())
                {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Please enter a valid menu option." << std::endl;
                    std::cin >> potChoice;

                    std::cout << std::endl;
                }

                switch (potChoice)
                {
                case 1:
                {
                    potAmount = (playerList.size() - 1) * 10.25f;
                    std::cout << "Pot set to $" << potAmount << std::endl;
                    validMenuChoice = true;
                    break;
                }

                case 2:
                {
                    std::cout << "Enter the desired pot amount (xx.xx): ";
                    std::cin >> potAmount;

                    while (std::cin.fail())
                    {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << std::endl << "Please enter a valid pot amount." << std::endl;
                        std::cin >> potAmount;
                    }

                    std::cout << "Pot set to $" << potAmount << std::endl;
                    validMenuChoice = true;
                    break;
                }

                default:
                {
                    std::cout << "Please enter a valid menu option." << std::endl;
                    break;
                }
                }
            }

            break;
        }

        case 6: // Terminate program
        {
            exit = true;
            break;
        }

        default:
        {
            std::cout << "Please enter a valid menu option." << std::endl;
            break;
        }
        }
    }
}
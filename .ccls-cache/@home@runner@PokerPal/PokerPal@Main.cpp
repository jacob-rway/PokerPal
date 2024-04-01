#include "PokerPal.h"

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

			std::cout << '\n';
            break;
        }

        case 2: // Remove player
        {
            std::cout << "Enter the name of the player to remove: ";
            std::string plrToDel = getStringInput(REMOVE_PLAYER);

            if (playerList.size() == 2)
            { // must account for NONE player at index 0, hence the 2
                std::cerr << "ERROR: Must be at least one player!" << '\n';
            }
            else
            {
                playerList.erase(playerList.begin() + getPlayerIndex(plrToDel));
                playerListChanged = true;
            }

			std::cout << '\n';	
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

                std::cout << '\n';
                printChipAmounts(player);
            }

			std::cout << '\n';
            break;
        }

        case 4: // Display player winnings
        {
            float totalWinnings = 0.0f;

            for (int i = 1; i < playerList.size(); i++)
            {
                Player player = playerList[i];
                float playerWinnings = calculateWinnings(player);
                std::cout << player.name << ": $" << playerWinnings << '\n';

                totalWinnings += playerWinnings;
            }

			std::cout << '\n';

            if (potAmount == 0)
            {
                std::cerr << "WARNING: No pot amount is currently set." << '\n';
            }
            else if (potAmount < totalWinnings)
            {
                float difference = totalWinnings - potAmount;
                std::cerr << "WARNING: Total winnings exceed the pot amount by $"
                    << difference << "! Ensure chips haven't been overcounted."
                    << '\n';
            }
            else if (potAmount > totalWinnings)
            {
                float difference = potAmount - totalWinnings;
                std::cerr << "WARNING: Total winnings are less than the pot amount by $"
                    << difference << "! Ensure chips haven't been undercounted."
                    << '\n';
            }

            std::cout << "Total pot amount: $" << potAmount << '\n';

			std::cout << '\n';
            break;
        }

        case 5: // Set pot amount
        {
            std::cout << "Choose an option for setting the pot." << '\n';
            std::cout << "1. Default - Multiplies the number of players by 10.25." << '\n';
            std::cout << "2. Custom - Specify a custom amount." << '\n';

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

            std::cout << "Pot set to $" << potAmount << '\n';
        	std::cout << '\n';	
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
                        outFile << '\n' << playerList[i].name;
                    }
                }
            }

            break;
        }
        }
    }
}
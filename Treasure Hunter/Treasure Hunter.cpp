#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <print>
#include <limits>

#include "Map.h"
#include "Character.h"

const int digCount{ 25 };

/**
 * @brief Builds the first map.
 * @version 1.0
 * @author Chay Hawk
 *
 * Just a test map to test out important features.
 *
 * @param map
 */
void GoldenHills(Map& map)
{
    map.EditTileRange(0, 0, 1, 4, ' ', false, false, false, false, false); //Clears the room of ground
    map.EditTileRange(0, 6, 1, 4, ' ', false, false, false, false, false); //Clears the room of ground

    map.EditTile(2, 0, '#', true, false, false, false, false);
    map.EditTile(2, 1, '#', true, false, false, false, false);
    map.EditTile(2, 2, '#', true, false, false, false, false);
    map.EditTile(2, 3, '#', true, false, false, false, false);
    map.EditTile(2, 4, '_', true, true, false, true); //Door
    map.EditTile(2, 5, '#', true, false, false, false, false);
    map.EditTile(2, 6, '#', true, false, false, false, false);
    map.EditTile(2, 7, '#', true, false, false, false, false);
    map.EditTile(2, 8, '#', true, false, false, false, false);
    map.EditTile(2, 9, '#', true, false, false, false, false);
    map.EditTile(2, 10, '#', true, false, false, false, false);
    map.EditTile(2, 11, '#', true, false, false, false, false);
    map.EditTile(1, 11, '#', true, false, false, false, false);
    map.EditTile(0, 11, '#', true, false, false, false, false);

    //Scenery
    map.EditTile(7, 11, '@', true, false, false, false, false);
    map.EditTile(8, 12, '@', true, false, false, false, false);
    map.EditTile(8, 10, '@', true, false, false, false, false);

    //Treasure
    map.EditTile(8, 11, '.', false, false, false, false, true);

    map.EditTile(1, 5, '#', true, false, false, false, false);
    map.EditTile(0, 5, '|', true, true, false, true, false); //Door
    map.EditTile(0, 2, '*', true, false, false, false, false); //NPC
}

/**
 * @brief Dialogue ID Tags
 * @version 1.0
 * @author Chay Hawk
 *
 * These are used to call specific dialogue options.
 *
 */
const enum class DialogueID
{
    NPC_01_Dialogue_01,
    NPC_01_Dialogue_02
};

struct Flags
{
    bool talkedToProspector{ false };
    bool canDig{ false };
};

/**
 * @brief Show dialogue
 * @version 1.0
 * @author Chay Hawk
 *
 * This function outputs the dialogue based on the ID given to it.
 *
 * @param ID
 */
void Dialogue(const DialogueID& ID, Flags& flags, Character& character)
{
    switch (ID)
    {
        case DialogueID::NPC_01_Dialogue_01:
        {
            std::print("Hey there fella, my names prospector Pete!\n");
            std::print("Would you like to go on a treasure hunting adventure fella?!\n");
            
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::print("1. Sure, I'd love to!\n");
            std::print("2. Sorry Pete\n");
            int input{};

            std::cin >> input;

            if (input == 1)
            {
                std::print("Thats the spirit! So heres how the game works, you got {} attempts to dig up some\n", digCount);
                std::print("good loot, but not all the areas out there have treasure, you'll just have to use your wits!\n");
                std::print("Heres a  shovel pal, good luck out there!\n");
                flags.canDig = true;
                character.SetDigCount(digCount);

            }
            else if (input == 2)
            {
                std::print("Well thats a darn shame fella, it's almost like it was pointless to talk to me.\n");
            }
            else
            {
                std::print("I cant understand you fella!\n");
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        break;
    }
}


void Intro()
{
    std::print("{:^39}\n", "TREASURE HUNTER");
    std::print("{:^39}\n", "By");
    std::print("{:^39}\n", "Chay Hawk\n");
}


void UI(int score, int y, int x, const Character& character)
{
    std::print("{:=>39}\n", "");  // Top border
    std::print(" Score:      [ {:^8} ]   Y: [ {:^4} ]\n", score, y);
    std::print(" Direction:  [ {:^8} ]   X: [ {:^4} ]\n", character.GetDirection(), x);
    std::print(" Digs Left:  [ {:^8} ]\n", character.GetDigsLeft());
    std::print("{:=>39}\n", "");  // Bottom border
}


int main()
{
    Map theGoldenHills("The Golden Hills", 20, 20, '.');
    theGoldenHills.Initialize();
    GoldenHills(theGoldenHills);
    Character player("Player", 'O', 4, 4);

    Flags flags;

    Intro();

    bool isGameOver = false;

    while (!isGameOver)
    {
        theGoldenHills.SetEntityAt(player.GetY(), player.GetX(), player.GetSprite());
        theGoldenHills.Draw();

        std::print("{:^39}\n", "TREASURE HUNTER - 0.2.0");
        UI(player.GetScore(), player.GetY(), player.GetX(), player);

        std::print("\nMove (W/A/S/D) or Interact (E): \n\n");

        char input;
        std::cin >> input;
        input = std::tolower(input);

        //NOTE
        //Interactions are being hard coded for now, but later i need to find a way to package everything 
        //together so the game takes care of all that for me. For example, placing an entity and it easily
        //allowing me to attach another entity to it to trigger it, for example, a switch triggering a
        //remote door.
        if (input == 'e')
        {
            if (player.GetY() == 3 && player.GetX() == 4 || player.GetY() == 1 && player.GetX() == 4)
            {
                theGoldenHills.Toggle(2, 4, '_', ' ', true, false);
            }
            else if (player.GetY() == 0 && player.GetX() == 4 || player.GetY() == 0 && player.GetX() == 6)
            {
                theGoldenHills.Toggle(0, 5, '|', ' ', true, false);
            }
            else if (player.GetY() == 1 && player.GetX() == 2)
            {
                Dialogue(DialogueID::NPC_01_Dialogue_01, flags, player);
            }
        }
        else if (input == 'f')
        {
            if (flags.canDig == true)
            {
                if (player.GetDigsLeft() == 0)
                {
                    std::print("You ran out of digs, you earned {} score!\n", player.GetScore());
                    std::print("Game Over\n");

                    return 0;
                }
              
                player.Dig(theGoldenHills);
            }
            else
            {
                std::print("I don't have a shovel!\n");
            }
        }
        //Example
        else if (input == 'r')
        {
            theGoldenHills.ModifyLayer([](Map::Tile& tile)
                {
                    tile.SetBaseTile('.');
                });
        }
        else
        {
            player.Move(input, theGoldenHills);
        }
    }

    return 0;
}
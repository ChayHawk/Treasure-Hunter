#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <print>

#include "Map.h"
#include "Character.h"



/**
 * @brief Builds the first map.
 * @version 1.0
 * @author Chay Hawk
 *
 * Just a test map to test out important features.
 *
 * @param map
 */
void TestMap(Map& map)
{
    map.EditTileRange(0, 0, 1, 4, ' ', false, false, false, false); //Clears the room of ground
    map.EditTileRange(0, 6, 1, 4, ' ', false, false, false, false); //Clears the room of ground

    map.EditTile(2, 0, '#', true);
    map.EditTile(2, 1, '#', true, false, false, true);
    map.EditTile(2, 2, '#', true);
    map.EditTile(2, 3, '#', true);
    map.EditTile(2, 4, '_', true, true, false, true); //Door
    map.EditTile(2, 5, '#', true);
    map.EditTile(2, 6, '#', true);
    map.EditTile(2, 7, '#', true);
    map.EditTile(2, 8, '#', true);
    map.EditTile(2, 9, '#', true);
    map.EditTile(2, 10, '#', true);
    map.EditTile(2, 11, '#', true);
    map.EditTile(1, 11, '#', true);
    map.EditTile(0, 11, '#', true);

    //Scenery
    map.EditTile(7, 11, '@', true);
    map.EditTile(8, 12, '@', true);
    map.EditTile(8, 10, '@', true);


    map.EditTile(1, 5, '#', true);
    map.EditTile(0, 5, '|', true, true, false, true); //Door
    map.EditTile(0, 2, '*', true); //NPC
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

/**
 * @brief Show dialogue
 * @version 1.0
 * @author Chay Hawk
 *
 * This function outputs the dialogue based on the ID given to it.
 *
 * @param ID
 */
void Dialogue(const DialogueID& ID)
{
    switch (ID)
    {
        case DialogueID::NPC_01_Dialogue_01:
        {
            std::print("Hey there fella, my names prospector pete!\n");
            std::print("Would you like to go on a treasure hunting adventure?!\n");
        }
        break;
    }
}


void Intro()
{
    std::print("{:^39}\n", "TREASURE HUNTER");
    std::print("{:^39}\n", "By");
    std::print("{:^39}\n", "Chay Hawk");
}


void UI(int score, int money, int y, int x, const Character& character)
{
    std::print("{:=>39}\n", "");
    std::print(" Score: [ {:^8} ]        Y: [ {:^4} ]\n", score, y);
    std::print(" Money: [ {:^8} ]        X: [ {:^4} ]\n", money, x);
    std::print(" Direction: [ {:^4} ]\n", character.GetDirection());
    std::print(" Digs Left: [ {:^4}]\n", character.GetDigsLeft());
    std::print("{:<4}\n", "");
    std::print("{:=>39}\n", "");
}


int main()
{
    Map theGoldenHills("Test Map", 20, 20, '.');
    theGoldenHills.Initialize();
    TestMap(theGoldenHills);

    Character player("Player", 'O', 4, 4);

    Intro();

    bool isGameOver = false;

    while (!isGameOver)
    {
        theGoldenHills.SetEntityAt(player.GetY(), player.GetX(), player.GetSprite());
        theGoldenHills.Draw();

        int money{ 100 }; //Temporary
        std::print("{:^39}\n", "TREASURE HUNTER - 0.1.0");
        UI(player.GetScore(), money, player.GetY(), player.GetX(), player);

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
                Dialogue(DialogueID::NPC_01_Dialogue_01);
            }
        }
        else if (input == 'f')
        {
            player.Dig(theGoldenHills);
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
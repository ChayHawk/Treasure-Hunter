#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <print>

#include "Map.h"
#include "Character.h"

void TestMap(Map& map)
{
    map.ModifyTile(2, 0, '_', true);

    map.ModifyTile(0, 0, ' ', false, false, false, true);
    map.ModifyTile(0, 1, ' ', false, false, false, true);
    map.ModifyTile(0, 4, ' ', false, false, false, true);
    map.ModifyTile(1, 0, ' ', false, false, false, true);
    map.ModifyTile(1, 1, ' ', false, false, false, true);
    map.ModifyTile(1, 2, ' ', false, false, false, true);

    map.ModifyTile(2, 1, '_', true);
    map.ModifyTile(2, 2, '_', true);
    map.ModifyTile(2, 3, '#', true);
    map.ModifyTile(2, 4, '=', true, true, false, false, true); //Door
    map.ModifyTile(2, 5, '#', true);
    map.ModifyTile(2, 6, '_', true);
    map.ModifyTile(2, 7, '_', true);
    map.ModifyTile(2, 8, '_', true);
    map.ModifyTile(2, 9, '_', true);
    map.ModifyTile(2, 10, '_', true);
    map.ModifyTile(2, 11, '|', true);
    map.ModifyTile(1, 11, '|', true);
    map.ModifyTile(0, 11, '|', true);

    map.ModifyTile(1, 5, '|', true);
    map.ModifyTile(0, 5, 'I', true, true, false, false, true); //Door

    map.ModifyTile(0, 2, '*', true); //NPC
}

const enum class DialogueID
{
    NPC_01_Dialogue_01
};

void Dialogue(const DialogueID& ID)
{
    switch (ID)
    {
        //Should put the contents of this case in a function then call it here to lessen clutter
        case DialogueID::NPC_01_Dialogue_01:
        {
            std::cout << "Hey there guy, would you like to see what im selling?\n";

            int choice{ 0 };

            std::cout << "1.) Sure, why not?\n";
            std::cout << "2.) I'll pass\n";

            std::cin >> choice;

            if (choice == 1)
            {
                std::cout << "Welcome to the shop, what would you like to buy?\n";
            }
            else if(choice == 2)
            {
                std::cout << "Come back if ya change yer mind.\n";
                return;
            }
        }
        break;
    }
}

int main()
{
    Map myMap("Test Map", 20, 20, '.');
    myMap.Initialize();
    TestMap(myMap);
    myMap.RescanMap();

    Character player("Hero", '*', 4, 4);

    bool isGameOver = false;

    while (!isGameOver)
    {
        myMap.SetEntityAt(player.GetY(), player.GetX(), player.GetSprite());
        myMap.Draw();

        std::cout << "\nMove (W/A/S/D) or Interact (E): ";
        std::cout << "\n\n";
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
            if (player.GetY() == 3 && player.GetX() == 3)
            {
                myMap.Toggle(2, 4, '=', ' ', true, false);
            }
            else if (player.GetY() == 0 && player.GetX() == 4 || player.GetY() == 0 && player.GetX() == 6)
            {
                myMap.Toggle(0, 5, 'I', ' ', true, false);
            }
            else if (player.GetY() == 1 && player.GetX() == 2)
            {
                Dialogue(DialogueID::NPC_01_Dialogue_01);
            }
        }
        else if (input == 'f')
        {
            player.Dig(player.GetY(), player.GetX(), myMap);
        }
        //Example
        else if (input == 'r')
        {
            myMap.ModifyLayer([](Map::Tile& tile)
                {
                    tile.SetBaseTile('k');
                });
        }
        else
        {
            player.Move(input, myMap);
        }
    }

    return 0;
}
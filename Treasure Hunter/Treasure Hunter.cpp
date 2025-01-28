#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

class Map
{
    public:
        Map(const std::string& name, int height, int width, char tile)
            : mName(name), mHeight(height), mWidth(width), mTile(tile) {
        }

        void Initialize()
        {
            mMap.assign(mHeight, std::vector<Tile>(mWidth, { mTile, mTile, false, false, 0, false }));

            //Assign ID to tiles. Could maybe make this its own function.
            int idCounter{ 100 };
            for (size_t column = 0; column < mHeight; ++column)
            {
                for (size_t row = 0; row < mWidth; ++row)
                {
                    mMap[column][row].tileID = ++idCounter; //Set tile ID
                    mMap[column][row].masterTile = mMap[column][row].baseTile; //Set tile state, this allows for the tile to be reset if modified
                }
            }
        }

        void Draw()
        {
            for (size_t column = 0; column < mHeight; ++column)
            {
                for (size_t row = 0; row < mWidth; ++row)
                {
                    if (mMap[column][row].doNotRedraw == true)
                    {
                        mMap[column][row].baseTile = ' ';
                    }
                    
                    if (mMap[column][row].resetTileState == true)
                    {
                        mMap[column][row].baseTile = mMap[column][row].masterTile;
                    }

                    std::cout << mMap[column][row].baseTile << ' ';
                }
                std::cout << '\n';
            }
        }

        //This is needed to add modifications made to the base map
        void RescanMap()
        {
            for (size_t column = 0; column < mHeight; ++column)
            {
                for (size_t row = 0; row < mWidth; ++row)
                {
                    if (!mMap[column][row].isPersistent)
                    {
                        mMap[column][row].masterTile = mMap[column][row].baseTile;
                    }
                }
            }
        }


        //Rearrange the initializer list to put more important things first.
        void ModifyTile
        (
            int y, int x, char newTile = '\0', bool collisionState = false, bool interactionState = false,
            bool doNotRedraw = false, bool resetTileState = false, bool isPersistent = false // New parameter
        )
        {
            if (!IsInBounds(y, x))
            {
                std::cout << "Coordinates are out of bounds!\n";
                return;
            }

            if (newTile != '\0')
            {
                mMap[y][x].baseTile = newTile;
            }

            mMap[y][x].collisionState = collisionState;
            mMap[y][x].interactionState = interactionState;
            mMap[y][x].doNotRedraw = doNotRedraw;
            mMap[y][x].resetTileState = resetTileState;
            mMap[y][x].isPersistent = isPersistent; // Set persistence
        }


        bool GetInteractionState(int y, int x) const
        {
            return mMap[y][x].interactionState;
        }

        bool GetCollisionState(int y, int x) const
        {
            return mMap[y][x].collisionState;
        }

        int GetHeight() const 
        { 
            return mHeight; 
        }

        int GetWidth() const 
        { 
            return mWidth; 
        }

        int GetTileID(int y, int x) const
        {
            return mMap[y][x].tileID;
        }

        bool GetDoNotRedrawState(int y, int x) const
        {
            return mMap[y][x].doNotRedraw;
        }

        void SetNewTile(int y, int x, char newTile)
        {
            mMap[y][x].baseTile = newTile;
        }

        char GetTile(int y, int x) const
        {
            if (IsInBounds(y, x))
            {
                return mMap[y][x].baseTile;
            }
            else
            {
                std::cout << "Out of bounds\n";
            }
        }

        void Toggle(int y, int x, char onTile, char offTile, bool collisionOnState, bool collisionOffState)
        {
            if (GetInteractionState(y, x))
            {
                // Currently "on", so toggle to "off"
                ModifyTile(y, x, offTile, collisionOffState, false);
            }
            else
            {
                // Currently "off", so toggle to "on"
                ModifyTile(y, x, onTile, collisionOnState, true);
            }
        }

       
        void ResetTileState(int y, int x)
        {
            if (!mMap[y][x].isPersistent)
            {
                mMap[y][x].baseTile = mMap[y][x].masterTile;
            }
        }


    private:
        struct Tile
        {
            char baseTile{};
            char masterTile{};
            bool resetTileState{ false };
            bool collisionState{ false };
            bool interactionState{ false };
            int interactCount{ 0 };
            bool isCollectible{ false };
            int tileID{};
            bool doNotRedraw{ false };
            bool isPersistent{ false };
        };

        std::string mName{};
        int mHeight{};
        int mWidth{};
        char mTile{};
        std::vector<std::vector<Tile>> mMap;

        bool IsInBounds(int y, int x) const
        {
            return y >= 0 && y < mHeight && x >= 0 && x < mWidth;
        }
};

class Character
{
    public:
        Character(const std::string& name, char sprite, int y, int x)
            : mName(name), mSprite(sprite), mPosY(y), mPosX(x)
        {}
                                                   
        void Move(char direction, Map& map)
        {
            int nextY = mPosY;
            int nextX = mPosX;

            switch (direction)
            {
                case 'W':
                case 'w':
                    --nextY;
                    break;

                case 'S':
                case 's':
                    ++nextY;
                    break;

                case 'A':
                case 'a':
                    --nextX;
                    break;

                case 'D':
                case 'd':
                    ++nextX;
                    break;

                default:
                    std::cout << "Invalid choice!\n";
                    return;
            }

            int lastY{ mPosY };
            int lastX{ mPosX };

            if (IsMoveValid(nextY, nextX, map))
            {
                mPosY = nextY;
                mPosX = nextX;

                map.ResetTileState(lastY, lastX);
            }
            else
            {
                std::cout << "Movement blocked!\n";
            }
        }

        int GetY() const { return mPosY; }
        int GetX() const { return mPosX; }
        char GetSprite() const { return mSprite; }

        //Need to find a way to stop player from digging into collidable objects.
        void Dig(int y, int x, Map& map)
        {
            if (map.GetCollisionState(y - 1, x))
            {
                std::cout << "You can't dig here! It's collidable.\n";
                return;
            }

            map.ModifyTile(y - 1, x, ' ', false, false, false, false, true); // Mark as persistent
            std::cout << "You dug up the spot!\n";
        }

    private:
        std::string mName{};
        char mSprite{};
        int mPosY{};
        int mPosX{};
        char mPreviousTile{};
        int mDigsLeft{};

        std::vector<std::string> mInventory;

        bool IsMoveValid(int y, int x, const Map& map) const
        {
            return y >= 0 && y < map.GetHeight() && x >= 0 && x < map.GetWidth() && !map.GetCollisionState(y, x);
        }
};

void TestMap(Map& map)
{
    map.ModifyTile(2, 0, '_', true);
    map.ModifyTile(2, 1, '_', true);
    map.ModifyTile(2, 2, '_', true);
    map.ModifyTile(2, 3, '#', true);
    map.ModifyTile(2, 4, ' ', true, false, false, false, true); //Door
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
    map.ModifyTile(0, 5, 'I', true, false, false, false, true); //Door

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
        myMap.ModifyTile(player.GetY(), player.GetX(), player.GetSprite());
        myMap.Draw();

        std::cout << "\nMove (W/A/S/D) or Interact (E): ";
        std::cout << "\n\n";
        char input;
        std::cin >> input;

        //NOTE
        //Interactions are being hard coded for now, but later i need to find a way to package everything 
        //together so the game takes care of all that for me. For example, placing an entity and it easily
        //allowing me to attach another entity to it to trigger it, for example, a switch triggering a
        //remote door.
        if (input == 'E' || input == 'e')
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
        else if (input == 'F' || input == 'f')
        {
            player.Dig(player.GetY(), player.GetX(), myMap);
        }
        else
        {
            player.Move(input, myMap);
        }
    }

    return 0;
}
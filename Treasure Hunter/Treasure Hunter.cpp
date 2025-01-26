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
        mMap.assign(mHeight, std::vector<Tile>(mWidth, { mTile, false, false, 0, false }));

        int idCounter{ 100 };
        for (size_t column = 0; column < mHeight; ++column)
        {
            for (size_t row = 0; row < mWidth; ++row)
            {
                mMap[column][row].tileID = ++idCounter;
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
                    mMap[column][row].tile = ' ';
                }

                std::cout << mMap[column][row].tile << ' ';
            }
            std::cout << '\n';
        }
    }

    void ModifyTile(int y, int x, char newTile = '\0', bool collisionState = false, bool interactionState = false, int interactCount = 0, bool isCollectible = false, bool doNotRedraw = false)
    {
        if (!IsInBounds(y, x))
        {
            std::cout << "Coordinates are out of bounds!\n";
            return;
        }

        if (newTile != '\0')
        {
            mMap[y][x].tile = newTile;
        }

        mMap[y][x].collisionState = collisionState;
        mMap[y][x].interactionState = interactionState;
        mMap[y][x].doNotRedraw = doNotRedraw;
    }

    bool GetInteractionState(int y, int x) const
    {
        return mMap[y][x].interactionState;
    }

    bool GetCollisionState(int y, int x) const
    {
        return mMap[y][x].collisionState;
    }

    int GetHeight() const { return mHeight; }
    int GetWidth() const { return mWidth; }

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
        mMap[y][x].tile = newTile;
    }

    char GetTile(int y, int x) const
    {
        if (IsInBounds(y, x))
        {
            return mMap[y][x].tile;
        }
        else
        {
            std::cout << "Out of bounds\n";
        }
    }

private:
    struct Tile
    {
        char tile{};
        bool collisionState{ false };
        bool interactionState{ false };
        int interactCount{ 0 };
        bool isCollectible{ false };
        int tileID{};
        bool doNotRedraw{ false };
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
        {
        }

        //TODO
        //Implement a system that redraws the tiles original tile after the player moves.

        void Move(char direction, Map& map)
        {
            int nextY{ mPosY };
            int nextX{ mPosX };

            char nextTile{};

            switch (direction)
            {
                case 'W':
                case 'w':
                    --nextY;
                    nextTile = map.GetTile(nextY, nextX);
                    std::cout << "Next Tile W: " << nextTile << '\n';
                    break;
                case 'S':
                case 's':
                    ++nextY;
                    nextTile = map.GetTile(nextY, nextX);
                    std::cout << "Next Tile S: " << nextTile << '\n';
                    break;
                case 'A':
                case 'a':
                    --nextX;
                    nextTile = map.GetTile(nextY, nextX);
                    std::cout << "Next Tile A: " << nextTile << '\n';
                    break;
                case 'D':
                case 'd':
                    ++nextX;
                    nextTile = map.GetTile(nextY, nextX);
                    break;
                default:
                    std::cout << "Invalid choice!\n";
                    std::cout << "Next Tile D: " << nextTile << '\n';
                    return;
            }

            int lastY{ mPosY };
            int lastX{ mPosX };

            std::cout << "Next Tile: " << nextTile << '\n';
            std::cout << "Current Y: " << mPosY << '\n';
            std::cout << "Current X: " << mPosX << '\n';
            std::cout << "Last Y: " << lastY << '\n';
            std::cout << "Last X: " << lastX << '\n';

            if (IsMoveValid(nextY, nextX, map))
            {
                mPosY = nextY;
                mPosX = nextX;
                map.ModifyTile(lastY, lastX, nextTile);
            }
            else
            {
                std::cout << "Movement blocked!\n";
            }
        }

        int GetY() const { return mPosY; }
        int GetX() const { return mPosX; }
        char GetSprite() const { return mSprite; }

        void AddToInventory(const std::string& itemName, int amount)
        {

        }

        void Dig(int y, int x, Map& map)
        {
            //I set y to -1 to test if its actually modifying the tile, it can be set back when debugging is complete. 
            map.ModifyTile(y - 1, x, ' ', false, true, 0, false, true);
        }

    private:
        std::string mName{};
        char mSprite{};
        int mPosY{};
        int mPosX{};

        std::vector<std::string> mInventory;

        bool IsMoveValid(int y, int x, const Map& map) const
        {
            return y >= 0 && y < map.GetHeight() && x >= 0 && x < map.GetWidth() && !map.GetCollisionState(y, x);
        }
};

void TestMap(Map& map)
{
    map.ModifyTile(2, 0, '_', true, false, 0, false, true);
    map.ModifyTile(2, 1, '_', true, false, 0, false, true);
    map.ModifyTile(2, 2, '_', true, false, 0, false, true);
    map.ModifyTile(2, 3, '#', true, false, 0, false, true);
    map.ModifyTile(2, 4, '_', true);
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
    map.ModifyTile(0, 5, 'I', true);

    map.ModifyTile(0, 2, '*', true);
}


//Need to implement a system where individual objects can be made and placed on the map, and they will act independantly
//of each other, so for example say i place 2 doors and 2 switches, the doors will be tied to their respective
//switches via the tile ID. There should be a function for each object that can take a y and x param, a tileID and
//any other information. 

void Toggle(int y, int x, char onTile, char offTile, bool collisionOnState, bool collisionOffState, Map& map)
{
    if (map.GetInteractionState(y, x))
    {
        // Currently "on", so toggle to "off"
        map.ModifyTile(y, x, offTile, collisionOffState, false);
    }
    else
    {
        // Currently "off", so toggle to "on"
        map.ModifyTile(y, x, onTile, collisionOnState, true);
    }
}

//KEY
//NPC = Type
//_01 = after NPC this is the code for the character itself

const enum class DialogueID
{
    NPC_01_Dialogue_01,
};

void Dialogue(const DialogueID& ID)
{
    switch (ID)
    {
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

    Character player("Hero", '*', 4, 4);

    bool isGameOver = false;

    while (!isGameOver)
    {
        myMap.ModifyTile(player.GetY(), player.GetX(), player.GetSprite());
        myMap.Draw();

        std::cout << "\nMove (W/A/S/D) or Interact (E): ";
        char input;
        std::cin >> input;

        //NOTE
        //Interactions are being hard coded for now, but later i need to find a way to package everything 
        //together so the game takes care of all that for me. For example, placing an entity and it easily
        //allowing me to attach another entity to it to trigger it, for example, a switch triggering a
        //remote door.
        if (input == 'E' || input == 'e')
        {
            player.Dig(player.GetY(), player.GetX(), myMap);

            if (player.GetY() == 3 && player.GetX() == 3)
            {
                Toggle(2, 4, '=', ' ', true, false, myMap);
            }
            else if (player.GetY() == 0 && player.GetX() == 4 || player.GetY() == 0 && player.GetX() == 6)
            {
                Toggle(0, 5, 'I', ' ', true, false, myMap);
            }
            else if (player.GetY() == 1 && player.GetX() == 2)
            {
                Dialogue(DialogueID::NPC_01_Dialogue_01);
            }
        }
        else
        {
            player.Move(input, myMap);
        }
    }

    return 0;
}

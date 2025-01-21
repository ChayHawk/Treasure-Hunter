#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>

//TODO

//ONE
//Need to implement dig system. I was thinking to add a check to see if the space has been dug
//Then create a new map to store dig locations, then replace the current map tiles with the dug 
//ones. I can make a new map of all the X spots then replace current map tiles with them.
//I might need to find a way to stop the map from drawing over the x's without leaving a
//player trail.

//TWO
//Implement a system where the player has to follow clues to find treasure, and they have only
//a certain amoiunt of digs they can use before the game is over.

class Map
{
    public:
        Map(const std::string& name, int height, int width, char tile)
            : mName(name), mHeight(height), mWidth(width), mTile(tile)
        {
        }

        void Initialize()
        {
            mMap.assign(mHeight, std::vector<char>(mWidth, mTile));
            mCollidable.assign(mHeight, std::vector<bool>(mWidth, false)); // Initialize collidable map
            mDugSpots.assign(mHeight, std::vector<bool>(mWidth, false));
        }

        void DrawMap()
        {
            for (size_t y = 0; y < mHeight; ++y)
            {
                for (size_t x = 0; x < mWidth; ++x)
                {
                    if (mDugSpots[y][x] == true)
                    {
                        // Display 'X' if the spot is dug
                        std::cout << 'x' << ' ';
                    }
                    else
                    {
                        // Otherwise, display the regular map tile
                        std::cout << mMap[y][x] << ' ';
                    }
                }
                std::cout << '\n';
            }
        }

        void PlaceOnMap(int y, int x, char object, bool collidable = false)
        {
            mMap[y][x] = object;
            if (collidable == false)
            {
                mCollidable[y][x] = true;
            }
        }

        void MarkAsDug(int y, int x)
        {
            mDugSpots[y][x] = true;
        }

        bool IsDug(int y, int x) const
        {
            return mDugSpots[y][x];
        }

        int GetHeight() const
        {
            return mHeight;
        }

        int GetWidth() const
        {
            return mWidth;
        }

        bool IsCollidable(int y, int x) const
        {
            return mCollidable[y][x];
        }

        char GetTile() const
        {
            return mTile;
        }

    private:
        std::string mName{ "Map Name" };
        int mHeight{ 10 };
        int mWidth{ 10 };
        char mTile{ '.' };

        std::vector<std::vector<char>> mMap;
        std::vector<std::vector<bool>> mCollidable;
        std::vector<std::vector<bool>> mDugSpots;
};


class Treasure
{
    public:
        Treasure(const std::string name, int value) : mName(name), mValue(value)
        { }

        std::string GetName() const
        {
            return mName;
        }

        int GetValue() const
        {
            return mValue;
        }

    private:
        std::string mName{};
        int mValue{};
};


class Character
{
    public:
        Character(const std::string& name, char sprite) : mName(name), mSprite(sprite)
        {
        }

        std::string GetName() const
        {
            return mName;
        }

        char GetSprite() const
        {
            return mSprite;
        }

        int GetYCoordinates() const
        {
            return mYPos;
        }

        int GetXCoordinates() const
        {
            return mXPos;
        }

        void SetCoordinates(int y, int x)
        {
            mYPos = y;
            mXPos = x;
        }

        int GetScore() const
        {
            return mScore;
        }

        void GiveScore(int amount)
        {
            if (amount > 0)
            {
                mScore += amount;
            }
        }

        void TakeTreasure(const Treasure& treasure, int amount)
        {
            auto findDuplicate = [&treasure](const std::pair<Treasure, int>& a)
                {
                    return treasure.GetName() == a.first.GetName();
                };

            auto itr = std::find_if(mTreasureList.begin(), mTreasureList.end(), findDuplicate);

            if (itr != mTreasureList.end())
            {
                itr->second += amount;
            }
            else
            {
                mTreasureList.push_back(std::make_pair(treasure, amount));
            }
        }

        void ViewTreasure() const
        {
            if (mTreasureList.empty())
            {
                std::cout << "I don't have any treasure yet.\n";
                return;
            }

            for (auto& i : mTreasureList)
            {
                std::cout << i.first.GetName() << "x(" << i.second << ")\n";
            }
        }

        void Dig(Map& map)
        {
            int y = GetYCoordinates();
            int x = GetXCoordinates();

            if (!map.IsDug(y, x))
            {
                map.MarkAsDug(y, x);
                std::cout << "You dug a hole at (" << y << ", " << x << ").\n";
            }
            else
            {
                std::cout << "This spot has already been dug.\n";
            }
        }

    private:
        std::string mName{ "Character Name" };
        char mSprite{};
        int mYPos{ 0 };
        int mXPos{ 0 };

        std::vector<std::pair<Treasure, int>> mTreasureList{};
};

class Collision
{
    public:
        bool IsMoveValid(Character& character, const Map& map, char direction)
        {
            int assesNextMoveY = character.GetYCoordinates();
            int assesNextMoveX = character.GetXCoordinates();

            // Predict the new coordinates
            switch (direction)
            {
                case 'W': case 'w': 
                    assesNextMoveY--;
                    break;

                case 'S': case 's': 
                    assesNextMoveY++;
                    break;

                case 'A': case 'a': 
                    assesNextMoveX--;
                    break;

                case 'D': case 'd': 
                    assesNextMoveX++;
                    break;

                default:
                    std::cout << "Invalid direction.\n";
                    return false;
            }

            if (!IsWithinMapBounds(assesNextMoveY, assesNextMoveX, map))
            {
                return false;
            }

            if (IsCollidableObject(assesNextMoveY, assesNextMoveX, map))
            {
                return false;
            }

            return true;
        }

    private:
        bool IsWithinMapBounds(int proposedY, int proposedX, const Map& map)
        {
            return proposedY >= 0 && proposedY < map.GetHeight() &&
                proposedX >= 0 && proposedX < map.GetWidth();
        }

        bool IsCollidableObject(int proposedY, int proposedX, const Map& map)
        {
            return map.IsCollidable(proposedY, proposedX);
        }
};

struct Game
{
    int score{ 0 };
    int digAttemptsLeft{ 10 };
};


int main()
{
    Map start("Starting Area", 10, 10, '.');
    Character hero("Master Cheeze", '*');
    Collision collision;

    start.Initialize();

    // Add some collidable objects
    start.PlaceOnMap(2, 2, 'T', true); // Tree at (2,2)
    start.PlaceOnMap(5, 5, 'R', true); // Rock at (5,5)
    start.PlaceOnMap(6, 6, 'Z', true);

    bool isGameOver{ false };

    while (!isGameOver)
    {
        std::cout << "Score: " << hero.GetScore() << "\n\n";

        int lastY{ hero.GetYCoordinates() };
        int lastX{ hero.GetXCoordinates() };

        start.PlaceOnMap(hero.GetYCoordinates(), hero.GetXCoordinates(), hero.GetSprite());

        start.DrawMap();

        std::cout << '\n';
        std::cout << "W). Up\n";
        std::cout << "S). Down\n";
        std::cout << "A). Left\n";
        std::cout << "D). Right\n\n";

        std::cout << "I). View Treasure\n";
        std::cout << "X). Dig\n";
        std::cout << ">";

        char selection{ ' ' };
        std::cin >> selection;

        if (selection == 'I' || selection == 'i')
        {
            hero.ViewTreasure();
        }
        else if (selection == 'X' || selection == 'x')
        {
            hero.Dig(start);
        }
        else if(collision.IsMoveValid(hero, start, selection))
        {
            // Update position if the move is valid
            switch (selection)
            {
                case 'W': case 'w': 
                    hero.SetCoordinates(hero.GetYCoordinates() - 1, hero.GetXCoordinates()); 
                    break;

                case 'S': case 's': 
                    hero.SetCoordinates(hero.GetYCoordinates() + 1, hero.GetXCoordinates()); 
                    break;

                case 'A': case 'a': 
                    hero.SetCoordinates(hero.GetYCoordinates(), hero.GetXCoordinates() - 1); 
                    break;

                case 'D': case 'd': 
                    hero.SetCoordinates(hero.GetYCoordinates(), hero.GetXCoordinates() + 1); 
                    break;
            }

            //Reset map tile back to default after player walks over it.
            start.PlaceOnMap(lastY, lastX, start.GetTile());
        }
        else
        {
            std::cout << "Cannot go any further in that direction.\n";
        }
    }
}
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

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
    }

    void DrawMap()
    {
        for (const auto& column : mMap)
        {
            for (char row : column)
            {
                std::cout << row << ' ';
            }
            std::cout << '\n';
        }
    }

    void PlaceOnMap(int y, int x, char object, bool collidable = false)
    {
        mMap[y][x] = object;
        if (collidable)
        {
            mCollidable[y][x] = true;
        }
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
    std::vector<std::vector<bool>> mCollidable; // Tracks collidable tiles
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

private:
    std::string mName{ "Character Name" };
    char mSprite{};
    int mYPos{ 0 };
    int mXPos{ 0 };
};

class Collision
{
public:
    bool IsWithinMapBounds(int proposedY, int proposedX, const Map& map)
    {
        return proposedY >= 0 && proposedY < map.GetHeight() &&
            proposedX >= 0 && proposedX < map.GetWidth();
    }

    bool IsCollidableObject(int proposedY, int proposedX, const Map& map)
    {
        return map.IsCollidable(proposedY, proposedX);
    }

    bool IsMoveValid(Character& character, const Map& map, char direction)
    {
        int proposedY = character.GetYCoordinates();
        int proposedX = character.GetXCoordinates();

        // Predict the new coordinates
        switch (direction)
        {
        case 'W': case 'w': proposedY--; break;
        case 'S': case 's': proposedY++; break;
        case 'A': case 'a': proposedX--; break;
        case 'D': case 'd': proposedX++; break;
        default:
            std::cout << "Invalid direction.\n";
            return false;
        }

        if (!IsWithinMapBounds(proposedY, proposedX, map))
        {
            std::cout << "Out of bounds!\n";
            return false;
        }

        if (IsCollidableObject(proposedY, proposedX, map))
        {
            std::cout << "Collision detected! Cannot move there.\n";
            return false;
        }

        return true;
    }
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
        int lastY{ hero.GetYCoordinates() };
        int lastX{ hero.GetXCoordinates() };

        start.PlaceOnMap(hero.GetYCoordinates(), hero.GetXCoordinates(), hero.GetSprite());

        start.DrawMap();

        std::cout << '\n';
        std::cout << "W). Up\n";
        std::cout << "S). Down\n";
        std::cout << "A). Left\n";
        std::cout << "D). Right\n";
        std::cout << ">";

        char direction{ ' ' };
        std::cin >> direction;

        if (collision.IsMoveValid(hero, start, direction))
        {
            // Update position if the move is valid
            switch (direction)
            {
            case 'W': case 'w': hero.SetCoordinates(hero.GetYCoordinates() - 1, hero.GetXCoordinates()); break;
            case 'S': case 's': hero.SetCoordinates(hero.GetYCoordinates() + 1, hero.GetXCoordinates()); break;
            case 'A': case 'a': hero.SetCoordinates(hero.GetYCoordinates(), hero.GetXCoordinates() - 1); break;
            case 'D': case 'd': hero.SetCoordinates(hero.GetYCoordinates(), hero.GetXCoordinates() + 1); break;
            }

            start.PlaceOnMap(lastY, lastX, start.GetTile());

            std::cout << "Current Y: " << hero.GetYCoordinates() << '\n';
            std::cout << "Current X: " << hero.GetXCoordinates() << "\n\n";

            std::cout << "Last Y: " << lastY << '\n';
            std::cout << "Last X: " << lastX << '\n';
        }
        else
        {
            std::cout << "Move invalid! Try again.\n";
        }
    }
}
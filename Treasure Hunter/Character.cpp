#include "Character.h"

void Character::Move(char direction, Map& map)
{
    int nextY = mPositionY;
    int nextX = mPositionX;

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

    int lastY{ mPositionY };
    int lastX{ mPositionX };

    if (IsMoveValid(nextY, nextX, map))
    {
        // Clear the player's symbol from the old position.
        map.SetEntityAt(mPositionY, mPositionX, '\0');

        mPositionY = nextY;
        mPositionX = nextX;

        // Place the player's sprite in the new position.
        map.SetEntityAt(mPositionY, mPositionX, mSprite);
    }
    else
    {
        std::cout << "Movement blocked!\n";
    }
}

int Character::GetY() const
{ 
    return mPositionY; 
}

int Character::GetX() const
{ 
    return mPositionX; 
}

char Character::GetSprite() const
{ 
    return mSprite; 
}

void Character::Dig(int y, int x, Map& map)
{
    if (map.GetCollisionState(y - 1, x))
    {
        std::cout << "You can't dig here! It's collidable.\n";
        return;
    }

    map.EditTile(y - 1, x, ' ', false, false, true, true); // Mark as persistent
    std::cout << "You dug up the spot!\n";
}

bool Character::IsMoveValid(int y, int x, const Map& map) const
{
    return y >= 0 && y < map.GetHeight() && x >= 0 && x < map.GetWidth() && !map.GetCollisionState(y, x);
}
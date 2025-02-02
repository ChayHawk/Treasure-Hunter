#include "Character.h"

void Character::Move(char direction, Map& map)
{
    int nextY = mPositionY;
    int nextX = mPositionX;

    switch (direction)
    {
        case 'W':
        case 'w':
            SetDirection('N');
            --nextY;
            break;

        case 'S':
        case 's':
            SetDirection('S');
            ++nextY;
            break;

        case 'A':
        case 'a':
            SetDirection('W');
            --nextX;
            break;

        case 'D':
        case 'd':
            SetDirection('E');
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
        map.SetEntityAt(mPositionY, mPositionX, NO_ENTITY);

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
    if (map.GetHasCollided(y - 1, x) || map.GetHasCollided(y + 1, x) || map.GetHasCollided(y, x + 1) || map.GetHasCollided(y, x - 1))
    {
        std::cout << "You can't dig here! It's collidable.\n";
        return;
    }

    if (GetDirection() == "North")
    {
        map.EditTile(y - 1, x, ' ', false, false, true, true); // Mark as persistent
    }
    else if (GetDirection() == "East")
    {
        map.EditTile(y, x + 1, ' ', false, false, true, true); // Mark as persistent
    }
    else if (GetDirection() == "South")
    {
        map.EditTile(y + 1, x, ' ', false, false, true, true); // Mark as persistent
    }
    else if (GetDirection() == "West")
    {
        map.EditTile(y, x - 1, ' ', false, false, true, true); // Mark as persistent
    }
    else
    {
        std::print("You cant dig outside of the map dummy, you want the game to crash or somethin'?\n");
    }
}

bool Character::IsMoveValid(int y, int x, const Map& map) const
{
    return y >= 0 && y < map.GetHeight() && x >= 0 && x < map.GetWidth() && !map.GetHasCollided(y, x);
}

int Character::GetDigsLeft() const
{
    return mDigsLeft;
}

int Character::GetScore() const
{
    return mScore;
}

std::string Character::GetDirection() const
{
    switch (mDirection)
    {
        case 'N':
            return "North";
            break;

        case 'E':
            return "East";
            break;

        case 'S':
            return "South";
            break;

        case 'W':
            return "West";
            break;

        default:
            return "Error";
    }
}

void Character::SetDirection(char direction)
{
    if (std::isprint(static_cast<unsigned char>(direction)))
    {
        mDirection = direction;
    }
}
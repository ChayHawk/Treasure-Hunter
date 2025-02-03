#include "Character.h"

// ============================================================================
//                              CHARACTER CLASS IMPLEMENTATION
// ============================================================================

/**
 * @brief Move character
 * @version 1.0
 * @author Chay Hawk
 *
 * Moves the player or any other character on the map
 *
 * @param direction 
 * @param map
 */
void Character::Move(char direction, Map& map)
{
    int nextY = mPositionY;
    int nextX = mPositionX;

    switch (direction)
    {
        case 'w':
            SetDirection('N');
            --nextY;
            break;

        case 's':
            SetDirection('S');
            ++nextY;
            break;

        case 'a':
            SetDirection('W');
            --nextX;
            break;

        case 'd':
            SetDirection('E');
            ++nextX;
            break;

        default:
            std::print("Movement blocked at ({}, {})!\n", nextY, nextX);
            return;
    }

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
        std::print("Movement blocked at ({}, {})!\n", nextY, nextX);
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


void Character::Dig(Map& map)
{
    // Determine target tile based on current position and direction.
    int targetY = mPositionY;
    int targetX = mPositionX;

    switch (mDirection)
    {
        case 'N': targetY = mPositionY - 1; break;
        case 'E': targetX = mPositionX + 1; break;
        case 'S': targetY = mPositionY + 1; break;
        case 'W': targetX = mPositionX - 1; break;
        default:
            std::print("Invalid direction for digging.\n");
            return;
    }

    if (mDigsLeft > 0)
    {
        // Check if the target tile is in bounds.
        if (!map.IsInBounds(targetY, targetX))
        {
            std::print("Cannot dig outside of the map.\n");
            return;
        }

        // Check if the target tile is collidable.
        if (map.GetHasCollision(targetY, targetX))
        {
            std::print("You can't dig here! It's collidable.\n");
            return;
        }

        // Perform the dig (set the target tile to empty and mark it persistent).
        map.EditTile(targetY, targetX, ' ', false, false, true, true);
        SubtractFromDigCount();
    }
    else
    {
        std::print("");
    }

    
}


bool Character::IsMoveValid(int y, int x, const Map& map) const
{
    return y >= 0 && y < map.GetHeight() && x >= 0 && x < map.GetWidth() && !map.GetHasCollision(y, x);
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
        case 'N': return "North";
        case 'E': return "East";
        case 'S': return "South";
        case 'W': return "West";
        default:  return "Error";
    }
}


void Character::SetDirection(char direction)
{
    // Only allow valid directions: N, S, E, or W.
    switch (direction)
    {
        case 'N':
        case 'S':
        case 'E':
        case 'W':
            mDirection = direction;
            break;
        default:
            std::print("Invalid direction provided: {}\n", direction);
            // Optionally, you might set a default value or leave mDirection unchanged.
            break;
    }
}


void Character::SetDigCount(int amount)
{
    if (amount > 0)
    {
        mDigsLeft = amount;
    }
}

void Character::SubtractFromDigCount()
{
    if (mDigsLeft > 0)
    {
        --mDigsLeft;
    }
}

void Character::AddToDigCount()
{
    ++mDigsLeft;
}
#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "Map.h"

class Character
{
    public:
        Character(const std::string& name, char sprite, int y, int x)
            : mName(name), mSprite(sprite), mPositionY(y), mPositionX(x)
        {
        }

        void Move(char direction, Map& map)
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
                mPositionY = nextY;
                mPositionX = nextX;

                map.ResetTileState(lastY, lastX);
            }
            else
            {
                std::cout << "Movement blocked!\n";
            }
        }

        int GetY() const { return mPositionY; }
        int GetX() const { return mPositionX; }
        char GetSprite() const { return mSprite; }

        //Need to find a way to stop player from digging into collidable objects.
        void Dig(int y, int x, Map& map)
        {
            if (map.GetCollisionState(y - 1, x))
            {
                std::cout << "You can't dig here! It's collidable.\n";
                return;
            }

            map.ModifyTile(y - 1, x, ' ', false, false, true, false, true); // Mark as persistent
            std::cout << "You dug up the spot!\n";
        }

    private:
        std::string mName{};
        char mSprite{};
        int mPositionY{};
        int mPositionX{};
        char mPreviousTile{};
        int mDigsLeft{};

        std::vector<std::string> mInventory;

        bool IsMoveValid(int y, int x, const Map& map) const
        {
            return y >= 0 && y < map.GetHeight() && x >= 0 && x < map.GetWidth() && !map.GetCollisionState(y, x);
        }
};

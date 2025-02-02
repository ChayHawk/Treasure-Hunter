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
        {}

        void Move(char direction, Map& map);

        int GetY() const;
        int GetX() const;
        char GetSprite() const;
        int GetDigsLeft() const;
        int GetScore() const;
        std::string GetDirection() const;

        //Need to find a way to stop player from digging into collidable objects.
        void Dig(int y, int x, Map& map);

    private:
        std::string mName{};
        char mSprite{};
        int mPositionY{};
        int mPositionX{};
        char mPreviousTile{};
        int mDigsLeft{};
        int mScore{};
        char mDirection{};

        std::vector<std::string> mInventory;

        bool IsMoveValid(int y, int x, const Map& map) const;
        void SetDirection(char direction);
};

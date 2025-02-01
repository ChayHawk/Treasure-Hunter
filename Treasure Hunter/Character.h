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

        //Need to find a way to stop player from digging into collidable objects.
        void Dig(int y, int x, Map& map);

    private:
        std::string mName{};
        char mSprite{};
        int mPositionY{};
        int mPositionX{};
        char mPreviousTile{};
        int mDigsLeft{};

        std::vector<std::string> mInventory;

        bool IsMoveValid(int y, int x, const Map& map) const;
};

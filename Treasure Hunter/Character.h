#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "Map.h"


/**
 * @class Character
 * @brief Character Class
 * @version 1.0
 * @author Chay Hawk
 *
 * This represents a character in the game.
 * 
 * @todo Implement an interaction system where you can interact with objects
 * and entities within a certain radius
 * @todo Implement Inventory system for treasure
 * @todo Implement a metal detector or an item finder that tells the player
 * The general location of treasure, maybe within a 9 square block. Upgrades could make
 * the radius smaller.
 */
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
        void Dig(Map& map);
        void SetDigCount(int amount);
        void SubtractFromDigCount();
        void AddToDigCount();

        void AddToInventory()
        {
            mInventory.push_back();
        }

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

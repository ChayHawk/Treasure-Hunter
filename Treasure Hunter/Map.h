#pragma once

#include <iostream>
#include <string>
#include <vector>

class Map
{
    public:
        Map(const std::string& name, int height, int width, char tile)
            : mName(name), mHeight(height), mWidth(width), mTile(tile) {
        }

        void Initialize()
        {
            mMap.assign(mHeight, std::vector<Tile>(mWidth, { mTile, mTile, false, false, 0, false }));

            //Assign ID to tiles. Could maybe make this its own function.
            constexpr int startingTileID = 100;
            int idCounter{ startingTileID };
            for (size_t column = 0; column < mHeight; ++column)
            {
                for (size_t row = 0; row < mWidth; ++row)
                {
                    mMap[column][row].tileID = ++idCounter; //Set tile ID
                    mMap[column][row].masterTile = mMap[column][row].baseTile; //Set tile state, this allows for the tile to be reset if modified
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
                        mMap[column][row].baseTile = ' ';
                    }

                    if (mMap[column][row].resetTileState == true)
                    {
                        mMap[column][row].baseTile = mMap[column][row].masterTile;
                    }

                    std::cout << mMap[column][row].baseTile << ' ';
                }
                std::cout << '\n';
            }
        }


        //This is needed to add modifications made to the base map
        void RescanMap()
        {
            for (size_t column = 0; column < mHeight; ++column)
            {
                for (size_t row = 0; row < mWidth; ++row)
                {
                    if (!mMap[column][row].isPersistent)
                    {
                        mMap[column][row].masterTile = mMap[column][row].baseTile;
                    }
                }
            }
        }

        void ModifyTile
        (
            int y, int x, char newTile = '\0', bool collisionState = false, bool interactionState = false,
            bool doNotRedraw = false, bool resetTileState = false, bool isPersistent = false
        )
        {
            if (!IsInBounds(y, x))
            {
                std::cout << "Coordinates are out of bounds!\n";
                return;
            }

            if (newTile != '\0')
            {
                mMap[y][x].baseTile = newTile;
            }

            mMap[y][x].collisionState = collisionState;
            mMap[y][x].interactionState = interactionState;
            mMap[y][x].doNotRedraw = doNotRedraw;
            mMap[y][x].resetTileState = resetTileState;
            mMap[y][x].isPersistent = isPersistent;
        }


        bool GetInteractionState(int y, int x) const
        {
            return mMap[y][x].interactionState;
        }

        bool GetCollisionState(int y, int x) const
        {
            return mMap[y][x].collisionState;
        }

        int GetHeight() const
        {
            return mHeight;
        }

        int GetWidth() const
        {
            return mWidth;
        }

        int GetTileID(int y, int x) const
        {
            return mMap[y][x].tileID;
        }

        void Toggle(int y, int x, char onTile, char offTile, bool collisionOnState, bool collisionOffState)
        {
            if (GetInteractionState(y, x))
            {
                // Currently "on", so toggle to "off"
                ModifyTile(y, x, offTile, collisionOffState, false);
            }
            else
            {
                // Currently "off", so toggle to "on"
                ModifyTile(y, x, onTile, collisionOnState, true);
            }
        }


        void ResetTileState(int y, int x)
        {
            if (!mMap[y][x].isPersistent)
            {
                mMap[y][x].baseTile = mMap[y][x].masterTile;
            }
        }


    private:
        struct Tile
        {
            char baseTile{};
            char masterTile{};
            bool resetTileState{ false };
            bool collisionState{ false };
            bool interactionState{ false };
            int interactCount{ 0 };
            bool isCollectible{ false };
            int tileID{};
            bool doNotRedraw{ false };
            bool isPersistent{ false };
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
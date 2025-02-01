#include "Map.h"

void Map::Initialize()
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

void Map::Draw()
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

void Map::RescanMap()
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

void Map::ModifyTile
(
    int y, int x, char newTile, bool collisionState, bool interactionState,
    bool doNotRedraw, bool resetTileState, bool isPersistent
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

bool Map::GetInteractionState(int y, int x) const
{
    return mMap[y][x].interactionState;
}

bool Map::GetCollisionState(int y, int x) const
{
    return mMap[y][x].collisionState;
}

int Map::GetHeight() const
{
    return mHeight;
}

int Map::GetWidth() const
{
    return mWidth;
}

int Map::GetTileID(int y, int x) const
{
    return mMap[y][x].tileID;
}

void Map::Toggle(int y, int x, char onTile, char offTile, bool collisionOnState, bool collisionOffState)
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

void Map::ResetTileState(int y, int x)
{
    if (!mMap[y][x].isPersistent)
    {
        mMap[y][x].baseTile = mMap[y][x].masterTile;
    }
}

bool Map::IsInBounds(int y, int x) const
{
    return y >= 0 && y < mHeight && x >= 0 && x < mWidth;
}
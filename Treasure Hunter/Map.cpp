#include "Map.h"

void Map::Initialize()
{
    mMap.assign(mHeight, std::vector<Tile>(mWidth, Tile(mTile)));
}

void Map::Draw()
{
    for (size_t column = 0; column < mHeight; ++column)
    {
        for (size_t row = 0; row < mWidth; ++row)
        {
            if (mMap[column][row].GetDoNotRedraw() == true)
            {
                mMap[column][row].SetBaseTile(' ');
            }

            //If entity is present then print it, if not, print base tile
            if (mMap[column][row].GetEntityTile() != '\0')
            {
                std::cout << mMap[column][row].GetEntityTile() << ' ';
            }
            else
            {
                std::cout << mMap[column][row].GetBaseTile() << ' ';
            }
        }
        std::cout << '\n';
    }
}

void Map::EditTile(int y, int x, char newTile, bool hasCollision, bool hasInteracted, bool doNotRedraw, bool isPersistent)
{
    if (!IsInBounds(y, x))
    {
        std::cout << "Coordinates are out of bounds!\n";
        return;
    }

    if (newTile != NO_ENTITY)
    {
        mMap[y][x].SetObjectTile(newTile);
    }

    mMap[y][x].SetHasCollision(hasCollision);
    mMap[y][x].SetHasInteracted(hasInteracted);
    mMap[y][x].SetDoNotRedraw(doNotRedraw);
    mMap[y][x].SetIsPersistent(isPersistent);
}

bool Map::GetHasInteracted(int y, int x) const
{
    return mMap[y][x].HasInteracted();
}

bool Map::GetHasCollided(int y, int x) const
{
    return mMap[y][x].HasCollision();
}

int Map::GetHeight() const
{
    return mHeight;
}

int Map::GetWidth() const
{
    return mWidth;
}

void Map::Toggle(int y, int x, char onTile, char offTile, bool collisionOnState, bool collisionOffState)
{
    if (GetHasInteracted(y, x))
    {
        // Currently "on", so toggle to "off"
        EditTile(y, x, offTile, collisionOffState, false);
    }
    else
    {
        // Currently "off", so toggle to "on"
        EditTile(y, x, onTile, collisionOnState, true);
    }
}

void Map::ResetTileState(int y, int x)
{
    if (!mMap[y][x].GetIsPersistent())
    {
        mMap[y][x].SetBaseTile(mMap[y][x].GetBaseTile());
    }
}

bool Map::IsInBounds(int y, int x) const
{
    return y >= 0 && y < mHeight && x >= 0 && x < mWidth;
}

void Map::SetEntityAt(int y, int x, char entity)
{
    if (IsInBounds(y, x))
    {
        mMap[y][x].SetEntityTile(entity);
    }
}

// In your Map.cpp, implement it like this:
void Map::ModifyLayer(const std::function<void(Tile&)>& func)
{
    for (auto&row : mMap)
    {
        for (auto&tile : row)
        {
            func(tile);
        }
    }
}

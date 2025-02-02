#include "Map.h"

void Map::Initialize()
{
    mMap.assign(mHeight, std::vector<Tile>(mWidth, Tile(mTile)));

    constexpr int startingTileID = 100;
    int idCounter{ startingTileID };
    for (size_t column = 0; column < mHeight; ++column)
    {
        for (size_t row = 0; row < mWidth; ++row)
        {
            mMap[column][row].SetTileID(++idCounter); //Set tile ID
            mMap[column][row].SetMasterTile(mMap[column][row].GetBaseTile()); //Set master tile, this allows for the tile to be reset if modified
        }
    }
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

            if (mMap[column][row].GetResetTileState() == true)
            {
                mMap[column][row].SetBaseTile(mMap[column][row].GetMasterTile());
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

void Map::RescanMap()
{
    for (size_t column = 0; column < mHeight; ++column)
    {
        for (size_t row = 0; row < mWidth; ++row)
        {
            if (!mMap[column][row].GetIsPersistent())
            {
                mMap[column][row].SetMasterTile(mMap[column][row].GetBaseTile());
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
        mMap[y][x].SetBaseTile(newTile);
    }

    mMap[y][x].SetCollisionState(collisionState);
    mMap[y][x].SetInteractionState(interactionState);
    mMap[y][x].SetDoNotRedraw(doNotRedraw);
    mMap[y][x].SetResetTileState(resetTileState);
    mMap[y][x].SetIsPersistent(isPersistent);
}

bool Map::GetInteractionState(int y, int x) const
{
    return mMap[y][x].GetInteractionState();
}

bool Map::GetCollisionState(int y, int x) const
{
    return mMap[y][x].GetCollisionState();
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
    return mMap[y][x].GetTileID();
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
    if (!mMap[y][x].GetIsPersistent())
    {
        mMap[y][x].SetBaseTile(mMap[y][x].GetMasterTile());
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

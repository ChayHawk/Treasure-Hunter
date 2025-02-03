#include "Map.h"

/**
 * @brief Initialize the map
 * @version 1.0
 * @author Chay Hawk
 *
 * This function initializes the map
 *
 */
void Map::Initialize()
{
    mMap.assign(mHeight, std::vector<Tile>(mWidth, Tile(mTile)));
}


/**
 * @brief Draw the map
 * @version 1.0
 * @author Chay Hawk
 *
 * This function draws the map and characters to the screen
 *
 */
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
            if (mMap[column][row].GetEntityTile() != NO_ENTITY)
            {
                std::cout << mMap[column][row].GetEntityTile() << ' ';
                //std::print(" ", mMap[column][row].GetEntityTile());
            }
            else
            {
                std::cout << mMap[column][row].GetBaseTile() << ' ';
                //std::print(" ", mMap[column][row].GetBaseTile());
            }
        }
        std::cout << '\n';
    }
}


/**
 * @brief Modify a tile on the map
 * @version 1.0
 * @author Chay Hawk
 * 
 * This function allows you to modify a tile anywhere on the map
 *
 * @param y The players y coordinates
 * @param x The players x coordinates
 * @param newTile The tile to place down
 * @param hasCollision If the new tile has collision
 * @param hasInteracted If this tile should now be marked as interacted with
 * @param doNotRedraw If this tile should redraw itself or not
 * @param isPersistent Marks the tile as persistent which prevents it from being drawn over.
 */
void Map::EditTile(int y, int x, char newTile, bool hasCollision, bool hasInteracted, bool doNotRedraw, bool isPersistent)
{
    if (!IsInBounds(y, x))
    {
        std::print("Coordinates are out of bounds!\n");
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


/**
 * @brief Modify a range of tiles on the map
 * @version 1.0
 * @author Chay Hawk
 *
 * This function allows you to modify tiles in a given range
 *
 * @param y The players y coordinates
 * @param x The players x coordinates
 * @param newTile The tile to place down
 * @param hasCollision If the new tile has collision
 * @param hasInteracted If this tile should now be marked as interacted with
 * @param doNotRedraw If this tile should redraw itself or not
 * @param isPersistent Marks the tile as persistent which prevents it from being drawn over.
 */
void Map::EditTileRange(int y, int x, int rangeY, int rangeX, char newTile, bool hasCollision, bool hasInteracted, bool doNotRedraw, bool isPersistent)
{
    // Ensure the entire range is within bounds
    if (!IsInBounds(y, x) || !IsInBounds(y + rangeY, x + rangeX))
    {
        std::print("Coordinates are out of bounds!\n");
        return;
    }

    // Iterate over the intended range
    for (int column = y; column <= y + rangeY; ++column)
    {
        for (int row = x; row <= x + rangeX; ++row)
        {
            if (IsInBounds(column, row))  // Double-check bounds to prevent errors
            {
                if (newTile != NO_ENTITY)
                {
                    mMap[column][row].SetObjectTile(newTile);
                }

                mMap[column][row].SetHasCollision(hasCollision);
                mMap[column][row].SetHasInteracted(hasInteracted);
                mMap[column][row].SetDoNotRedraw(doNotRedraw);
                mMap[column][row].SetIsPersistent(isPersistent);
            }
        }
    }
}


/**
 * @brief Check interaction state
 * @version 1.0
 * @author Chay Hawk
 *
 * Gets the interaction state of the tile
 *
 * @param y The players y coordinates
 * @param x The players x coordinates
 * @return Returns true if the tile has been interacted with, false otherwise
 */
bool Map::GetHasInteracted(int y, int x) const
{
    return mMap[y][x].HasInteracted();
}


/**
 * @brief Check collision
 * @version 1.0
 * @author Chay Hawk
 *
 * Check if the tile has collision or not
 *
 * @param y The players y coordinates
 * @param x The players x coordinates
 * @return Returns true if the tile has collision, false if it does not
 */
bool Map::GetHasCollision(int y, int x) const
{
    return mMap[y][x].HasCollision();
}


/**
 * @brief Get map height
 * @version 1.0
 * @author Chay Hawk
 *
 * Get the height of the map
 *
 * @return Returns the height of the map
 */
int Map::GetHeight() const
{
    return mHeight;
}


/**
 * @brief Get map width
 * @version 1.0
 * @author Chay Hawk
 *
 * Get the width of the map
 *
 * @return Returns the width of the map
 */
int Map::GetWidth() const
{
    return mWidth;
}

/**
 * @brief Toggle switch
 * @version 1.0
 * @author Chay Hawk
 *
 * This function acts as a toggle for things like doors
 * 
 * @todo Look into refactoring boolean names to something clearer
 *
 * @param y The players y coordinates
 * @param x The players x coordinates
 * @param onTile The tile that displays when the toggle is "on"
 * @param offTile The tile that displays when the tile is "off"
 * @param collisionOnState Should collision be on or off for the "on" tile
 * @param collisionOffState Should collision be on or off for the "off" tile
 */
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


/**
 * @brief 
 * @version 1.0
 * @author Chay Hawk
 *
 *@warning Function does not appear to be used 
 *
 * @param y The players y coordinates
 * @param x The players x coordinates
 */
void Map::ResetTileState(int y, int x)
{
    if (!mMap[y][x].GetIsPersistent())
    {
        mMap[y][x].SetBaseTile(mMap[y][x].GetBaseTile());
    }
}


/**
 * @brief Check if within map bounds
 * @version 1.0
 * @author Chay Hawk
 *
 * Checks to see if the coordinates given to it are within the boundaries of the current map
 *
 * @param y The players y coordinates
 * @param x The players x coordinates
 */
bool Map::IsInBounds(int y, int x) const
{
    return y >= 0 && y < mHeight && x >= 0 && x < mWidth;
}


/**
 * @brief Set entity on map
 * @version 1.0
 * @author Chay Hawk
 *
 * This function sets an entity, usually an NPC or some other entity that has movement on the map.
 * 
 *
 * @param y The players y coordinates
 * @param x The players x coordinates
 * @param entity The entity to set
 */
void Map::SetEntityAt(int y, int x, char entity)
{
    if (IsInBounds(y, x))
    {
        mMap[y][x].SetEntityTile(entity);
    }
}


/**
 * @brief Modifies a layer of the map
 * @version 1.0
 * @author Chay Hawk
 *
 * This function modifies entire layers of the map
 *
 * @param func Takes a lambda
 */
void Map::ModifyLayer(const std::function<void(Tile&)>& func)
{
    for (auto& row : mMap)
    {
        for (auto& tile : row)
        {
            func(tile);
        }
    }
}




char Map::Tile::GetBaseTile() const 
{ 
    return baseTile; 
}

char Map::Tile::GetObjectTile() const 
{ 
    return objectTile; 
}

char Map::Tile::GetEntityTile() const 
{ 
    return entityTile; 
}

bool Map::Tile::HasCollision() const 
{ 
    return hasCollision; 
}

bool Map::Tile::HasInteracted() const 
{ 
    return hasInteracted; 
}

bool Map::Tile::GetDoNotRedraw() const 
{ 
    return doNotRedraw; 
}

bool Map::Tile::GetIsPersistent() const 
{ 
    return isPersistent; 
}

void Map::Tile::SetBaseTile(char newBaseTile)
{
    if (std::isprint(static_cast<unsigned char>(newBaseTile)))
    {
        baseTile = newBaseTile;
    }
}

void Map::Tile::SetObjectTile(char newObjectTile)
{
    if (std::isprint(static_cast<unsigned char>(newObjectTile)))
    {
        baseTile = newObjectTile;
    }
}

void Map::Tile::SetEntityTile(char newEntityTile)
{
    if (newEntityTile == NO_ENTITY || std::isprint(static_cast<unsigned char>(newEntityTile)))
    {
        entityTile = newEntityTile;
    }
}

void Map::Tile::SetHasCollision(bool value) 
{ 
    hasCollision = value; 
}

void Map::Tile::SetHasInteracted(bool value) 
{ 
    hasInteracted = value; 
}

void Map::Tile::SetDoNotRedraw(bool value) 
{ 
    doNotRedraw = value; 
}

void Map::Tile::SetIsPersistent(bool value) 
{ 
    isPersistent = value; 
}
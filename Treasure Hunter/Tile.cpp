#include "Map.h"

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
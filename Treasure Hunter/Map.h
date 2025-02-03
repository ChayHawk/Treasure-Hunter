#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <cctype>
#include <print>

static constexpr char NO_ENTITY = '\0';

class Map
{
    public:
        Map(const std::string& name, int height, int width, char tile)
            : mName(name), mHeight(height), mWidth(width), mTile(tile)
        {}

        void Initialize();
        void Draw();
        void EditTile(int y, int x, char newTile = NO_ENTITY, bool hasCollision = false, bool hasInteracted = false, bool doNotRedraw = false, bool isPersistent = false);
        void EditTileRange(int y, int x, int rangeY, int rangeX, char newTile = NO_ENTITY, bool hasCollision = false, bool hasInteracted = false, bool doNotRedraw = false, bool isPersistent = false);
        bool GetHasInteracted(int y, int x) const;
        bool GetHasCollided(int y, int x) const;
        int GetHeight() const;
        int GetWidth() const;
        void Toggle(int y, int x, char onTile, char offTile, bool collisionOnState, bool collisionOffState);
        void ResetTileState(int y, int x);
        void SetEntityAt(int y, int x, char entity);
        bool IsInBounds(int y, int x) const;

        class Tile 
        {
            public:
                Tile(char base)
                    : baseTile(base), objectTile(NO_ENTITY), entityTile(NO_ENTITY),
                    hasCollision(false), hasInteracted(false), doNotRedraw(false), 
                    isPersistent(false)
                {}

                char GetBaseTile() const { return baseTile; }
                char GetObjectTile() const { return objectTile; }                
                char GetEntityTile() const { return entityTile; }
                bool HasCollision() const { return hasCollision; }
                bool HasInteracted() const { return hasInteracted; }
                bool GetDoNotRedraw() const { return doNotRedraw; }
                bool GetIsPersistent() const { return isPersistent; }

                void SetBaseTile(char newBaseTile)
                {
                    if (std::isprint(static_cast<unsigned char>(newBaseTile)))
                    {
                        baseTile = newBaseTile;
                    }
                }

                void SetObjectTile(char newObjectTile) 
                { 
                    if(std::isprint(static_cast<unsigned char>(newObjectTile)))
                    {
                        baseTile = newObjectTile;
                    }
                }
        
                void SetEntityTile(char newEntityTile) 
                { 
                    if (newEntityTile == NO_ENTITY || std::isprint(static_cast<unsigned char>(newEntityTile)))
                    {
                        entityTile = newEntityTile;
                    }
                }

                void SetHasCollision(bool value) { hasCollision = value; }
                void SetHasInteracted(bool value) { hasInteracted = value; }
                void SetDoNotRedraw(bool value) { doNotRedraw = value; }
                void SetIsPersistent(bool value) { isPersistent = value; }

            private:
                char objectTile;
                char baseTile;
                char entityTile;
                bool hasCollision;
                bool hasInteracted;
                bool doNotRedraw;
                bool isPersistent;
        };

        void ModifyLayer(const std::function<void(Tile&)>& func);

    private:
        std::string mName{};
        int mHeight{};
        int mWidth{};
        char mTile{};
        std::vector<std::vector<Tile>> mMap;
};
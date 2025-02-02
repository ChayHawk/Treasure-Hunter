#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <cctype>

static constexpr char NO_ENTITY = '\0';

class Map
{
    public:
        Map(const std::string& name, int height, int width, char tile)
            : mName(name), mHeight(height), mWidth(width), mTile(tile)
        {}

        void Initialize();
        void Draw();
        void RescanMap();
        void EditTile
        (
            int y, int x, char newTile = NO_ENTITY, bool hasCollision = false, bool hasInteracted = false,
            bool doNotRedraw = false, bool isPersistent = false
        );
        bool GetInteractionState(int y, int x) const;
        bool GetCollisionState(int y, int x) const;
        int GetHeight() const;
        int GetWidth() const;
        int GetTileID(int y, int x) const;
        void Toggle(int y, int x, char onTile, char offTile, bool collisionOnState, bool collisionOffState);
        void ResetTileState(int y, int x);
        void SetEntityAt(int y, int x, char entity);

        class Tile 
        {
            public:
                Tile(char base)
                    : baseTile(base), objectTile(NO_ENTITY), entityTile(NO_ENTITY),
                    hasCollision(false), hasInteracted(false), tileID(0), 
                    doNotRedraw(false), isPersistent(false)
                {}

                char GetObjectTile() const { return objectTile; }
                char GetBaseTile() const { return baseTile; }
                char GetEntityTile() const { return entityTile; }
                bool HasCollision() const { return hasCollision; }
                bool HasInteracted() const { return hasInteracted; }
                int GetTileID() const { return tileID; }
                bool GetDoNotRedraw() const { return doNotRedraw; }
                bool GetIsPersistent() const { return isPersistent; }

                void SetObjectTile(char newObjectTile) 
                { 
                    if(std::isprint(static_cast<unsigned char>(newObjectTile)))
                    {
                        baseTile = newObjectTile;
                    }
                }

                void SetBaseTile(char newBaseTile) 
                { 
                    if (std::isprint(static_cast<unsigned char>(newBaseTile)))
                    {
                        baseTile = newBaseTile;
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

                void SetTileID(int id) 
                { 
                    if (id >= 0)
                    {
                        tileID = id;
                    }
                }

                void SetDoNotRedraw(bool value) { doNotRedraw = value; }
                void SetIsPersistent(bool value) { isPersistent = value; }

            private:
                char objectTile;
                char baseTile;
                char entityTile;
                bool hasCollision;
                bool hasInteracted;
                int tileID;
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

        bool IsInBounds(int y, int x) const;
};
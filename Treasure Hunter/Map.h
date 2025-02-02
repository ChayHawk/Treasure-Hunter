#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <cctype>

class Map
{
    public:
        Map(const std::string& name, int height, int width, char tile)
            : mName(name), mHeight(height), mWidth(width), mTile(tile)
        {}

        void Initialize();
        void Draw();
        void RescanMap();
        void ModifyTile
        (
            int y, int x, char newTile = '\0', bool collisionState = false, bool interactionState = false,
            bool doNotRedraw = false, bool resetTileState = false, bool isPersistent = false
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
                    : baseTile(base), masterTile(base), entityTile('\0'),
                    resetTileState(false), collisionState(false),
                    interactionState(false), interactCount(0), isCollectible(false),
                    tileID(0), doNotRedraw(false), isPersistent(false)
                {}

                char GetBaseTile() const { return baseTile; }
                char GetMasterTile() const { return masterTile; }
                char GetEntityTile() const { return entityTile; }
                bool GetResetTileState() const { return resetTileState; }
                bool GetCollisionState() const { return collisionState; }
                bool GetInteractionState() const { return interactionState; }
                int GetInteractCount() const { return interactCount; }
                bool GetIsCollectible() const { return isCollectible; }
                int GetTileID() const { return tileID; }
                bool GetDoNotRedraw() const { return doNotRedraw; }
                bool GetIsPersistent() const { return isPersistent; }

                void SetBaseTile(char newBaseTile) 
                { 
                    if(std::isprint(static_cast<unsigned char>(newBaseTile)))
                    {
                        baseTile = newBaseTile;
                    }
                }

                void SetMasterTile(char newMasterTile) 
                { 
                    if (std::isprint(static_cast<unsigned char>(newMasterTile)))
                    {
                        masterTile = newMasterTile;
                    }
                }

                void SetEntityTile(char newEntityTile) 
                { 
                    if (newEntityTile == '\0' || std::isprint(static_cast<unsigned char>(newEntityTile)))
                    {
                        entityTile = newEntityTile;
                    }
                }

                void SetResetTileState(bool value) { resetTileState = value; }
                void SetCollisionState(bool value) { collisionState = value; }
                void SetInteractionState(bool value) { interactionState = value; }

                void SetInteractCount(int count) 
                { 
                    if(count >= 0)
                    {
                        interactCount = count;
                    }
                }

                void SetIsCollectible(bool value) { isCollectible = value; }

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
                char baseTile;
                char masterTile;
                char entityTile;
                bool resetTileState;
                bool collisionState;
                bool interactionState;
                int interactCount;
                bool isCollectible;
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
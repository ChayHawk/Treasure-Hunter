#pragma once

#include <iostream>
#include <string>
#include <vector>

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

        bool IsInBounds(int y, int x) const;
};
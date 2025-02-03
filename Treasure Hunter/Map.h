#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <cctype>
#include <print>

static constexpr char NO_ENTITY = '\0';

/**
 * @class Map
 * @brief Map Class
 * @version 1.0
 * @author Chay Hawk
 *
 * This represents a map in the game
 */
class Map
{
    public:
        Map(const std::string& name, int height, int width, char tile)
            : mName(name), mHeight(height), mWidth(width), mTile(tile)
        {}

        void Initialize();
        void Draw();
        void EditTile(int y, int x, char newTile = NO_ENTITY, bool hasCollision = false, bool hasInteracted = false, bool doNotRedraw = false, bool isPersistent = false, bool hasTreasure = false);
        void EditTileRange(int y, int x, int rangeY, int rangeX, char newTile = NO_ENTITY, bool hasCollision = false, bool hasInteracted = false, bool doNotRedraw = false, bool isPersistent = false, bool hasTreasure = false);
        bool GetHasInteracted(int y, int x) const;
        bool GetHasCollision(int y, int x) const;
        bool GetHasTreasure(int y, int x) const;
        int GetHeight() const;
        int GetWidth() const;
        void Toggle(int y, int x, char onTile, char offTile, bool collisionOnState, bool collisionOffState);
        void SetEntityAt(int y, int x, char entity);
        bool IsInBounds(int y, int x) const;

         /**
         * @class Tile
         * @brief Nested Tile Class
         * @version 1.0
         * @author Chay Hawk
         *
         * This represents a tile inside of the map
         */
        class Tile 
        {
            public:
                Tile(char base)
                    : baseTile(base), objectTile(NO_ENTITY), entityTile(NO_ENTITY),
                    treasureTile(NO_ENTITY), hasCollision(false), hasInteracted(false),
                    doNotRedraw(false), isPersistent(false), hasTreasure(false)
                {}

                char GetBaseTile() const;
                void SetBaseTile(char newBaseTile);

                char GetObjectTile() const;
                void SetObjectTile(char newObjectTile);

                char GetEntityTile() const;
                void SetEntityTile(char newEntityTile);

                bool HasCollision() const;
                void SetHasCollision(bool value);

                bool HasInteracted() const;
                void SetHasInteracted(bool value);

                bool HasTreasure() const;
                void SetHasTreasure(bool value);

                bool GetDoNotRedraw() const;
                void SetDoNotRedraw(bool value);

                bool GetIsPersistent() const;
                void SetIsPersistent(bool value);

                char GetTreasureTile() const;
                void SetTreasureTile(char newTreasureTile);

            private:
                char objectTile;
                char baseTile;
                char entityTile;
                char treasureTile;
                bool hasCollision;
                bool hasInteracted;
                bool doNotRedraw;
                bool isPersistent;
                bool hasTreasure;
        };

        void ModifyLayer(const std::function<void(Tile&)>& func);

    private:
        std::string mName{};
        int mHeight{};
        int mWidth{};
        char mTile{};
        std::vector<std::vector<Tile>> mMap;
};
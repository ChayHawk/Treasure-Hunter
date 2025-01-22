#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>
#include <format>

class Map
{
    public:
        Map(const std::string& name, int height, int width, char tile)
            : mName(name), mHeight(height), mWidth(width), mTile(tile)
        {
        }

        void Initialize()
        {
            mBaseLayer.assign(mHeight, std::vector<char>(mWidth, mTile));
            mCollisionLayer.assign(mHeight, std::vector<bool>(mWidth, false));
            mInteractionLayer.assign(mHeight, std::vector<bool>(mWidth, false));
        }

        void DrawMap()
        {
            for (size_t column = 0; column < mHeight; ++column)
            {
                for (size_t row = 0; row < mWidth; ++row)
                {
                    if (mInteractionLayer[column][row] == true)
                    {
                        std::cout << 'x' << ' ';
                    }
                    else
                    {
                        std::cout << mBaseLayer[column][row] << ' ';
                    }
                }
                std::cout << '\n';
            }
        }

        void PlaceOnMap(int posY, int posX, char object, bool collidable = false)
        {
            mBaseLayer[posY][posX] = object;
            mCollisionLayer[posY][posX] = collidable;
        }


        void MarkAsDug(int posY, int posX)
        {
            mInteractionLayer[posY][posX] = true;
        }

        bool IsDug(int posY, int posX) const
        {
            return mInteractionLayer[posY][posX];
        }

        int GetHeight() const
        {
            return mHeight;
        }

        int GetWidth() const
        {
            return mWidth;
        }

        bool IsCollidable(int posY, int posX) const
        {
            return mCollisionLayer[posY][posX];
        }

        char GetTile() const
        {
            return mTile;
        }

    private:
        std::string mName{ "Map Name" };
        int mHeight{ 10 };
        int mWidth{ 10 };
        char mTile{ '.' };

        std::vector<std::vector<char>> mBaseLayer;
        std::vector<std::vector<bool>> mCollisionLayer;
        std::vector<std::vector<bool>> mInteractionLayer;
};


class Treasure
{
    public:
        Treasure(const std::string name, int value) : mName(name), mValue(value)
        { }

        std::string GetName() const
        {
            return mName;
        }

        int GetValue() const
        {
            return mValue;
        }

    private:
        std::string mName{};
        int mValue{};
};


class Character
{
    public:
        Character(const std::string& name, char sprite, int score, int digsLeft, double money) 
            : mName(name), mSprite(sprite), mScore(score), mDigsLeft(digsLeft), mMoney(money)
        {}

        std::string GetName() const
        {
            return mName;
        }

        char GetSprite() const
        {
            return mSprite;
        }

        int GetYCoordinates() const
        {
            return mYPos;
        }

        int GetXCoordinates() const
        {
            return mXPos;
        }

        void SetCoordinates(int posY, int posX)
        {
            mYPos = posY;
            mXPos = posX;
        }

        void TakeTreasure(const Treasure& treasure, int amount)
        {
            auto findDuplicate = [&treasure](const std::pair<Treasure, int>& a)
                {
                    return treasure.GetName() == a.first.GetName();
                };

            auto itr = std::find_if(mTreasureList.begin(), mTreasureList.end(), findDuplicate);

            if (itr != mTreasureList.end())
            {
                itr->second += amount;
            }
            else
            {
                mTreasureList.push_back(std::make_pair(treasure, amount));
            }
        }

        void ViewTreasure() const
        {
            if (mTreasureList.empty())
            {
                std::cout << "I don't have any treasure yet.\n";
                return;
            }

            for (auto& i : mTreasureList)
            {
                std::cout << i.first.GetName() << "x(" << i.second << ")\n";
            }
        }

        void Dig(Map& map)
        {
            int y = GetYCoordinates();
            int x = GetXCoordinates();

            if (!map.IsDug(y, x))
            {
                map.MarkAsDug(y, x);
                std::cout << "You dug a hole at (" << y << ", " << x << ").\n";
                --mDigsLeft;
            }
            else
            {
                std::cout << "This spot has already been dug.\n";
            }
        }

        int GetScore() const
        {
            return mScore;
        }

        int GetDigsLeft() const
        {
            return mDigsLeft;
        }

        double GetMoney() const
        {
            return mMoney;
        }

        void AddMoney(int amount)
        {
            if (amount > 0)
            {
                mMoney += amount;
            }
        }

        void SubtractMoney(int amount)
        {
            if (amount > 0)
            {
                mMoney -= amount;

                if (mMoney < 0)
                {
                    mMoney = 0;
                }
            }
        }

    private:
        std::string mName{ "Character Name" };
        char mSprite{};
        int mYPos{ 0 };
        int mXPos{ 0 };
        int mScore{ 0 };
        int mDigsLeft{ 10 };
        double mMoney{ 0.00L };

        std::vector<std::pair<Treasure, int>> mTreasureList{};
};

class Collision
{
    public:
        bool IsMoveValid(Character& character, const Map& map, char direction)
        {
            int assesNextMoveY = character.GetYCoordinates();
            int assesNextMoveX = character.GetXCoordinates();

            // Predict the new coordinates
            switch (direction)
            {
                case 'W': case 'w': 
                    assesNextMoveY--;
                    break;

                case 'S': case 's': 
                    assesNextMoveY++;
                    break;

                case 'A': case 'a': 
                    assesNextMoveX--;
                    break;

                case 'D': case 'd': 
                    assesNextMoveX++;
                    break;

                default:
                    std::cout << "Invalid direction.\n";
                    return false;
            }

            if (!IsWithinMapBounds(assesNextMoveY, assesNextMoveX, map))
            {
                return false;
            }

            if (IsCollidableObject(assesNextMoveY, assesNextMoveX, map))
            {
                return false;
            }

            return true;
        }

    private:
        bool IsWithinMapBounds(int proposedY, int proposedX, const Map& map)
        {
            return proposedY >= 0 && proposedY < map.GetHeight() &&
                proposedX >= 0 && proposedX < map.GetWidth();
        }

        bool IsCollidableObject(int proposedY, int proposedX, const Map& map)
        {
            return map.IsCollidable(proposedY, proposedX);
        }
};


const enum class DialogueID
{
    Prosp_0001, //Welcoms player
    Shop_0001 //Opening shop dialogue
};

void Dialogue(const DialogueID& ID);

int main()
{
    Map start("Starting Area", 10, 10, '.');
    Character player("Player", '*', 0, 25, 10.50);
    Collision collision;

    start.Initialize();

    // Add some collidable objects
    start.PlaceOnMap(2, 2, 'T', true); // Tree at (2,2)
    start.PlaceOnMap(5, 5, 'R', true); // Rock at (5,5)
    start.PlaceOnMap(6, 6, 'Z', true);

    bool isGameOver{ false };

    while (!isGameOver)
    {
        std::cout << "Score: " << player.GetScore() << '\n';
        std::cout << "Money: " << std::format("{:.2f}", player.GetMoney()) << '\n';
        std::cout << "Digs Left: " << player.GetDigsLeft() << "\n\n";

        int lastY{ player.GetYCoordinates() };
        int lastX{ player.GetXCoordinates() };

        start.PlaceOnMap(player.GetYCoordinates(), player.GetXCoordinates(), player.GetSprite());

        start.DrawMap();

        std::cout << '\n';
        std::cout << "W). Up\n";
        std::cout << "S). Down\n";
        std::cout << "A). Left\n";
        std::cout << "D). Right\n\n";

        std::cout << "1). View Treasure\n";
        std::cout << "2). Dig\n";
        std::cout << "3). Shop\n"; //Not Implemented
        std::cout << "4). Exit\n"; //Not Implemented
        std::cout << ">";

        char selection{ ' ' };
        std::cin >> selection;

        if (selection == '1')
        {
            player.ViewTreasure();
        }
        else if (selection == '2')
        {
            if (player.GetDigsLeft() > 0)
            {
                player.Dig(start);
            }
            else
            {
                //Not implemented this part of the game, but this dialogue will appear for now anyways.
                std::cout << "You are out of digs! Return to the prospector for another chance.\n";
            }
        }
        else if (selection == '3')
        {
            std::cout << "Not implemented yet.\n\n";
        }
        else if (selection == '4')
        {
            std::cout << "Goodbye.\n";
            return 0;
        }

        if(collision.IsMoveValid(player, start, selection))
        {
            // Update position if the move is valid
            switch (selection)
            {
                case 'W': case 'w': 
                    player.SetCoordinates(player.GetYCoordinates() - 1, player.GetXCoordinates());
                    break;

                case 'S': case 's': 
                    player.SetCoordinates(player.GetYCoordinates() + 1, player.GetXCoordinates());
                    break;

                case 'A': case 'a': 
                    player.SetCoordinates(player.GetYCoordinates(), player.GetXCoordinates() - 1);
                    break;

                case 'D': case 'd': 
                    player.SetCoordinates(player.GetYCoordinates(), player.GetXCoordinates() + 1);
                    break;
            }

            //Reset map tile back to default after player walks over it.
            start.PlaceOnMap(lastY, lastX, start.GetTile());
        }
        else
        {
            std::cout << "You cannot proceed in that direction. Please turn back.\n";
        }
    }
}


void Dialogue(const DialogueID& ID)
{
    switch (ID)
    {
        case DialogueID::Prosp_0001:
            std::cout << "Welcome to the mining site pal, heres a pickaxe, on the house this time.\n";
            std::cout << "it only lasts a short while so try to make it count out there!\n";
            break;

        case DialogueID::Shop_0001:
            std::cout << "Welcome to the shop, what would you like to buy?\n";
            break;

        default:
            std::cout << "Unable to obtain dialogue ID.\n\n";
    }
}
#ifndef HUD_H
#define HUD_H

#include "Core/Globals.hpp"
#include "Core/SceneNode.hpp"
#include "Core/TextNode.hpp"
#include "Core/SpriteNode.hpp"
#include "Core/State.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/ResourceIdentifiers.hpp"
#include "Core/DataTables.hpp"

class HUD : public SpriteNode
{
public:
    HUD( TextureManager &tex, FontManager& Fonts );
    void updateCurrent( float dt, CommandQueue& commands );
    void drawCurrent( sf::RenderTarget& target, sf::RenderStates states );
    void updateText(int distance, int score, int troopCount, int fuel , int health , unsigned int lives );
    void setHighScore( unsigned int highscore );
private:
    TextNode* mDistanceDisplay;
    TextNode* mFuelLevelDisplay;
    TextNode* mTroopCountDisplay;
    TextNode* mScoreDisplay;
    TextNode* mHighScoreDisplay;
    TextNode* mHealthWarning;
    TextNode* mNumLivesDisplay;

    bool isFlipped;
    bool hasMaxTroops;
    bool fuelLow;
    bool isDamaged;
    unsigned int highScore;
    unsigned int numLives;
};

#endif // HUD_H

#include "HUD.hpp"



HUD::HUD( TextureManager& tex, FontManager& fonts)
    : SpriteNode( tex.get( TextureMap.at( "HUDBackground" ) ) )
    , isFlipped( false )
    , hasMaxTroops( false )
    , fuelLow( false )
    , isDamaged( false )
{

    setOrigin( WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 );
    setPosition( WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

    std::unique_ptr<TextNode> highScore( new TextNode( fonts, "" ) );
    mHighScoreDisplay = highScore.get();
    mHighScoreDisplay->setColor( sf::Color( 79, 67, 174, 255 ) );
    mHighScoreDisplay->setPosition( 100, 15 );
    attachChild( std::move( highScore ) );

    std::unique_ptr<TextNode> score( new TextNode( fonts, "" ) );
    mScoreDisplay = score.get();
    mScoreDisplay->setColor( sf::Color( 79, 67, 174, 255 ) );
    mScoreDisplay->setPosition( 100, 40 );
    attachChild( std::move( score ) );

    std::unique_ptr<TextNode> distance( new TextNode( fonts, "" ) );
    mDistanceDisplay = distance.get();
    mDistanceDisplay->setColor( sf::Color( 79, 67, 174, 255 ) );
    mDistanceDisplay->setPosition( WINDOW_WIDTH - 75, 15 );
    attachChild( std::move( distance ) );

    std::unique_ptr<TextNode> lives( new TextNode( fonts, "" ) );
    mNumLivesDisplay = lives.get();
    mNumLivesDisplay->setColor( sf::Color( 79, 67, 174, 255 ) );
    mNumLivesDisplay->setPosition( WINDOW_WIDTH - 75, 40 );
    attachChild( std::move( lives ) );

    std::unique_ptr<TextNode> troopCount( new TextNode( fonts, "" ) );
    mTroopCountDisplay = troopCount.get();
    mTroopCountDisplay->setColor( sf::Color( 79, 67, 174, 255 ) );
    mTroopCountDisplay->setPosition( WINDOW_WIDTH / 2, 15 );
    attachChild( std::move( troopCount ) );

    std::unique_ptr<TextNode> fuel( new TextNode( fonts, "" ) );
    mFuelLevelDisplay = fuel.get();
    mFuelLevelDisplay->setColor( sf::Color( 79, 67, 174, 255 ) );
    mFuelLevelDisplay->setPosition( WINDOW_WIDTH / 2, 40 );
    attachChild( std::move( fuel ) );

    std::unique_ptr<TextNode> warning( new TextNode( fonts, "WARNING!\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\tDAMAGE CRITICAL!" ) );
    mHealthWarning = warning.get();
    mHealthWarning->setColor( sf::Color( 255, 0, 0, 0 ) );
    mHealthWarning->setPosition( 525, 25 );
    attachChild( std::move( warning ) );
}

void HUD::updateCurrent( float, CommandQueue&  )
{
}

void HUD::drawCurrent( sf::RenderTarget&, sf::RenderStates )
{
}

void HUD::updateText( int distance, int score, int troopCount, int fuel, int health, unsigned int lives )
{
    if( FLIP_GAMEPLAY && !ALREADY_FLIPPED )
    {
        mScoreDisplay->setRotation( 180.0f );
        mTroopCountDisplay->setRotation( 180.0f );
        mDistanceDisplay->setRotation( 180.0f );
        mHighScoreDisplay->setRotation( 180.0f );
        mFuelLevelDisplay->setRotation( 180.0f );
        mHealthWarning->setRotation( 180.0f );
        mNumLivesDisplay->setRotation( 180.0f );
    }
    else if( !FLIP_GAMEPLAY && ALREADY_FLIPPED )
    {
        mScoreDisplay->setRotation( 0.0f );
        mTroopCountDisplay->setRotation( 0.0f );
        mDistanceDisplay->setRotation( 0.0f );
        mHighScoreDisplay->setRotation( 0.0f );
        mFuelLevelDisplay->setRotation( 0.0f );
        mHealthWarning->setRotation( 0.0f );
        mNumLivesDisplay->setRotation( 0.0f );
    }
    mScoreDisplay->setString(       "Score: "       + std::to_string( score         ) );
    mTroopCountDisplay->setString(  "Troops: "      + std::to_string( troopCount    ) );
    mDistanceDisplay->setString(    "Distance: "    + std::to_string( distance      ) );
    mFuelLevelDisplay->setString(   "Fuel: "        + std::to_string( fuel          ) );
    mNumLivesDisplay->setString(    "Lives: "       + std::to_string( lives      ) );

    if( !hasMaxTroops && troopCount == 3 )
    {
        mTroopCountDisplay->setColor( sf::Color::Blue );
        hasMaxTroops = true;
    }
    else if( hasMaxTroops && troopCount != 3 )
    {
        mTroopCountDisplay->setColor( sf::Color( 79, 67, 174, 255 ) );
        hasMaxTroops = false;
    }
    if( !fuelLow && fuel < 26 )
    {
        mFuelLevelDisplay->setColor( sf::Color::Red );
        fuelLow = true;
    }
    else if( fuelLow && fuel > 25 )
    {
        mFuelLevelDisplay->setColor( sf::Color( 79, 67, 174, 255 ) );
        fuelLow = false;
    }
    if( health <= 10.0f && !isDamaged)
    {
        mHealthWarning->setColor( sf::Color( 255, 0, 0, 255 ) );
        isDamaged = true;
    }
    else if( health > 10 && isDamaged )
    {
        mHealthWarning->setColor( sf::Color( 255, 0, 0, 0 ) );
        isDamaged = false;
    }
    if( (unsigned int)score > highScore )
        setHighScore( (unsigned int)score );
}

void HUD::setHighScore( unsigned int highScore )
{
    this->highScore = highScore;
    mHighScoreDisplay->setString( "HighScore: " + std::to_string( this->highScore ) );
}

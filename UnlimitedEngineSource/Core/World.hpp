#ifndef WORLD_HPP
#define WORLD_HPP

#include "Core/ResourceManager.hpp"
#include "Core/ResourceIdentifiers.hpp"

#include "Core/SceneNode.hpp"
#include "Core/SpriteNode.hpp"
#include "Core/CommandQueue.hpp"
#include "Core/Command.hpp"
#include "Core/State.hpp"
#include "Core/PostEffect.hpp"
#include "Core/BloomEffect.hpp"
#include "Core/Globals.hpp"

#include "Objects/Aircraft.hpp"
#include "Objects/Pickup.hpp"
#include "Objects/Wall.hpp"
#include "Objects/HUD.hpp"

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <array>
#include <queue>


// Forward declaration
namespace sf
{
    class RenderTarget;
}

struct SpawnPoint
{
    SpawnPoint( unsigned int type, float x, float y)
    : type(type)
    , x(x)
    , y(y)
    {
    }

    unsigned int type;
    float x;
    float y;
};

static std::vector<SpawnPoint> EnemySpawnPoints;

///
/// \brief The World class
/// The Game world houses everything happening in the game including the SceneGraph and reference
/// to the player. The world class wil be a state machine for all the levels in the game.
/// The World is owned by the Game class and gets updated etc.. from there
class World : private sf::NonCopyable
{
public:
    explicit World( sf::RenderTarget& outputTarget, TextureManager& textures, FontManager& fonts, MusicPlayer& music, SoundPlayer &sounds, State::Context context );
    ~World( );

    void	 update( sf::Time dt );
    void	 draw( );

    CommandQueue& getCommandQueue( );

    bool hasAlivePlayer( ) const;

protected:
    void loadTextures( );
    void adaptPlayerPosition( );

    void handleCollisions( );
    void buildScene( );
    /*
    void addEnemies();
    static void addEnemy(unsigned int type, float relX, float relY);
    void spawnEnemies();
    */
    void destroyEntitiesOutsideView( );

    void guideMissiles( );
    sf::FloatRect getViewBounds( ) const;
    sf::FloatRect getBattlefieldBounds( ) const;

    enum Layer
    {
        Background,
        LowerAir,
        UpperAir,
        Text,
        LayerCount
    };

    sf::RenderTarget&					mTarget;
    sf::RenderTexture					mSceneTexture;
    sf::View							mWorldView;
    TextureManager&						mTextures;
    FontManager&						mFonts;
    MusicPlayer&                        mMusic;
    SoundPlayer&						mSounds;

    State::Context                      mContext;
    sf::Sprite                          mWindowSprite;


    SceneNode							mSceneGraph;
    std::array<SceneNode*, LayerCount>	mSceneLayers;
    CommandQueue						mCommandQueue;

    Aircraft*							mPlayerAircraft;

    std::vector<Aircraft*>				mActiveEnemies;

    BloomEffect							mBloomEffect;
    std::queue<std::vector<Wall*>>      mLeftWalls;
    std::queue<std::vector<Wall*>>      mRightWalls;
    sf::Time                            mUpdateDepthCounter;
    sf::Time                            mSpawnChopperTimer;
    SpriteNode*                         mBackgroundSpriteA;
    SpriteNode*                         mBackgroundSpriteB;
    HUD*                                mHud;
    bool                                mAlarmPlaying;
    sf::Time                            mAlarmTimer;

};

#endif // WORLD_HPP

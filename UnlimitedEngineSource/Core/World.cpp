 #include "Engine/World.hpp"
#include "Objects/Projectile.hpp"
#include "Objects/Pickup.hpp"
#include "TextNode.hpp"
#include "DataTables.hpp"
#include "State.hpp"
#include "ParticleNode.hpp"
#include "Engine/Player.hpp"
#include "Engine/MusicPlayer.hpp"
#include "Engine/SoundPlayer.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp> // debugging only erase me!!

#include <algorithm>
#include <cmath>
#include <limits>

static sf::Vector2f SpawnPosition;
unsigned int        WALL_DEPTH          = 3;
float               GAME_SPEED          = 200;
bool                FLIP_SCREEN         = false;
bool                FLIP_GAMEPLAY       = false;
bool                ALREADY_FLIPPED     = false;
int                 PLAYER_SCORE        = 0;
float               BASE_SPEED          = -150;
int                 GAME_LEVEL          = 1;

World::World( sf::RenderTarget &outputTarget, TextureManager &textures, FontManager& fonts, MusicPlayer& music, SoundPlayer& sounds, State::Context context )
: mTarget( outputTarget )
, mSceneTexture( )
, mWorldView( outputTarget.getDefaultView( ) )
, mTextures( textures )
, mFonts( fonts )
, mMusic( music )
, mSounds( sounds )
, mContext( context )
, mWindowSprite(  )
, mSceneGraph( )
, mSceneLayers( )
, mPlayerAircraft( nullptr )
, mSpawnChopperTimer( sf::Time::Zero )
, mAlarmPlaying( false )
,mAlarmTimer( sf::Time::Zero )
{
    WALL_DEPTH = 3;
    GAME_SPEED = 100;
    FLIP_SCREEN = false;
    FLIP_GAMEPLAY = false;
    ALREADY_FLIPPED = false;
    PLAYER_SCORE = 0;
    BASE_SPEED = -150;
    GAME_LEVEL = 1;

    srand( time( nullptr ) );
    SpawnPosition.x =  mWorldView.getCenter( ).x;
    SpawnPosition.y =  mWorldView.getCenter( ).y + 300;
    if( !mSceneTexture.create( mTarget.getSize( ).x, mTarget.getSize( ).y ) ) std::cout << "Render ERROR" << std::endl;
    mSceneTexture.setView( mWorldView );
    buildScene( );
}

World::~World( )
{

}

void World::update( sf::Time dt )
{
    if( FLIP_GAMEPLAY && !ALREADY_FLIPPED )
    {
        ALREADY_FLIPPED = true;
        FLIP_SCREEN = true;
    }
    else if( ALREADY_FLIPPED && !FLIP_GAMEPLAY )
    {
        FLIP_SCREEN = true;
        ALREADY_FLIPPED = false;
    }

    if( FLIP_GAMEPLAY && mPlayerAircraft->getDistance() == 0 )
    {
        if( mPlayerAircraft->getTroopCount() > 0 )
        {
            GAME_SPEED = 0;
            FLIP_GAMEPLAY = false;
            if( mPlayerAircraft->getTroopCount() == 5 )
                PLAYER_SCORE += 5000;
            PLAYER_SCORE += mPlayerAircraft->getTroopCount() * 1000;
            GAME_LEVEL++;
            BASE_SPEED = -150 - GAME_LEVEL * 20;
            mPlayerAircraft->resetTroopCount( );
            mPlayerAircraft->increaseFuel( 100 );
            mPlayerAircraft->repair( 100 );
            mContext.player->setMissionStatus( Player::MissionSuccess );
            mContext.music->setPaused( true );
            mContext.sounds->play( SoundEffectMap.at( "ReturnToBase" ), mPlayerAircraft->getPosition(), 100 );
        }
        else // troopCount == 0
        {
            mPlayerAircraft->kill( );
        }
    }
    else GAME_SPEED = mPlayerAircraft->getVelocity().y * -1;

    if( mPlayerAircraft->getDistance() < 30 )
    {
        WALL_DEPTH = 1;
    }
    else if( mUpdateDepthCounter > sf::Time::Zero )
        mUpdateDepthCounter -= dt;
    else
    {
        mUpdateDepthCounter = sf::milliseconds( (rand() % 3000) + 500 ); // NEED TO UN-HARDCODE THESE VALUES!!!!!
        int random = rand() % 3 + 1;
        switch( random )
        {
            case 1: WALL_DEPTH++; break;
            case 2: WALL_DEPTH--; break;
            case 3:
                WALL_DEPTH = WALL_DEPTH + (rand() % 6 + 1) - 3;
            break;
        }
        //WALL_DEPTH = WALL_DEPTH + (rand() % 6 + 1) - 3;
        if( WALL_DEPTH > 6) WALL_DEPTH = 6;
        if( WALL_DEPTH < 1 ) WALL_DEPTH = 1;
    }

    if( mLeftWalls.back().front()->getPosition().y > 0 )
    {
        if( FLIP_GAMEPLAY ) mPlayerAircraft->decreaseDistance();
        else mPlayerAircraft->increaseDistance();

        PLAYER_SCORE += GAME_SPEED / 10;
        BASE_SPEED += ( BASE_SPEED * GAME_LEVEL * .001 );

        std::vector<Wall*> leftRow;
        std::vector<Wall*> rightRow;

        for( unsigned int j = 0; j < WALL_DEPTH; ++j )
        {
            std::unique_ptr<Wall> leftWall( new Wall( mTextures ) );
            std::unique_ptr<Wall> rightWall( new Wall( mTextures ) );

            leftWall->setPosition( (j * 64), mLeftWalls.back().front()->getPosition().y - 64 );
            rightWall->setPosition( WINDOW_WIDTH - (j+1)*64, mLeftWalls.back().front()->getPosition().y - 64 );

            leftRow.push_back( leftWall.get( ) );
            rightRow.push_back( rightWall.get( ) );

            // if depth farther then farthest tree in prevous row
            if( j == WALL_DEPTH - 1 || j > mLeftWalls.back().size() - 1 )
            {
                leftWall.get( )->setHasCollision( true );
                rightWall.get( )->setHasCollision( true );
                mSceneLayers[UpperAir]->attachChild( std::move( leftWall ) );
                mSceneLayers[UpperAir]->attachChild( std::move( rightWall ) );
            }
            else
            {
                // These trees don't have collision becuase nothing should ever be able to colide with them through the first row of trees.
                leftWall.get()->setHasCollision( false );
                rightWall.get()->setHasCollision( false );
                mSceneLayers[LowerAir]->attachChild( std::move( leftWall ) );
                mSceneLayers[LowerAir]->attachChild( std::move( rightWall ) );
            }
        }
        mLeftWalls.push( leftRow );
        mRightWalls.push( rightRow );
        mLeftWalls.pop( );
        mRightWalls.pop( );

        if( mPlayerAircraft->getDistance( ) > 30 )
        {
            int generator = rand( ) % (500 - GAME_LEVEL * 20 );

            if( generator < 10 )
            {
                if( !FLIP_GAMEPLAY )
                {
                    std::unique_ptr<Pickup> ally( new Pickup( Pickup::Ally, mTextures ) );
                    if( rand() % 2 )
                        ally->setPosition( mLeftWalls.back( ).back( )->getPosition( ).x + 100, mLeftWalls.back().back( )->getPosition( ).y );
                    else
                        ally->setPosition( mRightWalls.back( ).back( )->getPosition( ).x - 100, mLeftWalls.back( ).back( )->getPosition().y );
                    mSceneLayers[UpperAir]->attachChild( std::move( ally ) );
                }
            }
            else if( generator < 15 )
            {
                std::unique_ptr<Pickup> gas( new Pickup( Pickup::Fuel, mTextures ) );
                gas->setPosition( mPlayerAircraft->getPosition( ).x, -64 );
                mSceneLayers[UpperAir]->attachChild( std::move( gas ) );
            }
            else if( generator < 25 )
            {
                // Create Bad Guy at the top
                if( rand( ) % 2 )
                {
                    std::unique_ptr<Aircraft> badGuy( new Aircraft( AircraftMap.at( "GreenTank" ), mTextures, &mSounds, mFonts ) );
                    badGuy->setPosition( leftRow.back( )->getPosition().x + 90, leftRow.back( )->getPosition( ).y + 32 );
                    if( mPlayerAircraft->getPosition( ).x < badGuy->getPosition( ).x )
                        badGuy->setVelocity( -1 * badGuy->getVelocity( ).x, badGuy->getVelocity( ).y );
                    mSceneLayers[UpperAir]->attachChild( std::move( badGuy ) );
                }
                else
                {
                    std::unique_ptr<Aircraft> badGuy( new Aircraft( AircraftMap.at( "GreenTank" ), mTextures, &mSounds, mFonts ) );
                    badGuy->setPosition( rightRow.back()->getPosition().x - 30, rightRow.back()->getPosition().y + 32 );
                    if( mPlayerAircraft->getPosition().x < badGuy->getPosition().x )
                        badGuy->setVelocity( -1 * badGuy->getVelocity().x, badGuy->getVelocity().y );
                    mSceneLayers[UpperAir]->attachChild( std::move( badGuy ) );
                }
            }
            else if( generator < 30 )
            {
                std::unique_ptr<Aircraft> badGuy( new Aircraft( AircraftMap.at( "Hellcat" ), mTextures, &mSounds, mFonts ) );
                badGuy->setPosition( mWorldView.getCenter().x, -64 );
                mSceneLayers[UpperAir]->attachChild( std::move( badGuy ) );
            }
            else if( generator < 35 )
            {
                if( mSpawnChopperTimer <= sf::Time::Zero )
                {
                    std::unique_ptr<Aircraft> badGuy( new Aircraft( AircraftMap.at( "Chopper" ), mTextures, &mSounds, mFonts ) );
                    if( rand() % 2 )
                        badGuy->setPosition( mWorldView.getCenter().x - 100, mLeftWalls.back().back()->getPosition().y );
                    else
                        badGuy->setPosition( mWorldView.getCenter().x + 100, mLeftWalls.back().back()->getPosition().y );
                    mSceneLayers[UpperAir]->attachChild( std::move( badGuy ) );
                    mSpawnChopperTimer = sf::milliseconds( 5000.0f );
                }
            }
        }
    }
    if( mSpawnChopperTimer > sf::Time::Zero )
        mSpawnChopperTimer -= dt;


    mBackgroundSpriteA->move( 0, GAME_SPEED * dt.asSeconds( ) );
    mBackgroundSpriteB->move( 0, GAME_SPEED * dt.asSeconds( ) );

    if( mBackgroundSpriteA->getPosition().y > WINDOW_HEIGHT + 10 )
        mBackgroundSpriteA->setPosition( 0, mBackgroundSpriteB->getPosition().y - 1024 );
    else if( mBackgroundSpriteB->getPosition().y > WINDOW_HEIGHT + 10 )
        mBackgroundSpriteB->setPosition( 0, mBackgroundSpriteA->getPosition().y - 1024 );

    // mPlayerAircraft->setVelocity( mPlayerAircraft->getVelocity( ) );

    if( mPlayerAircraft->getVelocity( ).y < -350 )
    {
        mPlayerAircraft->setVelocity( mPlayerAircraft->getVelocity( ).x, -350 );
    }
    else if( mPlayerAircraft->getVelocity( ).y > 0 )
    {
        mPlayerAircraft->setVelocity( mPlayerAircraft->getVelocity( ).x, 0 );
    }
    else if( mPlayerAircraft->getVelocity().y == 0 )
    {
        mPlayerAircraft->setVelocity( mPlayerAircraft->getVelocity().x, BASE_SPEED );
    }

    if( mPlayerAircraft->getVelocity( ).x < -1 * mPlayerAircraft->getMaxSpeed( ) )
    {
        mPlayerAircraft->setVelocity( mPlayerAircraft->getMaxSpeed() * -1, mPlayerAircraft->getVelocity().y );
    }
    else if( mPlayerAircraft->getVelocity().x > mPlayerAircraft->getMaxSpeed( ) )
    {
        mPlayerAircraft->setVelocity( mPlayerAircraft->getMaxSpeed( ), mPlayerAircraft->getVelocity().y );
    }

    // Setup commands to destroy entities, and guide missiles
    destroyEntitiesOutsideView( );
    guideMissiles( ); // will be added to infantry badguys that shoot homing missiles at players

    // Forward commands to scene graph, adapt velocity (scrolling, diagonal correction)
    while ( !mCommandQueue.isEmpty( ) )
        mSceneGraph.onCommand(mCommandQueue.pop( ), dt );

    // Collision detection and response (may destroy entities)
    handleCollisions( );

    // Remove all destroyed entities, create new ones
    mSceneGraph.removeWrecks( );
    //spawnEnemies( );

    // Regular update step, adapt position (correct if outside view)
    mSceneGraph.update( dt, mCommandQueue );
    adaptPlayerPosition( );

    mHud->updateText( mPlayerAircraft->getDistance( ), PLAYER_SCORE, mPlayerAircraft->getTroopCount( ), mPlayerAircraft->getFuelLevel( ), mPlayerAircraft->getHitpoints( ) );

    if( !mAlarmPlaying )
    {
        if( mPlayerAircraft->getHitpoints( ) <= 10 || mPlayerAircraft->getFuelLevel( ) < 26 )
        {
            mAlarmTimer = sf::seconds( .1f );
            mAlarmPlaying = true;
        }
    }
    else
    {
        if( mPlayerAircraft->getHitpoints() <= 10 || mPlayerAircraft->getFuelLevel( ) < 26 )
        {
            mAlarmTimer -= dt;
            if( mAlarmTimer <= sf::Time::Zero )
            {
                mContext.sounds->play( SoundEffectMap.at( "FuelLow" ), mPlayerAircraft->getPosition( ), 80, false );
                mAlarmTimer = sf::seconds( 3.0f );
            }
        }
        if( mPlayerAircraft->getHitpoints() > 10 && mPlayerAircraft->getFuelLevel() > 25 )
        {
            mAlarmPlaying = false;
        }
    }

    mSounds.setListenerPosition( mPlayerAircraft->getPosition( ) );
}

void World::draw( )
{
    if( PostEffect::isSupported( ) )
    {
        mSceneTexture.clear( sf::Color( 34, 139, 34 ) );

        if( FLIP_SCREEN )
        {
            mWorldView.rotate( 180.0f );
            FLIP_SCREEN = false;
        }

        mSceneTexture.setView( mWorldView );
        mSceneTexture.draw( mSceneGraph );

        mSceneTexture.display( );

        mWindowSprite.setTexture( mSceneTexture.getTexture( ) );

        mTarget.draw( sf::Sprite( mWindowSprite ) );
        mBloomEffect.apply( mSceneTexture, mTarget );
    }
    else
    {
        mTarget.setView( mWorldView );
        mTarget.draw( mSceneGraph );
    }
}

CommandQueue& World::getCommandQueue( )
{
    return mCommandQueue;
}

bool World::hasAlivePlayer( ) const
{
    return !mPlayerAircraft->isMarkedForRemoval( );
}

void World::adaptPlayerPosition( )
{
    mPlayerAircraft->setPosition( mPlayerAircraft->getPosition().x, WINDOW_HEIGHT - 50 );
}

bool matchesCategories( std::pair<SceneNode*, SceneNode*>& colliders, Category::Type type1, Category::Type type2 )
{
    unsigned int category1 = colliders.first->getCategory( );
    unsigned int category2 = colliders.second->getCategory( );

    // Make sure first pair entry has category type1 and second has type2
    if( type1 & category1 && type2 & category2 )
    {
        return true;
    }
    else if ( type1 & category2 && type2 & category1 )
    {
        std::swap( colliders.first, colliders.second );
        return true;
    }
    else
    {
        return false;
    }
}

void World::handleCollisions( )
{
    std::set<std::pair<SceneNode*, SceneNode*>> collisionPairs;
    mSceneGraph.checkSceneCollision( *mSceneLayers[UpperAir], collisionPairs );

    for( std::pair<SceneNode*, SceneNode*> pair : collisionPairs )
    {
        if( matchesCategories( pair, Category::PlayerAircraft, Category::EnemyAircraft ) )
        {
            auto& player = static_cast<Aircraft&>( *pair.first );
            auto& enemy = static_cast<Aircraft&>( *pair.second );

            // Collision: Player damage = enemy's remaining HP
            player.damage( player.getHitpoints( ) );
            enemy.destroy( );
        }
        else if( matchesCategories( pair, Category::PlayerAircraft, Category::Pickup ) )
        {
            auto& player = static_cast<Aircraft&>( *pair.first );
            auto& pickup = static_cast<Pickup&>( *pair.second );

            if( pickup.getType() == (unsigned int)Pickup::Ally )
                mContext.sounds->play( SoundEffectMap.at( "CollectAlly" ), mPlayerAircraft->getPosition( ), 20 );
            else
                mContext.sounds->play( SoundEffectMap.at( "CollectPickup" ), mPlayerAircraft->getPosition( ), 100  );

            // Apply pickup effect to player, destroy projectile
            pickup.apply( player );
            pickup.destroy( );
            PLAYER_SCORE += 500;
        }

        else if( matchesCategories( pair, Category::EnemyAircraft, Category::AlliedProjectile )
              || matchesCategories( pair, Category::PlayerAircraft, Category::EnemyProjectile ) )
        {
            auto& aircraft = static_cast<Aircraft&>( *pair.first );
            auto& projectile = static_cast<Projectile&>( *pair.second );

            if( aircraft.getCategory() == Category::EnemyAircraft )
                PLAYER_SCORE += 1000;

            // Apply projectile damage to aircraft, destroy projectile
            aircraft.damage( projectile.getDamage( ) );
            projectile.destroy( );
        }

        else if( matchesCategories( pair, Category::PlayerAircraft, Category::Tree ) )
        {
            auto& player = static_cast<Aircraft&>(*pair.first);

            // Collision: Player damage = enemy's remaining HP
            player.damage( player.getHitpoints() );
        }

        else if( matchesCategories( pair, Category::Projectile, Category::Tree ) )
        {
            auto& projectile = static_cast<Projectile&>(*pair.first);
            projectile.destroy( );
        }
        else if( matchesCategories( pair, Category::EnemyAircraft, Category::Tree ) )
        {
            auto& aircraft = static_cast<Aircraft&>( *pair.first );
            auto& tree      = static_cast<Wall&> (* pair.second );
            if( tree.getPosition().x > aircraft.getPosition().x )
                aircraft.setVelocity( -1 * aircraft.getMaxSpeed( ), aircraft.getVelocity().y );
            else
                aircraft.setVelocity( aircraft.getMaxSpeed( ), aircraft.getVelocity().y );
        }
        else if( matchesCategories( pair, Category::EnemyAircraft, Category::EnemyAircraft ) )
        {
            auto& aircraft = static_cast<Aircraft&>( *pair.first );
            auto& aircraft2      = static_cast<Aircraft&> (* pair.second );
            if( aircraft2.getPosition().x > aircraft.getPosition( ).x )
            {
                aircraft.setVelocity( -1 * aircraft.getMaxSpeed( ), aircraft.getVelocity( ).y );
                aircraft2.setVelocity( aircraft2.getMaxSpeed( ), aircraft2.getVelocity( ).y );
            }
            else
            {
                aircraft.setVelocity( aircraft.getMaxSpeed( ), aircraft.getVelocity().y );
                aircraft2.setVelocity( -1 * aircraft2.getMaxSpeed( ), aircraft2.getVelocity().y );
            }
        }
    }
}

void World::buildScene( )
{
    // Initialize the different layers
    for( std::size_t i = 0; i < LayerCount; ++i )
    {
        Category::Type category = ( i == LowerAir ) ? Category::SceneAirLayer : Category::None;

        SceneNode::Ptr layer( new SceneNode( category ) );
        mSceneLayers[i] = layer.get( );

        mSceneGraph.attachChild( std::move( layer ) );
    }

    lua_State* L = luaL_newstate( );
    luaL_openlibs( L );

    // call with error checking
    lua_getglobal( L, "debug" );
    lua_getfield( L, -1, "traceback" );
    lua_replace( L, -2 );

    luaL_loadfile( L, "Game/Game.lua" );
    if ( lua_pcall( L, 0, LUA_MULTRET, -2 ) ) {
        luaL_traceback( L, L, lua_tostring( L, -1 ), 1 );
        std::cout << "ERROR: " << lua_tostring( L, -1 ) << std::endl;
    }
    if( lua_istable( L, -1 ) )
    {
        lua_getfield( L, -1, "Particles" );
        if( lua_istable( L, -1 ) )
        {
            lua_pushnil( L );
            while( lua_next( L, -2 ) != 0 )
            {
                std::unique_ptr<ParticleNode> node( new ParticleNode( ParticleMap.at( lua_tostring( L, -1 ) ), mTextures ) );
                mSceneLayers[LowerAir]->attachChild( std::move( node ) );
                lua_pop( L, 1 );
            }
        }
        lua_pop( L, 1 ); // Particles table

        lua_getfield( L, -1, "PlayerTexture" );
        if( lua_isstring( L, -1 ) )
        {
            // Add player's aircraft
            std::unique_ptr<Aircraft> player( new Aircraft( AircraftMap.at( lua_tostring( L, -1 ) ), mTextures, &mSounds, mFonts ) );
            mPlayerAircraft = player.get( );
            mPlayerAircraft->setPosition( SpawnPosition );
            mSceneLayers[UpperAir]->attachChild(std::move( player ) );
        }else std::cout << "Error Reading background Texture! World::BuildScene( )" << std::endl;
        lua_pop( L, 1 );
    }
    lua_close( L );

    // Prepare the tiled background
    sf::Texture& texture = mTextures.get( TextureMap.at( "Grass" ) );
    sf::IntRect textureRect( 0, 0, 1024, 1024 );
    texture.setRepeated( true );

    // Add the background sprite to the scene
    std::unique_ptr<SpriteNode> backgroundSprite( new SpriteNode( texture, textureRect ) );
    mBackgroundSpriteA = backgroundSprite.get( );
    mBackgroundSpriteA->setPosition( 0, 0 );
    mSceneLayers[Background]->attachChild( std::move( backgroundSprite ) );

    std::unique_ptr<SpriteNode> backgroundSpriteB( new SpriteNode( texture, textureRect ) );
    mBackgroundSpriteB = backgroundSpriteB.get( );
    mBackgroundSpriteB->setPosition( 0, -1000 );
    mSceneLayers[Background]->attachChild( std::move( backgroundSpriteB ) );

    // Helipad under player at beginning

    // Walls up both sides
    std::vector<Wall*> leftRow;
    std::vector<Wall*> rightRow;
    for( unsigned int i = 0; i < WINDOW_HEIGHT / 32; ++i )
    {
        std::unique_ptr<Wall> leftWall( new Wall( mTextures ) );
        std::unique_ptr<Wall> rightWall( new Wall( mTextures ) );

        leftWall->setPosition( 0, (i * 64) );
        leftWall->setVelocity( 0.f, 0.f );

        rightWall->setPosition( mWorldView.getSize( ).x - 64, i * 64 );
        rightWall->setVelocity( 0.f, 0.f );

        leftRow.push_back( leftWall.get( ) );
        rightRow.push_back( rightWall.get( ) );

        mLeftWalls.push( leftRow );
        mRightWalls.push( rightRow );

        mSceneLayers[UpperAir]->attachChild( std::move( leftWall ) );
        mSceneLayers[UpperAir]->attachChild( std::move( rightWall ) );
    }
    // The HUD
    std::unique_ptr<HUD> hud( new HUD( mTextures, mFonts ) );
    mHud = hud.get();
    mHud->setHighScore( mContext.highScores->at( 0 ).second );
    mSceneLayers[Layer::Text]->attachChild( std::move( hud ) );

    // Audio
    mContext.music->setVolume( 80 );
    mContext.music->play( MusicMap.at( "GameTheme" ) );
}

void World::destroyEntitiesOutsideView( )
{
    Command command;
    command.category = Category::Projectile | Category::EnemyAircraft | Category::Tree | Category::Decoration;
    command.action = derivedAction<Entity>([this] ( Entity& e, sf::Time )
    {
        sf::Rect<float> temp = sf::Rect<float>( 0, - 100, mWorldView.getSize().x, mWorldView.getSize().y + 250 );
        if( !temp.intersects( e.getBoundingRect( ) ) )
            e.destroy( );
    });

    mCommandQueue.push( command );
}

void World::guideMissiles( )
{
    // Setup command that stores all enemies in mActiveEnemies
    Command enemyCollector;
    enemyCollector.category = Category::EnemyAircraft;
    enemyCollector.action = derivedAction<Aircraft>([this] (Aircraft& enemy, sf::Time)
    {
        if (!enemy.isDestroyed())
            mActiveEnemies.push_back(&enemy);
    });

    // Setup command that guides all missiles to the enemy which is currently closest to the player
    Command missileGuider;
    missileGuider.category = Category::AlliedProjectile;
    missileGuider.action = derivedAction<Projectile>([this] (Projectile& missile, sf::Time)
    {
        // Ignore unguided bullets
        if( !missile.isGuided( ) )
            return;

        float minDistance = std::numeric_limits<float>::max( );
        Aircraft* closestEnemy = nullptr;

        // Find closest enemy
        for( Aircraft* enemy : mActiveEnemies )
        {
            float enemyDistance = distance( missile, *enemy );

            if( enemyDistance < minDistance )
            {
                closestEnemy = enemy;
                minDistance = enemyDistance;
            }
        }

        if( closestEnemy )
            missile.guideTowards( closestEnemy->getWorldPosition( ) );
    });

    // Push commands, reset active enemies
    mCommandQueue.push( enemyCollector );
    mCommandQueue.push( missileGuider );
    mActiveEnemies.clear( );
}



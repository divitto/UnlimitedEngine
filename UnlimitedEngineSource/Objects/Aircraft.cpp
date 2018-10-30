#include "Aircraft.hpp"
#include "Core/DataTables.hpp"
#include "Core/Utility.hpp"
#include "Pickup.hpp"
#include "Core/CommandQueue.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/Category.hpp"
#include "Core/EmitterNode.hpp"


#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <cmath>

namespace
{
    const auto Table = initializeAircraftData;
}

Aircraft::Aircraft( unsigned int type, const TextureManager& textures, SoundPlayer* soundPlayer, const FontManager& )
: Entity( Table[type].hitpoints )
, mTroopCount( 0 )
, mType(type)
, mStartingHitpoints( this->getHitpoints( ) )
, mSprite( textures.get( Table[type].texture ) )
, mFireCommand( )
, mMissileCommand( )
, mFireCountdown( sf::Time::Zero )
, mIsFiring( false )
, mIsLaunchingMissile( false )
, mSpawnedPickup( false )
, mFireRateLevel( 1 )
, mSpreadLevel( 1 )
, mMissileAmmo( 2 )
, mFuel( 100 )
, mDistance( 0 )
, mDropPickupCommand( )
, mTravelledDistance( 0.f )
, mDirectionIndex( 0 )
, mDistanceDisplay( nullptr )
, mTroopDisplay( nullptr )
, mFuelDisplay( nullptr )
, mShowExplosion( true )
, mExplosion( AnimationMap.at( "Explosion" ), textures )
, mCalledDestroyedMethod( false )
, mSoundPlayer( soundPlayer )

{
    mSprite.setTextureRect( Table[type].texRect );

    centerOrigin( mSprite );
    centerOrigin( mExplosion );

    mFireCommand.category = Category::SceneAirLayer;
    mFireCommand.action   = [this, &textures] ( SceneNode& node, sf::Time )
    {
         createBullets( node, textures );
    };

    mMissileCommand.category = Category::SceneAirLayer;
    mMissileCommand.action   = [this, &textures] ( SceneNode& node, sf::Time )
    {
        createProjectile( node, ProjectileMap.at( "Missile" ), 0.f, 0.5f, textures );
    };

    mDropPickupCommand.category = Category::SceneAirLayer;
    mDropPickupCommand.action   = [this, &textures] ( SceneNode& node, sf::Time )
    {
        createPickup( node, textures );
    };

    if( getCategory( ) == Category::PlayerAircraft )
    {
        std::unique_ptr<HelicopterBlades> blades( new HelicopterBlades( textures ) );
        this->mBlades = blades.get();
        this->mBlades->setPosition( 32, 23 );
        this->attachChild( std::move( blades ) );

        // particle effects??
    }
    else if( getCategory( ) == Category::EnemyAircraft )
    {     
        if( this->mType == AircraftMap.at( "GreenTank" ) )
        {
            this->setVelocity( 40, GAME_SPEED );
            this->mSprite.setRotation( 270 );
        }
        else if( this->mType == AircraftMap.at( "Hellcat" ) )
        {
            this->setVelocity( 0, GAME_SPEED * 2 );
            this->mSprite.setRotation( 180 );
        }
        else if( this->mType == AircraftMap.at( "Chopper" ) )
        {
            this->setVelocity( getMaxSpeed(), getMaxSpeed() / 4 );
            this->mSprite.setRotation( 180 );
            std::unique_ptr<HelicopterBlades> blades( new HelicopterBlades( textures ) );
            this->mBlades = blades.get();
            this->mBlades->setPosition( 32, 41 );
            this->attachChild( std::move( blades ) );
       }
    }
}

void Aircraft::drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const
{
    if( isDestroyed( ) && mShowExplosion )
        target.draw( mExplosion, states );
    else
        target.draw( mSprite, states );
}

void Aircraft::updateCurrent( sf::Time dt, CommandQueue& commands )
{
    // Entity has been destroyed: Possibly drop pickup, mark for removal
    if( isDestroyed( ) )
    {
        if( !mCalledDestroyedMethod ) {
            mCalledDestroyedMethod = true;
            if( this->getCategory() == Category::PlayerAircraft )
            {
                mSoundPlayer->play( SoundEffectMap.at( "PlayerDeath" ), this->getPosition(), 20 );
                PLAYER_LIVES -= 1;
            }
            else if( this->getPosition().y < WINDOW_HEIGHT )
                mSoundPlayer->play( SoundEffectMap.at( "Explosion" ), this->getPosition( ), 80 );

        }
        if( this->mType == AircraftMap.at( "Chopper" )   )
        {
            this->mBlades->remove();
        }
        else if( this->getCategory() == Category::PlayerAircraft )
        {
            this->mBlades->setScale( 0, 0 );
        }
        checkPickupDrop( commands );
        if( this->getPosition().y < WINDOW_HEIGHT )
            mExplosion.update( dt );
        return;
    }

    // Check if bullets or missiles are fired
    checkProjectileLaunch( dt, commands );

    // Update enemy movement pattern; apply velocity
    if( this->getCategory() == Category::EnemyAircraft )
    {
        if( this->mType == AircraftMap.at( "GreenTank" ) )
            this->setVelocity( this->getVelocity().x, GAME_SPEED );
        else if( this->mType == AircraftMap.at( "Hellcat" ) )
            this->setVelocity( 0, getMaxSpeed() + GAME_SPEED );
        else if( this->mType == AircraftMap.at( "Chopper" ) )
        {
            if( getPosition().x < 200 )
                this->setVelocity( getMaxSpeed(), getVelocity().y );
            else if( getPosition().x > 800 )
                this->setVelocity( -1 * getMaxSpeed(), getVelocity().y );
        }
    }
    else if( this->getCategory() == Category::PlayerAircraft )
    {
        static int timer = 50;
        timer--;
        if( timer < 0 )
        {
            timer = 50;
            this->decreaseFuel( dt.asSeconds( ) * (GAME_SPEED / 6) );
        }
    }
    Entity::updateCurrent( dt, commands );
}

unsigned int Aircraft::getCategory( ) const
{
    if( isAllied( ) )
        return Category::PlayerAircraft;
    else
        return Category::EnemyAircraft;
}

sf::FloatRect Aircraft::getBoundingRect( ) const
{
    return getWorldTransform( ).transformRect( mSprite.getGlobalBounds( ) );
}

bool Aircraft::isMarkedForRemoval( ) const
{
    return isDestroyed( ) && ( mExplosion.isFinished( ) || !mShowExplosion );
}

bool Aircraft::isAllied() const
{
    return mType == (unsigned int)AircraftMap.at( "PlayerAircraft" );
}

float Aircraft::getMaxSpeed( ) const
{
    return Table[mType].speed;
}

int Aircraft::getTroopCount( void )
{
    return mTroopCount;
}

void Aircraft::increaseFireRate( )
{
    if ( mFireRateLevel < 10 )
        ++mFireRateLevel;
}

void Aircraft::increaseSpread( )
{
    if ( mSpreadLevel < 3 )
        ++mSpreadLevel;
}

void Aircraft::collectMissiles( unsigned int count )
{
    mMissileAmmo += count;
}

void Aircraft::fire( )
{
    // Only ships with fire interval != 0 are able to fire
    if( Table[mType].fireInterval != sf::Time::Zero )
        mIsFiring = true;
}

void Aircraft::launchMissile( )
{
    if( mMissileAmmo > 0 )
    {
        mIsLaunchingMissile = true;
        --mMissileAmmo;
    }
}

void Aircraft::increaseTroopCount()
{
    if( mTroopCount < 3 )
    {
        this->mTroopCount++;
        if( mTroopCount == 3 )
        {
            mSoundPlayer->play( SoundEffectMap.at( "TroopCountFull" ), this->getPosition(), 60 );
            FLIP_GAMEPLAY = true;
        }
    }
}

void Aircraft::resetTroopCount( )
{
    this->mTroopCount = 0;
}

void Aircraft::repair( int points )
{
    assert( points > 0 );

    if( this->getCategory() == Category::PlayerAircraft )
    {
        mHitpoints += points;
        if( mHitpoints > 20 )
        {
            mHitpoints = 20;
        }
    }
}

void Aircraft::kill( )
{
    this->damage( this->getHitpoints( ) );
}

void Aircraft::increaseFuel( int amount )
{
    this->mFuel += amount;
    if( mFuel > 100 )
        mFuel = 100;
}

void Aircraft::decreaseFuel( float amount )
{
    this->mFuel -= amount;
}

void Aircraft::increaseDistance()
{
    mDistance++;
}

void Aircraft::decreaseDistance()
{
    mDistance--;
}

void Aircraft::updateMovementPattern( sf::Time dt )
{
    // Enemy airplane: Movement pattern
    const std::vector<Direction>& directions = Table[mType].directions;
    if( !directions.empty( ) )
    {
        // Moved long enough in current direction: Change direction
        if( mTravelledDistance > directions[mDirectionIndex].distance )
        {
            mDirectionIndex = ( mDirectionIndex + 1 ) % directions.size( );
            mTravelledDistance = 0.f;
        }

        // Compute velocity from direction
        float radians = toRadian( directions[mDirectionIndex].angle + 90.f );
        float vx = getMaxSpeed( ) * std::cos( radians );
        float vy = getMaxSpeed( ) * std::sin( radians );

        setVelocity( vx, vy );

        mTravelledDistance += getMaxSpeed( ) * dt.asSeconds( );
    }
}

void Aircraft::checkPickupDrop( CommandQueue& commands )
{
    if( !isAllied( ) && randomInt( 3 ) == 0 && !mSpawnedPickup )
        commands.push( mDropPickupCommand );
    mSpawnedPickup = true;
}

void Aircraft::checkProjectileLaunch( sf::Time dt, CommandQueue& commands )
{
    // Enemies try to fire all the time
    if( !isAllied( ) )
        fire( );

    // Check for automatic gunfire, allow only in intervals
    if( mIsFiring && mFireCountdown <= sf::Time::Zero )
    {
        // Interval expired: We can fire a new bullet
        commands.push( mFireCommand );
        mFireCountdown += Table[mType].fireInterval / ( mFireRateLevel + 1.f );
        mIsFiring = false;
    }
    else if( mFireCountdown > sf::Time::Zero )
    {
        // Interval not expired: Decrease it further
        mFireCountdown -= dt;
        mIsFiring = false;
    }

    // Check for missile launch
    if( mIsLaunchingMissile )
    {
        commands.push( mMissileCommand );
        mIsLaunchingMissile = false;
    }
}

void Aircraft::createBullets( SceneNode& node, const TextureManager& textures )
{
    unsigned int type = isAllied( ) ? ProjectileMap.at( "AlliedBullet" ) : ProjectileMap.at( "EnemyBullet" );

    switch( mSpreadLevel )
    {
        case 1:
            createProjectile( node, type, 0.0f, 0.5f, textures );
            break;
        case 2:
            createProjectile( node, type, -0.33f, 0.33f, textures );
            createProjectile( node, type, +0.33f, 0.33f, textures );
            break;
        case 3:
            createProjectile( node, type, -0.5f, 0.33f, textures );
            createProjectile( node, type,  0.0f, 0.5f, textures );
            createProjectile( node, type, +0.5f, 0.33f, textures );
            break;
    }
    if( this->getCategory() == Category::PlayerAircraft )
        mSoundPlayer->play( SoundEffectMap.at( "PlayerShoot" ), this->getPosition( ), 20 );
    else
        mSoundPlayer->play( SoundEffectMap.at( "EnemyGunfire" ), this->getPosition( ), 100 );
}

void Aircraft::createProjectile( SceneNode& node, unsigned int type, float xOffset, float yOffset, const TextureManager &textures ) const
{
    std::unique_ptr<Projectile> projectile( new Projectile( type, textures ) );

    sf::Vector2f offset( xOffset * mSprite.getGlobalBounds( ).width, yOffset * mSprite.getGlobalBounds( ).height );
    sf::Vector2f velocity( 0, projectile->getMaxSpeed( ) );

    float sign = isAllied( ) ? -1.f : +1.f;
    projectile->setPosition( getWorldPosition( ) + offset * sign );
    projectile->setVelocity( velocity * sign );
    node.attachChild( std::move( projectile ) );
}

void Aircraft::createPickup( SceneNode& node, const TextureManager& textures ) const
{
    if( this->mType == AircraftMap.at( "Chopper" ) )
    {
        std::unique_ptr<Pickup> pickup( new Pickup( static_cast<Pickup::Type>( Pickup::HealthRefill  ), textures ) );
        pickup->setPosition( getWorldPosition( ) );
        node.attachChild( std::move( pickup ) );
    }
    else
    {
        std::unique_ptr<Pickup> pickup( new Pickup( static_cast<Pickup::Type>( randomInt( Pickup::TypeCount - 1 ) ), textures ) );
        pickup->setPosition( getWorldPosition( ) );
        node.attachChild( std::move( pickup ) );
    }
}


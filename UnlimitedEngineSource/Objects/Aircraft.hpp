#ifndef AIRCRAFT_HPP
#define AIRCRAFT_HPP

#include "Core/Globals.hpp"
#include "Core/Entity.hpp"
#include "Core/Command.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/ResourceIdentifiers.hpp"
#include "Core/TextNode.hpp"
#include "Core/DataTables.hpp"
#include "Core/Animation.hpp"
#include "Core/EmitterNode.hpp"
#include "Core/SoundNode.hpp"
#include "Core/SoundPlayer.hpp"

#include "Objects/Projectile.hpp"
#include "Objects/HelicopterBlades.hpp"

#include <SFML/Graphics/Sprite.hpp>

class Aircraft : public Entity
{
    public:
    lua_State*          mLua;

    Aircraft(unsigned int type, const TextureManager& textures, SoundPlayer* soundPlayer, const FontManager &fonts );

    virtual unsigned int  getCategory( ) const;
    virtual sf::FloatRect getBoundingRect( ) const;
    virtual bool 		  isMarkedForRemoval( ) const;
    bool				  isAllied( ) const;
    float				  getMaxSpeed( ) const;
    int                   getTroopCount( void );

    void increaseFireRate( );
    void increaseSpread( );
    void collectMissiles( unsigned int count );
    void fire( );
    void launchMissile( );
    void increaseTroopCount( );
    void resetTroopCount( );
    void repair( int points );
    void kill( );
    void increaseFuel( int amount = 25 );
    void decreaseFuel( float amount );
    void increaseDistance( );
    void decreaseDistance( );
    int getDistance( void ) { return mDistance; }
    int getFuelLevel( void ) { return mFuel; }

    //## luaInterface
    static int luaCreateParticleEffect( lua_State* L );
    static int luaDestroyParticleEffect( lua_State* L );

private:
    virtual void drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const;
    virtual void updateCurrent( sf::Time dt, CommandQueue& commands );
    void updateMovementPattern( sf::Time dt );
    void checkPickupDrop( CommandQueue& commands );
    void checkProjectileLaunch( sf::Time dt, CommandQueue& commands );

    void createBullets( SceneNode& node, const TextureManager& textures );
    void createProjectile( SceneNode& node, unsigned int type, float xOffset, float yOffset, const TextureManager& textures ) const;
    void createPickup( SceneNode& node, const TextureManager& textures ) const;

    //# Attributes
    unsigned int        mTroopCount;
    unsigned int        mType;
    int                 mStartingHitpoints;
    bool                mIsSmoking;
    sf::Sprite          mSprite;
    Command             mFireCommand;
    Command             mMissileCommand;
    Command             mClearSmokeCommand;
    Command             mDestroyParticleEffectCommand;
    sf::Time            mFireCountdown;
    bool                mIsFiring;
    bool                mIsLaunchingMissile;
    bool                mSpawnedPickup;

    int                 mFireRateLevel;
    int                 mSpreadLevel;
    int                 mMissileAmmo;
    int                 mFuel;
    int                 mDistance;

    Command             mDropPickupCommand;
    float               mTravelledDistance;
    std::size_t         mDirectionIndex;
    TextNode*           mDistanceDisplay;
    TextNode*           mTroopDisplay;
    TextNode*           mFuelDisplay;
    bool                mShowExplosion;
    Animation           mExplosion;
    unsigned int        mDamageSmoke;
    bool                mCalledDestroyedMethod;
    HelicopterBlades*   mBlades;
    SoundPlayer*        mSoundPlayer;
    SoundNode*          mShootSound;

};
#endif // AIRCRAFT_HPP

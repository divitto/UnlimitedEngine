#include "Projectile.hpp"
#include "Core/DataTables.hpp"
#include "Core/Utility.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/EmitterNode.hpp"
#include "Core/Globals.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <cmath>
#include <cassert>

namespace
{
    const std::vector<ProjectileData> Table = initializeProjectileData;
}

Projectile::Projectile( unsigned int type, const TextureManager& textures)
: Entity(1)
, mType(type)
, mSprite(textures.get(Table[type].texture))
, mTargetDirection()
, mLifeSpan( sf::milliseconds( 2050 ) )
{
    mSprite.setTextureRect( Table[type].texRect );

	centerOrigin(mSprite);
}

void Projectile::guideTowards( sf::Vector2f position )
{
    assert( isGuided( ) );
    mTargetDirection = unitVector(position - getWorldPosition( ) );
}

bool Projectile::isGuided( ) const
{
    return mType == (unsigned int)ProjectileMap.at( "Missile" );
}

void Projectile::updateCurrent( sf::Time dt, CommandQueue& commands )
{
    mLifeSpan -= dt;
    if( mLifeSpan < sf::Time::Zero )
        this->destroy();

    if( isGuided( ) )
	{
        std::unique_ptr<EmitterNode> propellant( new EmitterNode( ParticleMap.at( "MissilePropellant" ) ) );
        propellant->setPosition( 0.0f, (getBoundingRect().height / 2.0f) + 5 );
        attachChild( std::move( propellant ) );

        std::unique_ptr<EmitterNode> smoke( new EmitterNode( ParticleMap.at( "MissileSmoke" ) ) );
        smoke->setPosition( 0.0f, (getBoundingRect().height / 2.0f) + 21 );
        attachChild( std::move( smoke ) );

        // THIS IS HOW YOU CHANGE MISSILE ACCURACY
        const float approachRate = 300.f;

        sf::Vector2f newVelocity = unitVector( approachRate * dt.asSeconds() * mTargetDirection + getVelocity());
        newVelocity *= getMaxSpeed( );
        float angle = std::atan2( newVelocity.y, newVelocity.x );

        setRotation( toDegree( angle ) + 90.f );
        setVelocity( newVelocity );
	}
    else if( getCategory() == Category::EnemyProjectile )
    {
        if( mType == (unsigned int)ProjectileMap.at( "EnemyBullet" ) )
            setVelocity( 0, getMaxSpeed() );
    }

    Entity::updateCurrent( dt, commands );
}

void Projectile::drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const
{
	target.draw(mSprite, states);
}

unsigned int Projectile::getCategory( ) const
{
    if ( mType == (unsigned int)ProjectileMap.at( "EnemyBullet" ) )
		return Category::EnemyProjectile;
	else
		return Category::AlliedProjectile;
}

sf::FloatRect Projectile::getBoundingRect( ) const
{
    return getWorldTransform( ).transformRect(mSprite.getGlobalBounds( ) );
}

float Projectile::getMaxSpeed( ) const
{
    if( getCategory() == Category::EnemyProjectile )
        return Table[mType].speed + GAME_SPEED;
    return Table[mType].speed;
}

int Projectile::getDamage( ) const
{
	return Table[mType].damage;
}

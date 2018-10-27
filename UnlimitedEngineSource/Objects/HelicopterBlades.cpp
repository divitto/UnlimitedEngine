#include "HelicopterBlades.hpp"

HelicopterBlades::HelicopterBlades( const TextureManager& tex)
    : Entity( 100 )
    , isRemoved( false )
    , deathClock( sf::microseconds( 1500 ) )
{
    std::unique_ptr<SpriteNode> sprite( new SpriteNode( tex.get( TextureMap.at( "HelicopterBlades" ) ) ) );
    this->sprite = sprite.get();
    this->setOrigin( 32, 32 );
    this->sprite->setOrigin( 32, 32 );
    this->attachChild( std::move( sprite ) );
}

HelicopterBlades::~HelicopterBlades( void )
{
   // delete sprite;
}

void HelicopterBlades::updateCurrent(sf::Time, CommandQueue& )
{
    if( isRemoved )
    {
        //if( deathClock >= sf::Time::Zero )
            //deathClock -= dt;
        this->sprite->rotate( 10 );
    }
    else
        this->sprite->rotate( 15 );
}

void HelicopterBlades::drawCurrent( sf::RenderTarget& target, sf::RenderStates ) const
{
    target.draw( this->sprite->getSprite( ) );
}

void HelicopterBlades::remove( void )
{
    this->isRemoved = true;
}

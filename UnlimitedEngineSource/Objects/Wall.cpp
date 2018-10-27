#include "Wall.hpp"

Wall::Wall(TextureManager& textures )
    : Entity(1)
    , mSprite(textures.get( TextureMap.at( "Tree" ) ))
{
    int r = rand() % 4 + 1;
    switch( r )
    {
        case 1: mSprite.setTextureRect( sf::IntRect(  0, 0, 68, 68  ) ); break;
        case 2: mSprite.setTextureRect( sf::IntRect( 68, 0, 68, 68  ) ); break;
        case 3: mSprite.setTextureRect( sf::IntRect( 0, 68, 68, 68  ) ); break;
        case 4: mSprite.setTextureRect( sf::IntRect( 68, 68, 68, 68 ) ); break;
        default: std::cout << "Imposible! case Wall(Tree)" << std::endl; break;
    }
}

unsigned int Wall::getCategory( ) const
{
    if( this->mHasCollision )
        return Category::Tree;
    return Category::Decoration;
}

sf::FloatRect Wall::getBoundingRect( ) const
{
    return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

void Wall::drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const
{
    target.draw( mSprite, states );
}

void Wall::updateCurrent( sf::Time dt, CommandQueue& )
{
    this->setPosition( this->getPosition( ).x, this->getPosition( ).y + GAME_SPEED * dt.asSeconds( ) );
}


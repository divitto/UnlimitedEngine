#ifndef WALL_HPP
#define WALL_HPP

#include "Core/Entity.hpp"
#include "Aircraft.hpp"
#include "Core/Globals.hpp"

class Wall : public Entity
{
public:
    Wall( TextureManager& textures );

    virtual unsigned int	getCategory() const;
    virtual sf::FloatRect	getBoundingRect() const;

    void setHasCollision( bool hasCollision )
    {
        this->mHasCollision = hasCollision;
    }

    sf::Sprite mSprite;
    bool mHasCollision;
protected:
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
    virtual void updateCurrent( sf::Time dt, CommandQueue& );
};

#endif // WALL_HPP

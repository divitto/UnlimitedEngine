#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include "Core/Entity.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/ResourceIdentifiers.hpp"

#include <SFML/Graphics/Sprite.hpp>

class Projectile : public Entity
{
public:
    Projectile( unsigned int type, const TextureManager& textures);

    void guideTowards(sf::Vector2f position);
    bool isGuided() const;

    virtual unsigned int  getCategory() const;
    virtual sf::FloatRect getBoundingRect() const;
    float                 getMaxSpeed() const;
    int					  getDamage() const;
	
private:
    virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

    //## Attributes
    unsigned int mType;
    sf::Sprite	 mSprite;
    sf::Vector2f mTargetDirection;
    sf::Time     mLifeSpan;
};

#endif // PROJECTILE_HPP

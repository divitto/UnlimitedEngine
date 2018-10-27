#ifndef PICKUP_HPP
#define PICKUP_HPP

#include "Core/Entity.hpp"
#include "Core/Command.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/ResourceIdentifiers.hpp"
#include <SFML/Graphics/Sprite.hpp>

class Aircraft;

class Pickup : public Entity
{
public:
    enum Type
    {
        HealthRefill,
        FireSpread,
        Fuel,
        Ally,
        TypeCount
    };

    Pickup( int type, const TextureManager& textures);

    virtual unsigned int	getCategory() const;
    virtual sf::FloatRect	getBoundingRect() const;

    void apply(Aircraft& player) const;
    unsigned int getType( void ) { return mType; }

protected:
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
    virtual void updateCurrent( sf::Time dt, CommandQueue& commands );
private:
    Type       mType;
    sf::Sprite mSprite;
};

#endif // PICKUP_HPP

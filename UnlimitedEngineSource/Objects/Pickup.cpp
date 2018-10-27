#include "Core/Globals.hpp"
#include "Pickup.hpp"
#include "Core/DataTables.hpp"
#include "Core/Category.hpp"
#include "Core/CommandQueue.hpp"
#include "Core/Utility.hpp"
#include "Core/ResourceManager.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace
{
	const std::vector<PickupData> Table = initializePickupData();
}

Pickup::Pickup(int type, const TextureManager &textures)
: Entity(1)
, mType((Pickup::Type)type)
, mSprite(textures.get(Table[type].texture))
{
    mSprite.setTextureRect( Table[type].texRect );
	centerOrigin(mSprite);

}

unsigned int Pickup::getCategory() const
{
	return Category::Pickup;
}

sf::FloatRect Pickup::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

void Pickup::apply(Aircraft& player) const
{
	Table[mType].action(player);
}

void Pickup::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

void Pickup::updateCurrent( sf::Time dt, CommandQueue& )
{
    sf::Vector2f pos = this->getPosition();
    this->setPosition( pos.x, pos.y + GAME_SPEED * dt.asSeconds() );
}

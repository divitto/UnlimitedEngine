#ifndef HELICOPTERBLADES_HPP
#define HELICOPTERBLADES_HPP

#include "Core/Globals.hpp"
#include "Core/SceneNode.hpp"
#include "Core/TextNode.hpp"
#include "Core/Entity.hpp"
#include "Core/State.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/ResourceIdentifiers.hpp"
#include "Core/DataTables.hpp"
#include "Core/SpriteNode.hpp"

class HelicopterBlades : public Entity
{
public:
   HelicopterBlades( const TextureManager& tex );
   ~HelicopterBlades( void );

   virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
   virtual void drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const;
   void remove( void );
private:
   SpriteNode* sprite;
   bool isRemoved;
   sf::Time deathClock;
};

#endif // HELICOPTERBLADES_HPP

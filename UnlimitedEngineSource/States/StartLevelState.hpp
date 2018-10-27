#ifndef STARTLEVELSTATE_HPP
#define STARTLEVELSTATE_HPP

#include "Core/State.hpp"
#include "Gui/Container.hpp"
#include "Core/Globals.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include "Core/SceneNode.hpp"
#include "Core/TextNode.hpp"


class StartLevelState : public State
{
public:
    StartLevelState( States::ID id, StateStack& stack, Context context );

    virtual void draw( );
    virtual bool update( sf::Time dt );
    virtual bool handleEvent( const sf::Event& event );
private:
    //## Attributes
    sf::Text mStartText;
};

#endif // STARTLEVELSTATE_HPP

#ifndef GAMEOVERSTATE_HPP
#define GAMEOVERSTATE_HPP

#include "Core/State.hpp"
#include "Gui/Container.hpp"
#include "Core/Globals.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class GameOverState : public State
{
public:
    GameOverState( States::ID id, StateStack& stack, Context context );

    virtual void draw( void );
    virtual bool update( sf::Time dt );
    virtual bool handleEvent( const sf::Event& event );

private:
    sf::Text mGameOverText;
    sf::Time mElapsedTime;
};

#endif // GAMEOVERSTATE_HPP

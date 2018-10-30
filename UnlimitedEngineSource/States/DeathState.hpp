#ifndef DEATHSTATE_HPP
#define DEATHSTATE_HPP

#include "Core/State.hpp"
#include "Core/ParallelTask.hpp"
#include "Core/DataTables.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

class DeathState : public State
{
public:
    DeathState( States::ID id, StateStack& stack, Context context );
    virtual void draw( );
    virtual bool update( sf::Time dt );
    virtual bool handleEvent( const sf::Event& event );

    void setCompletion( float percent );
private:

    //## Attributes
    sf::Text*		   mLoadingText;
    bool               mReady;
    sf::Time           mTimer;
    ParallelTask	   mLoadingTask;
};

#endif // DEATHSTATE_HPP

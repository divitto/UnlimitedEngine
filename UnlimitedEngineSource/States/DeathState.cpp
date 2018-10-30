#include "DeathState.hpp"
#include "Core/Utility.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/DataTables.hpp"
#include "Core/MusicPlayer.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

DeathState::DeathState( States::ID id, StateStack& stack, Context context )
: State( id, stack, context )
{
    mTimer = sf::seconds( 2 );
    mReady = false;

    std::string premise = "\t\t\t\t\t\t\t\tThat was pathetic!\n\t\t\t\tThat can't be the best you can do!\n\t\t\t\t\t\t\t\tGet back out there,\n and you better not scratch my chopper this time!\n\n\t\t\t\t\t\t\t\t Are you READY?";

    mLoadingText = new sf::Text( premise, context.fonts->get( FontMap.at( "Default" ) ), 40 );
    mLoadingText->setFillColor( sf::Color( 181, 182, 228, 255 ) );
    mLoadingText->setPosition( WINDOW_WIDTH / 2, 300 );
    centerOrigin( *mLoadingText );

    mLoadingTask.execute( context, CurrentLuaFile.c_str() );
}

void DeathState::draw( )
{
    sf::RenderTarget& window = *getContext( ).window;

    window.setView( window.getDefaultView( ) );

    window.draw( *mLoadingText );
}

bool DeathState::update( sf::Time dt )
{
    if( mReady )
        mTimer -= dt;

    // Update the progress bar from the remote task or finish it
    if( mLoadingTask.isFinished( ) && mTimer < sf::Time::Zero )
    {
        requestStackPop( );
        mContext.music->stop( );
        requestStackPush( States::Game );
    }
    return true;
}

bool DeathState::handleEvent( const sf::Event& event )
{
    if( ( event.type == sf::Event::JoystickButtonPressed || ( event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Enter ) ) && !mReady )
    {
        mContext.sounds->play( SoundEffectMap.at( "LetsGo" ), sf::Vector2f( 0.0f, 0.0f ), 100 );
        mLoadingText->setString( mLoadingText->getString() + "\n\n\n\t\t\t\t\t\t\t\t\t   LET'S GO!!" );
        mReady = true;
    }
    return true;
}

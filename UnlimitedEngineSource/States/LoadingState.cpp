#include "LoadingState.hpp"
#include "Core/Utility.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/DataTables.hpp"
#include "Core/MusicPlayer.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

LoadingState::LoadingState( States::ID id, StateStack& stack, Context context )
: State( id, stack, context )
{
    mTimer = sf::seconds( 2 );
    mReady = false;

    std::string premise = "Alright soldiers!\nOur best Troops are out behind enemy lines,\nand while you've been sitting there\nthey've been gathering intel!\nSo now it's your turn,\nGet out there and bring those troops home!\n\n\t\t\t\t\t\tAre you READY?";

    mLoadingText = new sf::Text( premise, context.fonts->get( FontMap.at( "Default" ) ), 40 );
    mLoadingText->setFillColor( sf::Color( 181, 182, 228, 255 ) );
    mLoadingText->setPosition( WINDOW_WIDTH / 2, 300 );
    centerOrigin( *mLoadingText );

    mLoadingTask.execute( context, CurrentLuaFile.c_str() );
}

void LoadingState::draw( )
{
    sf::RenderTarget& window = *getContext( ).window;

    window.setView( window.getDefaultView( ) );

    window.draw( *mLoadingText );
}

bool LoadingState::update( sf::Time dt )
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

bool LoadingState::handleEvent( const sf::Event& event )
{
    if( ( event.type == sf::Event::JoystickButtonPressed || ( event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Enter ) ) && !mReady )
    {
        mContext.sounds->play( SoundEffectMap.at( "LetsGo" ), sf::Vector2f( 0.0f, 0.0f ), 100 );
        mLoadingText->setString( mLoadingText->getString() + "\n\n\n\t\t\t\t\t\t\t  LET'S GO!!" );
        mReady = true;
    }
	return true;
}

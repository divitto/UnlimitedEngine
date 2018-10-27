#include "GameOverState.hpp"
#include "Core/Utility.hpp"
#include "Core/Player.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/DataTables.hpp"


#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

GameOverState::GameOverState( States::ID id, StateStack& stack, Context context )
: State( id, stack, context )
, mGameOverText( )
, mElapsedTime( sf::Time::Zero )
{
    sf::Font& font = context.fonts->get(FontMap.at( "Default" ) );
    sf::Vector2f windowSize( context.window->getSize( ) );

    mGameOverText.setFillColor( sf::Color( 181, 182, 228, 255 ) );
    mGameOverText.setFont( font );
    mGameOverText.setString( "Your Journey Ends...\n\n\n\t  Score: " + std::to_string( PLAYER_SCORE ) );

    mGameOverText.setCharacterSize( 70 );
    centerOrigin( mGameOverText );
    mGameOverText.setPosition( 0.5f * windowSize.x, 0.4f * windowSize.y );
}

void GameOverState::draw( )
{
    sf::RenderTarget& window = *getContext( ).window;
    window.setView( window.getDefaultView( ) );

	// Create dark, semitransparent background
	sf::RectangleShape backgroundShape;
    backgroundShape.setFillColor( sf::Color( 0, 0, 0, 150 ) );
    backgroundShape.setSize( window.getView( ).getSize( ) );

    window.draw( backgroundShape );
    window.draw( mGameOverText );
}

bool GameOverState::update( sf::Time dt )
{
	// Show state for 3 seconds, after return to menu
	mElapsedTime += dt;
    if( mElapsedTime > sf::seconds( 7 ) )
    {
        requestStateClear( );
        if( PLAYER_SCORE >= mContext.highScores->back().second )
            requestStackPush( States::GetNameState );
        else
            requestStackPush( States::HighScoreState );
	}
	return false;
}

bool GameOverState::handleEvent( const sf::Event& )
{
	return false;
}

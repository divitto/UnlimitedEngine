#include "StartLevelState.hpp"

#include "Core/Utility.hpp"
#include "Core/Player.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/DataTables.hpp"
#include "Core/SoundPlayer.hpp"
#include "Core/MusicPlayer.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

StartLevelState::StartLevelState( States::ID id, StateStack& stack, Context context )
    : State( id, stack, context )
{
    sf::Font& font = mContext.fonts->get( FontMap.at( "Default" ) );
    sf::Vector2f windowSize( context.window->getSize( ) );

    mStartText.setFont( font );
    mStartText.setString( "\t\t\tWell done!\n\n\nGet Ready for the next\n\t\t\t\t Raid!\n\t\t  (Press Enter)" );

    mStartText.setCharacterSize( 60 );
    centerOrigin( mStartText );
    mStartText.setPosition( 0.5f * windowSize.x, 0.4f * windowSize.y );
}

void StartLevelState::draw()
{
    sf::RenderTarget& window = *getContext( ).window;
    window.setView( window.getDefaultView( ) );

    sf::RectangleShape backgroundShape;
    backgroundShape.setFillColor( sf::Color( 0, 0, 0, 255 ) );
    backgroundShape.setSize( window.getView( ).getSize()  );

    window.draw( backgroundShape );
    window.draw( mStartText );
}

bool StartLevelState::update( sf::Time )
{
     return false;
}

bool StartLevelState::handleEvent( const sf::Event& event )
{
    if( event.type == sf::Event::JoystickButtonPressed || ( event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Enter ) )
    {
        mContext.sounds->play( SoundEffectMap.at( "LetsGo" ), sf::Vector2f( 0.0f, 0.0f ), 100 );
        mContext.player->setMissionStatus( Player::MissionRunning );
        mContext.music->setPaused( false );
        requestStackPop( );
    }
    return true;
}

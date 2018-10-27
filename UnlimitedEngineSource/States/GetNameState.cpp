#include "GetNameState.hpp"
#include "Gui/Button.hpp"
#include "Core/Utility.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/DataTables.hpp"
#include "Core/Globals.hpp"
#include "Core/MusicPlayer.hpp"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

GetNameState::GetNameState( States::ID id, StateStack& stack, Context context )
: State( id, stack, context )
, mSceneGraph( )
{
    context.textures->load( TextureMap.at( "Helicopter" ), MediaFileMap.at( "Textures" ).at( TextureMap.at( "Helicopter" ) ) );

    mTitleText = sf::Text( " SUPER\nRESCUE\n   RAID", mContext.fonts->get( FontMap.at( "Default" ) ), 100 );
    mTitleText.setFillColor( sf::Color( 181, 182, 228, 255 ) );
    centerOrigin( mTitleText );
    mTitleText.setPosition( WINDOW_WIDTH / 2, 200 );
    mTitleText.setOutlineColor( sf::Color( 79, 67, 174, 255 ) );
    mTitleText.setOutlineThickness( 5 );
    mTitleText.setStyle( sf::Text::Bold );

    mContext.music->setVolume( 30 );
    mContext.music->play( MusicMap.at( "MenuTheme" ) );

    bool hasRanked = false;
    nameComplete = false;

    // check whether the player earned a high score!
    for( unsigned int i = 0; i < 10; ++i )
    {
        if( PLAYER_SCORE > context.highScores->at( i ).second )
        {
            hasRanked = true;
            rankPosition = i;
            // Player made it on to highscore list!
            for( unsigned int j = 9; j > i; --j )
            {
                context.highScores->at( j ).first  = context.highScores->at( j - 1 ).first;
                context.highScores->at( j ).second = context.highScores->at( j - 1 ).second;
            }
            break;
        }
    }

    if( !hasRanked )
    {
        requestStateClear();
        requestStackPush( States::HighScoreState );
    }

    std::unique_ptr<TextNode> score( new TextNode( *context.fonts, "FINAL SCORE:\t\t\t" + std::to_string( PLAYER_SCORE ) ) );
    score.get()->getText()->setFillColor( sf::Color( 181, 182, 228, 255 ) );
    score.get( )->setPosition( WINDOW_WIDTH / 2, 600 );
    score.get( )->getText( )->setCharacterSize( 50 );
    centerOrigin( *score.get( )->getText() );
    mSceneGraph.attachChild( std::move( score ) );

    std::unique_ptr<TextNode> done( new TextNode( *context.fonts, "DONE" ) );
    done.get()->getText()->setFillColor( sf::Color( 181, 182, 228, 255 ) );
    done.get()->setPosition( 850 , WINDOW_HEIGHT / 2 + 20 );
    done.get()->getText()->setCharacterSize( 30 );
    centerOrigin( *done.get( )->getText() );
    mSceneGraph.attachChild( std::move( done ) );

    std::unique_ptr<TextNode> char1( new TextNode( *context.fonts, "_" ) );
    letter1 = char1.get( );
    letter1->getText()->setFillColor( sf::Color( 181, 182, 228, 255 ) );
    letter1->getText()->setCharacterSize( 70 );
    letter1->setPosition( WINDOW_WIDTH / 2 - 225, WINDOW_HEIGHT / 2 );
    mSceneGraph.attachChild( std::move( char1 ) );

    std::unique_ptr<TextNode> char2( new TextNode( *context.fonts, "_" ) );
    letter2 = char2.get( );
    letter2->getText()->setFillColor( sf::Color( 181, 182, 228, 255 ) );
    letter2->setPosition( WINDOW_WIDTH / 2 - 20, WINDOW_HEIGHT / 2 );
    letter2->getText()->setCharacterSize( 70 );
    mSceneGraph.attachChild( std::move( char2 ) );

    std::unique_ptr<TextNode> char3( new TextNode( *context.fonts, "_" ) );
    letter3 = char3.get( );
    letter3->getText()->setFillColor( sf::Color( 181, 182, 228, 255 ) );
    letter3->setPosition( WINDOW_WIDTH / 2 + 200, WINDOW_HEIGHT / 2 );
    letter3->getText()->setCharacterSize( 70 );
    mSceneGraph.attachChild( std::move( char3 ) );

    std::unique_ptr<SpriteNode> marker( new SpriteNode( context.textures->get( TextureMap.at( "Helicopter" ) ) ) );
    mMarker = marker.get();
    mMarker->setPosition( letter1->getPosition().x - 15, WINDOW_HEIGHT / 2 + 75 );

    // ADD HELICOPTER BLADES HERE!!!

    mSceneGraph.attachChild( std::move( marker ) );
}

GetNameState::~GetNameState( )
{
    mContext.textures->remove( TextureMap.at( "Helicopter" ) );
}

void GetNameState::draw( )
{
    sf::RenderTarget& window = *getContext( ).window;

    window.setView( window.getDefaultView( ) ); // YOU NEED TO MAKE THIS BETTER!
    window.draw( mSceneGraph );
    window.draw( mTitleText );
}

bool GetNameState::update( sf::Time )
{
    return true;
}

bool GetNameState::handleEvent( const sf::Event& event )
{
    if( event.type == sf::Event::JoystickMoved )
    {
        if( event.joystickMove.axis == sf::Joystick::Y && event.joystickMove.position < 0 )
        {
            int x = mMarker->getPosition().x;

            if( x == letter1->getPosition().x - 15 )
            {
                if( letter1Data < chars.length() - 1 )
                    letter1Data++;
                else
                    letter1Data = 0;
                letter1->getText()->setString( std::string( "" ) + chars.c_str()[letter1Data] );
            }
            else if( x == letter2->getPosition().x - 15 )
            {
                if( letter2Data < chars.length() - 1 )
                    letter2Data++;
                else
                    letter2Data = 0;
                letter2->getText()->setString( std::string( "" ) + chars.c_str()[letter2Data] );
            }
            else if( x == letter3->getPosition().x - 15 )
            {
                if( letter3Data < chars.length() - 1 )
                    letter3Data++;
                else
                    letter3Data = 0;
                letter3->getText()->setString( std::string( "" ) + chars.c_str()[letter3Data] );
            }
        }
        else if( event.joystickMove.axis == sf::Joystick::Y && event.joystickMove.position > 0 )
        {
            int x = mMarker->getPosition().x;

            if( x == letter1->getPosition().x - 15 )
            {
                if( letter1Data > 0 )
                    letter1Data--;
                else
                    letter1Data = chars.length() - 1;
                letter1->getText()->setString( std::string( "" ) + chars.c_str()[letter1Data] );
            }
            else if( x == letter2->getPosition().x - 15 )
            {
                if( letter2Data > 0 )
                    letter2Data--;
                else
                    letter2Data = chars.length() - 1;
                letter2->getText()->setString( std::string( "" ) + chars.c_str()[letter2Data] );
            }
            else if( x == letter3->getPosition().x - 15 )
            {
                if( letter3Data > 0 )
                    letter3Data--;
                else
                    letter3Data = chars.length() - 1;
                letter3->getText()->setString( std::string( "" ) + chars.c_str()[letter3Data] );
            }
        }

        if( event.joystickMove.axis == sf::Joystick::X && event.joystickMove.position > 0 )
        {
            int x = mMarker->getPosition().x;
            int y = mMarker->getPosition().y;
            if( x == letter1->getPosition().x - 15 )
            {
                mMarker->setPosition( letter2->getPosition().x - 15, y );
                nameComplete = false;
            }
            else if( x == letter2->getPosition().x - 15 )
            {
                mMarker->setPosition( letter3->getPosition().x - 15, y );
                nameComplete = false;
            }
            else if( x == letter3->getPosition().x - 15 )
            {
                mMarker->setPosition( 850, y );
                nameComplete = true;
            }
            else
            {
                mMarker->setPosition( letter1->getPosition().x - 15, y );
                nameComplete = false;
            }
        }
        else if( event.joystickMove.axis == sf::Joystick::X && event.joystickMove.position < 0 )
        {
            int x = mMarker->getPosition().x;
            int y = mMarker->getPosition().y;
            if( x == letter1->getPosition().x - 15 )
            {
                mMarker->setPosition( 850, y );
                nameComplete = true;
            }
            else if( x == letter2->getPosition().x - 15 )
            {
                mMarker->setPosition( letter1->getPosition().x - 15, y );
                nameComplete = false;
            }
            else if( x == letter3->getPosition().x - 15 )
            {
                mMarker->setPosition( letter2->getPosition().x - 15, y );
                nameComplete = false;
            }
            else
            {
                mMarker->setPosition( letter3->getPosition().x - 15, y );
                nameComplete = false;
            }
        }
    }
    else if( event.type == sf::Event::KeyPressed )
    {
        if( event.key.code == sf::Keyboard::Up )
        {
            int x = mMarker->getPosition().x;

            if( x == letter1->getPosition().x - 15 )
            {
                if( letter1Data < chars.length() - 1 )
                    letter1Data++;
                else
                    letter1Data = 0;
                letter1->getText()->setString( std::string( "" ) + chars.c_str()[letter1Data] );
            }
            else if( x == letter2->getPosition().x - 15 )
            {
                if( letter2Data < chars.length() - 1 )
                    letter2Data++;
                else
                    letter2Data = 0;
                letter2->getText()->setString( std::string( "" ) + chars.c_str()[letter2Data] );
            }
            else if( x == letter3->getPosition().x - 15 )
            {
                if( letter3Data < chars.length() - 1 )
                    letter3Data++;
                else
                    letter3Data = 0;
                letter3->getText()->setString( std::string( "" ) + chars.c_str()[letter3Data] );
            }
        }
        else if( event.key.code == sf::Keyboard::Down )
        {
            int x = mMarker->getPosition().x;

            if( x == letter1->getPosition().x - 15 )
            {
                if( letter1Data > 0 )
                    letter1Data--;
                else
                    letter1Data = chars.length() - 1;
                letter1->getText()->setString( std::string( "" ) + chars.c_str()[letter1Data] );
            }
            else if( x == letter2->getPosition().x - 15 )
            {
                if( letter2Data > 0 )
                    letter2Data--;
                else
                    letter2Data = chars.length() - 1;
                letter2->getText()->setString( std::string( "" ) + chars.c_str()[letter2Data] );
            }
            else if( x == letter3->getPosition().x - 15 )
            {
                if( letter3Data > 0 )
                    letter3Data--;
                else
                    letter3Data = chars.length() - 1;
                letter3->getText()->setString( std::string( "" ) + chars.c_str()[letter3Data] );
            }
        }
        else if( event.key.code == sf::Keyboard::Right )
        {
            int x = mMarker->getPosition().x;
            int y = mMarker->getPosition().y;
            if( x == letter1->getPosition().x - 15 )
            {
                mMarker->setPosition( letter2->getPosition().x - 15, y );
                nameComplete = false;
            }
            else if( x == letter2->getPosition().x - 15 )
            {
                mMarker->setPosition( letter3->getPosition().x - 15, y );
                nameComplete = false;
            }
            else if( x == letter3->getPosition().x - 15 )
            {
                mMarker->setPosition( 850, y );
                nameComplete = true;
            }
            else
            {
                mMarker->setPosition( letter1->getPosition().x - 15, y );
                nameComplete = false;
            }
        }
        else if( event.key.code == sf::Keyboard::Left )
        {
            int x = mMarker->getPosition().x;
            int y = mMarker->getPosition().y;
            if( x == letter1->getPosition().x - 15 )
            {
                mMarker->setPosition( 850, y );
                nameComplete = true;
            }
            else if( x == letter2->getPosition().x - 15 )
            {
                mMarker->setPosition( letter1->getPosition().x - 15, y );
                nameComplete = false;
            }
            else if( x == letter3->getPosition().x - 15 )
            {
                mMarker->setPosition( letter2->getPosition().x - 15, y );
                nameComplete = false;
            }
            else
            {
                mMarker->setPosition( letter3->getPosition().x - 15, y );
                nameComplete = false;
            }
        }
    }
    else if( event.type == sf::Event::KeyReleased )
    {
        if( nameComplete && ( event.type == sf::Event::JoystickButtonPressed || ( event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space ) ) )
        {
            mContext.highScores->at( rankPosition ).first  = letter1->getText( )->getString( ).toAnsiString( ) + letter2->getText()->getString( ).toAnsiString( ) + letter3->getText()->getString( ).toAnsiString( );;
            mContext.highScores->at( rankPosition ).second = PLAYER_SCORE;
            requestStateClear( );
            requestStackPush( States::HighScoreState );
        }
    }
    else if( event.type == sf::Event::JoystickButtonPressed )
    {
        if( nameComplete && event.type == sf::Event::JoystickButtonPressed )
        {
            mContext.highScores->at( rankPosition ).first  = letter1->getText( )->getString( ).toAnsiString( ) + letter2->getText()->getString( ).toAnsiString( ) + letter3->getText()->getString( ).toAnsiString( );;
            mContext.highScores->at( rankPosition ).second = PLAYER_SCORE;
            requestStateClear( );
            requestStackPush( States::HighScoreState );
        }
    }
    return false;
}

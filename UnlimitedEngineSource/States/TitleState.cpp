#include "TitleState.hpp"
#include "Core/Utility.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/DataTables.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

TitleState::TitleState( States::ID id, StateStack& stack, Context context )
: State( id, stack, context )
, mText( )
, mShowText( true )
, mTextEffectTime( sf::Time::Zero )
{
    mContext.textures->load( TextureMap.at( "TitleScreen" ), MediaFileMap.at( "Textures" ).at( TextureMap.at( "TitleScreen" ) ) );

    mBackgroundSprite.setTexture( context.textures->get( TextureMap.at( "TitleScreen" ) ) );

    mText.setFont( context.fonts->get( FontMap.at( "Default" ) ) );
    mText.setString( "PRESS START TO PLAY" );
    centerOrigin( mText );
    mText.setFillColor( sf::Color( 181, 182, 228, 255 ) );
    mText.setPosition( sf::Vector2f( context.window->getView( ).getCenter().x, context.window->getView( ).getCenter().y + (context.window->getView( ).getSize().y / 4) ) );
}

TitleState::~TitleState( )
{
    mContext.textures->remove( TextureMap.at( "TitleScreen" ) );
}

void TitleState::draw( )
{
    sf::RenderTarget& window = *getContext( ).window;
    window.draw( mBackgroundSprite );

    if ( mShowText )
        window.draw( mText );
}

bool TitleState::update(sf::Time dt)
{
	mTextEffectTime += dt;

    if( mTextEffectTime >= sf::seconds( 0.5f ) )
	{
		mShowText = !mShowText;
		mTextEffectTime = sf::Time::Zero;
	}

	return true;
}

bool TitleState::handleEvent( const sf::Event& event )
{
	// If any key is pressed, trigger the next screen
    if( event.type == sf::Event::KeyReleased || event.type == sf::Event::JoystickButtonPressed )
	{
        requestStackPop( );
        requestStackPush( States::Menu );
	}

	return true;
}

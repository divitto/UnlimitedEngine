#include "SettingsState.hpp"
#include "Core/Utility.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/DataTables.hpp"


#include <SFML/Graphics/RenderWindow.hpp>

SettingsState::SettingsState(States::ID id, StateStack& stack, Context context)
: State( id, stack, context)
, mGUIContainer()
{
    mContext.textures->load( TextureMap.at( "Desert" ), MediaFileMap.at( "Textures" ).at( TextureMap.at( "Desert" ) ) );
    mBackgroundSprite.setTexture(context.textures->get(TextureMap.at( "Desert" ) ) );
	
	// Build key binding buttons and labels
    addButtonLabel( Player::MoveLeft,       50.f,  "Move Left",     context );
    addButtonLabel( Player::MoveRight,      100.f, "Move Right",    context );
    addButtonLabel( Player::MoveUp,         150.f, "Move Up",       context );
    addButtonLabel( Player::MoveDown,       200.f, "Move Down",     context );
    addButtonLabel( Player::Fire,           250.f, "Fire Gun",      context );
    addButtonLabel( Player::LaunchMissile,  300.f, "Launch Missle", context );

    updateLabels( );

    auto backButton = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    backButton->setPosition( 380.f, 380.f );
    backButton->setText( "Back" );
    backButton->setCallback( std::bind( &SettingsState::requestStackPop, this ) );

    mGUIContainer.pack( backButton );
}

SettingsState::~SettingsState( void )
{
    mContext.textures->remove( TextureMap.at( "Desert" ) );
}

void SettingsState::draw( )
{
    sf::RenderTarget& window = *getContext( ).window;

    window.draw( mBackgroundSprite );
    window.draw( mGUIContainer );
}

bool SettingsState::update( sf::Time )
{
	return true;
}

bool SettingsState::handleEvent( const sf::Event& event )
{
	bool isKeyBinding = false;
	
	// Iterate through all key binding buttons to see if they are being pressed, waiting for the user to enter a key
    for( std::size_t action = 0; action < Player::ActionCount; ++action )
	{
        if( mBindingButtons[action]->isActive( ) )
		{
			isKeyBinding = true;
            if( event.type == sf::Event::KeyReleased )
			{
                getContext( ).player->assignKey( static_cast<Player::Action>( action ), event.key.code );
                mBindingButtons[action]->deactivate( );
			}
			break;
		}
	}

	// If pressed button changed key bindings, update labels; otherwise consider other buttons in container
    if( isKeyBinding )
        updateLabels( );
	else
        mGUIContainer.handleEvent( event );
	return false;
}

void SettingsState::updateLabels( )
{
    Player& player = *getContext( ).player;

    for( std::size_t i = 0; i < Player::ActionCount; ++i )
	{
        sf::Keyboard::Key key = player.getAssignedKey( static_cast<Player::Action>( i ) );
        mBindingLabels[i]->setText( toString( key ) );
	}
}

void SettingsState::addButtonLabel( Player::Action action, float y, const std::string& text, Context context )
{
    mBindingButtons[action] = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    mBindingButtons[action]->setPosition( 80.f, y );
    mBindingButtons[action]->setText( text );
    mBindingButtons[action]->setToggle( true );

    mBindingLabels[action] = std::make_shared<GUI::Label>( "", *context.fonts );
	mBindingLabels[action]->setPosition(300.f, y + 15.f);

    mGUIContainer.pack( mBindingButtons[action] );
    mGUIContainer.pack( mBindingLabels[action] );
}
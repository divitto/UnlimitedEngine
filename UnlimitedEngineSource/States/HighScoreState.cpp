#include "HighScoreState.hpp"
#include "Gui/Button.hpp"
#include "Core/Utility.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/DataTables.hpp"
#include "Core/Globals.hpp"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


#include <fstream>

HighScoreState::HighScoreState( States::ID id, StateStack& stack, Context context )
: State( id, stack, context )
, mSceneGraph( )
, mGUIContainer( )
, mBackgroundRect( sf::Vector2f( 800, 600 ) )
{
    mContext.textures->load( TextureMap.at( "TitleScreen" ), MediaFileMap.at( "Textures" ).at( TextureMap.at( "TitleScreen" ) ) );
    mBgSprite = sf::Sprite( context.textures->get( TextureMap.at( "TitleScreen" ) ) );
    mText = sf::Text( "HIGH SCORES!", context.fonts->get( FontMap.at( "Default" ) ) );
    mText.setPosition( sf::Vector2f( WINDOW_WIDTH / 2 - 100, 75 ) );

    mBackgroundRect.setFillColor( sf::Color( 0, 0, 0, 100 ) );
    mBackgroundRect.setPosition( 100, 50 );

    // Display high scores
    for( unsigned int i = 0; i < 10; ++i )
    {
        std::unique_ptr<TextNode> score( new TextNode( *context.fonts, "" ) );
        score->setColor( sf::Color( 181, 182, 228, 255 ) );
        if( i != 0 )
            score->setString( std::to_string( i + 1 ) + " .] " + context.highScores->at( i ).first + " " + std::to_string( context.highScores->at( i ).second ) );
        else
            score->setString( std::to_string( i + 1 ) + " .] " + context.highScores->at( i ).first + " " + std::to_string( context.highScores->at( i ).second ) );

        score->setPosition( 500, 100 + (50 * ( i + 1 ) ) );
        mSceneGraph.attachChild( std::move( score ) );
    }

    auto exitButton = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    exitButton->setPosition( WINDOW_WIDTH - 225, WINDOW_HEIGHT - 75 );
    exitButton->setText( "Gotcha!" );
    exitButton->setCallback( [this] ( )
    {
        requestStackPop( );
        requestStackPush( States::Menu );
    });

    mGUIContainer.pack( exitButton );

    // Make sure the latest high scores are saved for the next play thruugh
    saveHighScoresToFile( );
}

HighScoreState::~HighScoreState( )
{
    mContext.textures->remove( TextureMap.at( "TitleScreen" ) );
}

void HighScoreState::draw( )
{
    sf::RenderTarget& window = *getContext( ).window;

    window.setView( window.getDefaultView( ) );

    window.draw( mBgSprite );
    window.draw( mBackgroundRect );
    window.draw( mText );

    window.draw( mSceneGraph );

    window.draw( mGUIContainer );
}

bool HighScoreState::update( sf::Time )
{
    return true;
}

bool HighScoreState::handleEvent( const sf::Event& event )
{
    mGUIContainer.handleEvent( event );
    return false;
}

void HighScoreState::saveHighScoresToFile( void )
{
    std::ofstream file;
    file.open( "Game/HighScores.lua", std::ios::out | std::ios::trunc );
    if( file.is_open( ) )
    {
        file << "return\n{\n\t[0] = { name = \"" + mContext.highScores->at( 9 ).first + "\"; score = " + std::to_string( mContext.highScores->at( 9 ).second ) + "; },\n";
        for( unsigned int i = 0; i < mContext.highScores->size() - 1; ++i )
            file << "{ name = \"" + mContext.highScores->at( i ).first + "\"; score = " + std::to_string( mContext.highScores->at( i ).second ) + "; },\n";
        file << "};";
        file.close( );
    }
}

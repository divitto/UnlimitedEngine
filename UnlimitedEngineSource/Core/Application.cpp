#include "Application.hpp"
#include "Utility.hpp"
#include "State.hpp"
#include "StateIdentifiers.hpp"
#include "ResourceIdentifiers.hpp"
#include "States/TitleState.hpp"
#include "States/MenuState.hpp"
#include "States/PauseState.hpp"
#include "States/LoadingState.hpp"
#include "States/SettingsState.hpp"
#include "States/GameOverState.hpp"
#include "States/GameState.hpp"
#include "States/HighScoreState.hpp"
#include "States/GetNameState.hpp"
#include "States/StartLevelState.hpp"
#include "DataTables.hpp"
#include "Globals.hpp"


// Debugging only!!
#include <iostream>

const sf::Time Application::TimePerFrame = sf::seconds( 1.f/60.f );

Application::Application( )
    : mWindow( sf::VideoMode( WINDOW_WIDTH, WINDOW_HEIGHT ), "Super Rescue Raid!", sf::Style::Default )
, mTextures( )
, mFonts( )
, mPlayer( )
, mMusic( )
, mStateStack( State::Context( States::None, mWindow, mTextures, mFonts, mMusic, mSoundEffects, mPlayer, new std::vector<std::pair<std::string, int>>() ) )
, mStatisticsNumFrames( 0 )
, mStatisticsText( )
, mStatisticsUpdateTime( )
{
    mWindow.setKeyRepeatEnabled( false );
    mWindow.setVerticalSyncEnabled( true );

       mFonts.load(    FontMap.at( "Default"         ), MediaFileMap.at( "Fonts"    ).at(    FontMap.at( "Default"  ) ) );
    mTextures.load( TextureMap.at( "Buttons"         ), MediaFileMap.at( "Textures" ).at( TextureMap.at( "Buttons"  ) ) );

    mStatisticsText.setFont( mFonts.get( FontMap.at( "Default" ) ) );
    mStatisticsText.setPosition( 5.f, 5.f );
    mStatisticsText.setCharacterSize( 10u );

    registerStates( );
    mStateStack.pushState( States::Title );
}

void Application::run( )
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while( mWindow.isOpen( ) )
	{
        sf::Time dt = clock.restart( );
		timeSinceLastUpdate += dt;
        while( timeSinceLastUpdate > TimePerFrame )
		{
			timeSinceLastUpdate -= TimePerFrame;

			processInput();
            update( TimePerFrame );

			// Check inside this loop, because stack might be empty before update() call
            if( mStateStack.isEmpty( ) )
                mWindow.close( );
		}

        updateStatistics( dt );
        render( );
	}
}

void Application::processInput( )
{
	sf::Event event;
    while( mWindow.pollEvent( event ) )
	{
        mStateStack.handleEvent( event );

        if( event.type == sf::Event::Closed )
            mWindow.close( );
	}
}

void Application::update( sf::Time dt )
{
    mStateStack.update( dt );
}

void Application::render( )
{
    mWindow.clear( sf::Color::Black );
    mStateStack.draw( );
    mWindow.setView( mWindow.getDefaultView( ) );
    mWindow.draw( mStatisticsText );
    mWindow.display( );
}

void Application::updateStatistics( sf::Time dt )
{
	mStatisticsUpdateTime += dt;
	mStatisticsNumFrames += 1;
    if( mStatisticsUpdateTime >= sf::seconds( 1.0f ) )
	{
        mStatisticsText.setString( "FPS: " + toString( mStatisticsNumFrames ) );

        mStatisticsUpdateTime -= sf::seconds( 1.0f );
		mStatisticsNumFrames = 0;
	}
}

void Application::registerStates( )
{
    mStateStack.registerState<TitleState>       ( States::Title             );
    mStateStack.registerState<LoadingState>     ( States::Loading           );
    mStateStack.registerState<MenuState>        ( States::Menu              );
    mStateStack.registerState<GameState>        ( States::Game              );
    mStateStack.registerState<PauseState>       ( States::Pause             );
    mStateStack.registerState<SettingsState>    ( States::Settings          );
    mStateStack.registerState<GameOverState>    ( States::GameOver          );
    mStateStack.registerState<HighScoreState>   ( States::HighScoreState    );
    mStateStack.registerState<GetNameState>     ( States::GetNameState      );
    mStateStack.registerState<StartLevelState>  ( States::StartLevelState   );
}


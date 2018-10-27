#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "DataTables.hpp"
#include "ResourceManager.hpp"
#include "Player.hpp"
#include "StateStack.hpp"
#include "Engine/MusicPlayer.hpp"
#include "Engine/SoundPlayer.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

extern "C" {
    #include "lua/lua.h"
    #include "lua/lualib.h"
    #include "lua/lauxlib.h"
}

class Application
{
public:
    Application();
    void run();
		
private:
    //## Attributes
    static const sf::Time TimePerFrame;


    // Engine Resources
    sf::RenderWindow	  mWindow;

    TextureManager		  mTextures;
    FontManager  		  mFonts;
    Player				  mPlayer;
    MusicPlayer           mMusic;
    SoundPlayer          mSoundEffects;

    // Game FSM
    StateStack			  mStateStack;

    // Debuging info
    std::size_t			  mStatisticsNumFrames;
    sf::Text			  mStatisticsText;
    sf::Time			  mStatisticsUpdateTime;


    //## Behaviors
    ///
    /// \brief processInput
    /// Handle any user input or other world events that occur.
    void processInput();
    ///
    /// \brief update
    /// update the world logic for this frame in the game loop
    /// \param elapsedTime
    /// the time since the last time this method was called.
    ///
    void update(sf::Time dt);
    ///
    /// \brief render
    /// Clear the screen, draw the world to buffer, and then display to screen.
    ///
    void render();
    ///
    /// \brief updateStatistics
    /// Calculate and update the frames per seconds the game is currently getting on the fps screen display.
    /// \param elapsedTime
    /// The time since the last time this method was called.
    ///
    void updateStatistics(sf::Time dt);
    ///
    /// \brief registerStates
    /// Register all possible states for later use
    void registerStates();
};

#endif // APPLICATION_HPP

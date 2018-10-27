#ifndef MUSICPLAYER_HPP
#define MUSICPLAYER_HPP

#include "Engine/ResourceManager.hpp"
#include "Engine/ResourceIdentifiers.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Audio/Music.hpp>

#include <map>
#include <string>

class MusicPlayer : private sf::NonCopyable
{
	public:
        MusicPlayer( void );

        void play( unsigned int theme );
        void stop( );

        void setPaused( bool paused );
        void setVolume( float volume );

	private:
		sf::Music							mMusic;
        std::map<unsigned int, std::string>	mFilenames;
		float								mVolume;
};

#endif // MUSICPLAYER_HPP

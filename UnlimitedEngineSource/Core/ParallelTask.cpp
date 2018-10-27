#include "ParallelTask.hpp"

ParallelTask::ParallelTask()
: mThread( &ParallelTask::runTask, this )
, mFinished( false )
, mCompletion( 0.0f )
{
}

void ParallelTask::execute( State::Context context, std::string fileName )
{
    mContext = context;
    mFileName = fileName;
	mFinished = false;

    mThread.launch( );
}

bool ParallelTask::isFinished( )
{
    sf::Lock lock( mMutex );
	return mFinished;
}

float ParallelTask::getCompletion()
{
    sf::Lock lock( mMutex );
    return mCompletion;
}

void ParallelTask::runTask()
{
    sf::Lock lock( mMutex );

    lua_State* L = luaL_newstate();
    luaL_openlibs( L );

    // call with error checking
    lua_getglobal( L, "debug" );
    lua_getfield( L, -1, "traceback" );
    lua_replace( L, -2 );
    luaL_loadfile( L, "Game/Game.lua" );
    if ( lua_pcall( L, 0, LUA_MULTRET, -2 ) ) {
        luaL_traceback( L, L, lua_tostring( L, -1 ), 1 );
        std::cout << "ERROR: " << lua_tostring( L, -1 ) << std::endl;
        throw( lua_tostring( L, -1 ) );
    }
    if( lua_istable( L, -1 ) )
    {
        lua_pushstring( L, "Textures" );
        lua_gettable( L, -2 );
        if( lua_istable( L, -1 )  )
        {
            lua_pushnil( L );
            while( lua_next( L, -2 ) != 0 )
            {
                mContext.textures->load( TextureMap.at( lua_tostring( L, -1 ) ), MediaFileMap.at( "Textures" ).at( TextureMap.at( lua_tostring( L, -1 ) ) ) );
                lua_pop( L, 1 );
            }
            lua_pop( L ,1 );
        }
        if( lua_istable( L, -1 ) )
        {
            lua_pushstring( L, "Fonts" );
            lua_gettable( L, -2 );
            if( lua_istable( L, -1 )  )
            {
                lua_pushnil( L );
                while( lua_next( L, -2 ) != 0 )
                {
                    mContext.fonts->load( FontMap.at( lua_tostring( L, -1 ) ), MediaFileMap.at( "Fonts" ).at( FontMap.at( lua_tostring( L, -1 ) ) ) );
                    lua_pop( L, 1 );
                }
                lua_pop( L ,1 );
            }
        }
        if( lua_istable( L, -1 ) )
        {
            lua_pushstring( L, "SoundEffects" );
            lua_gettable( L, -2 );
            if( lua_istable( L, -1 ) )
            {
                lua_pushnil( L );
                while( lua_next( L, -2 ) != 0 )
                {
                    // is map being loaded right?
                    //std::cout << SoundEffectMap.at( lua_tostring( L, -1 ) ) << ", " <<  MediaFileMap.at( "SoundEffects" ).at( SoundEffectMap.at( lua_tostring( L, -1 ) ) ) << std::endl;
                    mContext.sounds->mSoundBuffers.load( SoundEffectMap.at( lua_tostring( L, -1 ) ), MediaFileMap.at( "SoundEffects" ).at( SoundEffectMap.at( lua_tostring( L, -1 ) ) ) );
                    lua_pop( L, 1 );
                }
                lua_pop( L, 1 );
            }
        }
    } else std::cout << "Error reading " << mFileName.c_str( ) << " data table" << std::endl;

    // call with error checking
    lua_getglobal( L, "debug" );
    lua_getfield( L, -1, "traceback" );
    lua_replace( L, -2 );
    luaL_loadfile( L, "Game/HighScores.lua" );
    if ( lua_pcall( L, 0, LUA_MULTRET, -2 ) ) {
        luaL_traceback( L, L, lua_tostring( L, -1 ), 1 );
        std::cout << "ERROR: " << lua_tostring( L, -1 ) << std::endl;
        throw( lua_tostring( L, -1 ) );
    }
    if( lua_istable( L, -1 ) ) // high scores
    {
        if( mContext.highScores->empty( ) )
        {
            lua_pushnil( L );
            while( lua_next( L, -2 ) != 0 )
            {
                std::string name = "";
                int score = 0;
                if( lua_istable( L, -1 ) )
                {
                    lua_getfield( L, -1, "name" );
                    name = lua_tostring( L, -1 );
                    lua_pop( L, 1 ); // name
                    lua_getfield( L, -1, "score" );
                    score = (int)lua_tonumber( L, -1 );
                    lua_pop( L, 1 );
                }
                mContext.highScores->push_back( std::pair<std::string, int>( name, score ) );
                lua_pop( L, 1 );
            }
            lua_pop( L ,1 );
        }
    }
    lua_close( L );
    mCompletion = 100.0f;
	{ // mFinished may be accessed from multiple threads, protect it
        sf::Lock lock( mMutex );
		mFinished = true;
	}	
}

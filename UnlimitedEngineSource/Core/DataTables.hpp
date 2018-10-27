#ifndef DATATABLES_HPP
#define DATATABLES_HPP

#include "ResourceManager.hpp"
#include "ResourceIdentifiers.hpp"


#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <vector>
#include <functional>

extern "C" {
    #include "lua/lua.h"
    #include "lua/lualib.h"
    #include "lua/lauxlib.h"
}
#include <iostream>

class Entity;
class Aircraft;

extern std::string CurrentLuaFile;

struct Direction
{
	Direction(float angle, float distance)
	: angle(angle)
	, distance(distance)
	{
	}

	float angle;
	float distance;
};
struct AircraftData
{
    std::string                     id;
	int								hitpoints;
	float							speed;
    unsigned int					texture;
    sf::Rect<int>                   texRect;
    sf::Time                        fireInterval;
    bool                            hasRollAnimation;
    unsigned int                    fireRate;
    unsigned int                    spreadLevel;
    unsigned int                    missileAmmo;
    std::string                     explosionAnimation; // not sure what data type this should be yet....
	std::vector<Direction>			directions;

};
struct ProjectileData
{
	int								damage;
	float							speed;
    unsigned int					texture;
    sf::Rect<int>                   texRect;
};
struct PickupData
{
	std::function<void(Aircraft&)>	action;
    unsigned int					texture;
    sf::Rect<int>                   texRect;
};
struct AnimationData  {
    unsigned int texture;
    sf::Vector2i frameSize;
    unsigned int numFrames;
    sf::Time duration;
    bool isRepeated;
};
struct ParticleData
{
    sf::Vector2i position;
    sf::Color    color;
    sf::Time     lifetime;
};

static std::map<std::string, unsigned int> TextureMap;
static std::map<std::string, unsigned int> FontMap;
static std::map<std::string, unsigned int> SoundEffectMap;
static std::map<std::string, unsigned int> MusicMap;

static bool loadAssetsLuaFile = []() -> bool {
        lua_State* L = luaL_newstate();
        luaL_openlibs( L );
        lua_getglobal( L, "debug" );
        lua_getfield( L, -1, "traceback" );
        lua_replace( L, -2 );
        luaL_loadfile( L, "Game/Resources/Assets.lua" );
        if ( lua_pcall( L, 0, LUA_MULTRET, -2 ) ) {
            luaL_traceback( L, L, lua_tostring( L, -1 ), 1 );
            std::cout << "ERROR: " << lua_tostring( L, -1 ) << std::endl;
            throw( lua_tostring( L, -1 ) );
        }
        auto getMap = [&]( std::string key ) -> std::map<std::string, unsigned int> {
            std::map<std::string, unsigned int> map;
            lua_getfield( L, -1, key.c_str() );
            if( lua_istable( L, -1 )  ) {
                lua_pushnil( L );
                while( lua_next( L, -2 ) != 0 ) {
                    map.insert( std::pair<std::string, int>( lua_tostring( L , -2 ), lua_tonumber( L , -1 ) ) );
                    lua_pop( L, 1 );
                }
                lua_pop( L ,1 );
            } else throw( "Error reading Assets.lua" );
            return map;
        };

        TextureMap      = getMap( "Textures"     );
        FontMap         = getMap( "Fonts"        );
        SoundEffectMap  = getMap( "SoundEffects" );
        MusicMap        = getMap( "Music"        );

        lua_close( L );
        return true;
}();
static std::map<std::string, std::map<unsigned int, std::string>> MediaFileMap = []( ) -> std::map<std::string, std::map<unsigned int, std::string>>
{
    std::map<std::string, std::map<unsigned int, std::string>> t;

    lua_State* L = luaL_newstate();
    luaL_openlibs( L );

    // call with error checking
    lua_getglobal( L, "debug" );
    lua_getfield( L, -1, "traceback" );
    lua_replace( L, -2 );
    luaL_loadfile( L, "Game/Resources/SourceFileMap.lua" );
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
            std::map<unsigned int, std::string> texSource;
            lua_pushnil( L );
            while( lua_next( L, -2 ) != 0 )
            {
                texSource.insert( std::pair<unsigned int, std::string>( lua_tonumber( L , -2 ), lua_tostring( L , -1 ) ) );
                lua_pop( L, 1 );
            }
            t.insert( std::pair<std::string, std::map<unsigned int, std::string>>( "Textures", texSource ) );
            lua_pop( L ,1 );
        }
        if( lua_istable( L, -1 ) )
        {
            lua_pushstring( L, "Fonts" );
            lua_gettable( L, -2 );
            if( lua_istable( L, -1 )  )
            {
                std::map<unsigned int, std::string> fontSource;
                lua_pushnil( L );
                while( lua_next( L, -2 ) != 0 )
                {
                    fontSource.insert( std::pair<unsigned int, std::string>( lua_tonumber( L , -2 ), lua_tostring( L , -1 ) ) );
                    lua_pop( L, 1 );
                }
                    t.insert( std::pair<std::string, std::map<unsigned int, std::string>>( "Fonts", fontSource ) );
                    lua_pop( L ,1 );
            }
         }
        if( lua_istable( L, -1 ) )
        {
            lua_pushstring( L, "SoundEffects" );
            lua_gettable( L, -2 );
            if( lua_istable( L, -1 )  )
            {
                std::map<unsigned int, std::string> soundSource;
                lua_pushnil( L );
                while( lua_next( L, -2 ) != 0 )
                {
                    soundSource.insert( std::pair<unsigned int, std::string>( lua_tonumber( L , -2 ), lua_tostring( L , -1 ) ) );
                    lua_pop( L, 1 );
                }
                    t.insert( std::pair<std::string, std::map<unsigned int, std::string>>( "SoundEffects", soundSource ) );
                    lua_pop( L ,1 );
            }
         }
         if( lua_istable( L, -1 ) )
         {
            lua_pushstring( L, "Levels" );
            lua_gettable( L, -2 );
            if( lua_istable( L, -1 )  )
            {
                std::map<unsigned int, std::string> levelSource;
                lua_pushnil( L );
                while( lua_next( L, -2 ) != 0 )
                {
                    levelSource.insert( std::pair<unsigned int, std::string>( lua_tonumber( L , -2 ), lua_tostring( L , -1 ) ) );
                    lua_pop( L, 1 );
                }
                t.insert( std::pair<std::string, std::map<unsigned int, std::string>>( "Levels", levelSource ) );
                lua_pop( L ,1 );
            }
         }
    } else std::cout << "Error reading SourceFileMap.lua" << std::endl;
    lua_close( L );

    return t;
}();
static std::map<std::string, unsigned  int> buildResourceMap( std::string fileName )
{
    std::map<std::string, unsigned int> t;
    lua_State* L = luaL_newstate();
    luaL_openlibs( L );

    // call with error checking
    lua_getglobal( L, "debug" );
    lua_getfield( L, -1, "traceback" );
    lua_replace( L, -2 );
    luaL_loadfile( L, fileName.c_str() );
    if( lua_pcall( L, 0, LUA_MULTRET, -2 ) ) {
        luaL_traceback( L, L, lua_tostring( L, -1 ), 1 );
        throw( lua_tostring( L, -1 ) );
    }
    if( lua_istable( L, -1 ) )  // anon table
    {
        lua_getfield( L, -1, "Map" );
        if( lua_istable( L, -1 ) ) // map table
        {
            lua_pushnil( L );  // first key
            while( lua_next( L, -2 ) != 0 ) {
                t.insert( std::pair<std::string, unsigned int>( lua_tostring( L, -2 ), (unsigned int)lua_tointeger( L, -1 )  ) );
                lua_pop( L, 1); // removes 'value'; keeps 'key' for next iteration
            }
            lua_pop( L, 1 ); // remove table
        }
    } else std::cout << "Error reading " << fileName.c_str( ) << " data table" << std::endl;
    lua_close( L );
    return t;
}

static std::map<std::string, unsigned int> AircraftMap    = buildResourceMap( "Game/Resources/Aircrafts.lua" );
static std::map<std::string, unsigned int> ProjectileMap  = buildResourceMap( "Game/Resources/Projectiles.lua" );
static std::map<std::string, unsigned int> PickupMap      = buildResourceMap( "Game/Resources/Pickups.lua" );
static std::map<std::string, unsigned int> ParticleMap    = buildResourceMap( "Game/Resources/Particles.lua" );
static std::map<std::string, unsigned int> AnimationMap   = buildResourceMap( "Game/Resources/Animations.lua" );


static std::vector<AircraftData> initializeAircraftData = []()-> std::vector<AircraftData> {
        lua_State* L = luaL_newstate();
        luaL_openlibs(L);
        lua_getglobal( L, "debug" );
        lua_getfield( L, -1, "traceback" );
        lua_replace( L, -2 );
        luaL_loadfile( L, "Game/Resources/Aircrafts.lua" );
        if ( lua_pcall( L, 0, LUA_MULTRET, -2 ) )
        {
            luaL_traceback( L, L, lua_tostring( L, -1 ), 1 );
            std::cout << "ERROR: " << lua_tostring( L, -1 ) << std::endl;
            throw( lua_tostring( L, -1 ) );
        }

        std::vector<AircraftData> data( AircraftMap.size( ) );

        if( lua_istable( L, -1 ) ) // Anon table
        {
            for( auto i = AircraftMap.begin(); i != AircraftMap.end(); ++i )
            {
                lua_getfield( L, -1, i->first.c_str( ) );
                if( lua_istable( L, -1 ) ) // Aircraft Definition
                {
                    lua_getfield( L, -1, "id" );
                    if( lua_isstring( L, -1 ) ) data[i->second].id  = lua_tostring( L, -1 ); else throw( "Error reading Aircrafts.lua" );
                    lua_pop( L, 1 ); // id
                    lua_getfield( L, -1, "hitpoints" );
                    if( lua_isnumber( L, -1 ) ) data[i->second].hitpoints  = (unsigned int)lua_tonumber( L, -1 ); else throw( "Error reading Aircrafts.lua" );
                    lua_pop( L, 1 ); // hitpoints
                    lua_getfield( L, -1, "speed" );
                    if( lua_isnumber( L, -1 ) ) data[i->second].speed    = lua_tonumber( L, -1 ); else throw( "Error reading Aircrafts.lua" );
                    lua_pop( L, 1 ); // speed
                    lua_getfield( L, -1, "texture" );
                    if( lua_isnumber( L, -1 ) ) data[i->second].texture  = lua_tointeger( L, -1 ); else throw( "Error reading Aircrafts.lua" );
                    lua_pop( L, 1 ); // texture
                    // Get Texture Rect
                    int rect[4];
                    lua_getfield( L, -1, "texRect" );
                    if( lua_istable( L, -1 ) ){
                        unsigned int i = 0;
                        lua_pushnil( L );
                        while( lua_next( L, -2 ) != 0 ) {
                            rect[i] = lua_tointeger( L, -1 );
                            ++i;
                            lua_pop( L, 1 );
                        }
                    }
                    data[i->second].texRect = sf::Rect<int>( rect[0], rect[1], rect[2], rect[3] );
                    lua_pop( L, 1 ); // TextureRect
                    lua_getfield( L, -1, "fireInterval" );
                    if( lua_isnumber( L, -1 ) ) data[i->second].fireInterval  = sf::milliseconds((int)lua_tonumber( L, -1 )); else throw( "Error reading fireInterval, Aircrafts.lua" );
                    lua_pop( L, 1 ); // fireInterval
                    lua_getfield( L, -1, "hasRollAnimation" );
                    if( lua_isboolean( L, -1 ) ) data[i->second].hasRollAnimation = lua_toboolean( L, -1 ); else throw( "Error reading hasRollAnimation, Aircrafts.lua" );
                    lua_pop( L, 1 );    // hasRollAnimation
                    lua_getfield( L, -1, "fireRate" );
                    if( lua_isnumber( L, -1 ) ) data[i->second].fireRate  = (unsigned int)lua_tonumber( L, -1 ); else throw( "Error reading fireRate, Aircrafts.lua" );
                    lua_pop( L, 1 ); // fireRate
                    lua_getfield( L, -1, "spreadLevel" );
                    if( lua_isnumber( L, -1 ) ) data[i->second].spreadLevel  = (unsigned int)lua_tonumber( L, -1 ); else throw( "Error reading spreadLevel, Aircrafts.lua" );
                    lua_pop( L, 1 ); // spreadLevel
                    lua_getfield( L, -1, "missileAmmo" );
                    if( lua_isnumber( L, -1 ) ) data[i->second].missileAmmo  = (unsigned int)lua_tonumber( L, -1 ); else throw( "Error reading missileAmmo, Aircrafts.lua" );
                    lua_pop( L, 1 ); // missleAmmo
                    lua_getfield( L, -1, "explosionAnimation" );
                    if( lua_isstring( L, -1 ) ) data[i->second].explosionAnimation  = lua_tostring( L, -1 ); else throw( "Error reading explosionAnimation, Aircrafts.lua" );
                    lua_pop( L, 1 ); // explosionAnimation

                    // "path finding"
                    lua_getfield( L, -1, "path" );
                    if( lua_istable( L, -1 ) )
                    {
                        unsigned int index = 0;
                        lua_pushnil( L );
                        while( lua_next(L , -2 ) != 0 )
                        {
                            if( lua_istable( L, -1 ) )
                            {
                                lua_pushnil( L );
                                std::vector<float> v;
                                while( lua_next( L, -2 ) != 0 )
                                {
                                    v.push_back((float)lua_tonumber( L, -1 ));
                                    lua_pop( L, 1 );
                                }
                                //std::cout << "attempting: "<< i->second << ": path {" << a << ", " << d << " }" << std::endl;
                                data[i->second].directions.push_back( Direction( v[0], v[1] ) );
                                lua_pop( L, 1 ); // anon vector
                            } else std::cout << "Error reading Aircraft path data" << std::endl;
                            index++;
                        }
                        lua_pop( L, 1 ); // "path" table
                    }
                }
                lua_pop( L, 1 ); // defintion table
            }
            lua_pop( L, 1 ); // anon table
        }else std::cout << "Error reading Aircrafts.lua" << std::endl;

        lua_close( L );
        return data;
}( ); // initializeAircraftData
static std::vector<ProjectileData>	initializeProjectileData = [](){
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    lua_getglobal( L, "debug" );
    lua_getfield( L, -1, "traceback" );
    lua_replace( L, -2 );
    luaL_loadfile( L, "Game/Resources/Projectiles.lua" );
    if ( lua_pcall( L, 0, LUA_MULTRET, -2 ) ) {
        luaL_traceback( L, L, lua_tostring( L, -1 ), 1 );
        std::cout << "ERROR: " << lua_tostring( L, -1 ) << std::endl;
        throw( lua_tostring( L, -1 ) );
    }
    std::vector<ProjectileData> data( ProjectileMap.size( ) );

    if( lua_istable( L, -1 ) ) // Anon table
    {
        for( auto i = ProjectileMap.begin(); i != ProjectileMap.end(); ++i )
        {
            lua_getfield( L, -1, i->first.c_str( ) );
            if( lua_istable( L, -1 ) ) // Projectile Definition
            {
                lua_getfield( L, -1, "damage" );
                if( lua_isnumber( L, -1 ) ) data[i->second].damage   = lua_tonumber( L, -1 ); else throw( "Error reading Projectile.lua" );
                lua_pop( L, 1 ); // damage
                lua_getfield( L, -1, "speed" );
                if( lua_isnumber( L, -1 ) ) data[i->second].speed    = lua_tonumber( L, -1 ); else throw( "Error reading Projectile.lua" );
                lua_pop( L, 1 ); // speed
                lua_getfield( L, -1, "texture" );
                if( lua_isnumber( L, -1 ) ) data[i->second].texture  = (unsigned int)lua_tonumber( L, -1 ); else throw( "Error reading Projectile.lua" );
                lua_pop( L, 1 ); // texture
                // Get Texture Rect
                int rect[4];
                lua_getfield( L, -1, "texRect" );
                if( lua_istable( L, -1 ) ){
                    unsigned int i = 0;
                    lua_pushnil( L );
                    while( lua_next( L, -2 ) != 0 ) {
                        rect[i] = lua_tointeger( L, -1 );
                        ++i;
                        lua_pop( L, 1 );
                    }
                }
                data[i->second].texRect = sf::Rect<int>( rect[0], rect[1], rect[2], rect[3] );
                lua_pop( L, 1 );
            }
            lua_pop( L, 1 ); // defintion table
        }
        lua_pop( L, 1 ); // anon table
    }else std::cout << "Error reading Projectiles.lua" << std::endl;

    lua_close( L );
    return data;
}( ); // initializeProjectileData
static std::vector<ParticleData> initializeParticleData = []() -> std::vector<ParticleData> {
        lua_State* L = luaL_newstate();
        luaL_openlibs(L);
        lua_getglobal( L, "debug" );
        lua_getfield( L, -1, "traceback" );
        lua_replace( L, -2 );
        luaL_loadfile( L, "Game/Resources/Particles.lua" );
        if ( lua_pcall( L, 0, LUA_MULTRET, -2 ) ) {
            luaL_traceback( L, L, lua_tostring( L, -1 ), 1 );
            std::cout << "ERROR: " << lua_tostring( L, -1 ) << std::endl;
            throw( lua_tostring( L, -1 ) );
        }
        std::vector<ParticleData> data( ParticleMap.size( ) );

        if( lua_istable( L, -1 ) ) // Anon table
        {
            for( auto i = ParticleMap.begin(); i != ParticleMap.end(); ++i )
            {
                lua_getfield( L, -1, i->first.c_str( ) );
                if( lua_istable( L, -1 ) ) // Particle Definition
                {
                    // Get Color Rect
                    sf::Uint8 color[3];
                    lua_getfield( L, -1, "color" );
                    if( lua_istable( L, -1 ) ){
                        unsigned int i = 0;
                        lua_pushnil( L );
                        while( lua_next( L, -2 ) != 0 ) {
                            color[i] = (sf::Uint8)lua_tointeger( L, -1 );
                            ++i;
                            lua_pop( L, 1 );
                        }
                    }
                    data[i->second].color = sf::Color( color[0], color[1], color[2] );
                    lua_pop( L, 1 );

                    lua_getfield( L, -1, "lifetime" );
                    if( lua_isnumber( L, -1 ) ) data[i->second].lifetime = sf::milliseconds( (sf::Uint32)lua_tointeger( L, -1 ) );
                    lua_pop( L, 1 );
                }
                lua_pop( L, 1 ); // defintion table
            }
            lua_pop( L, 1 ); // anon table
        }else std::cout << "Error reading Projectiles.lua" << std::endl;

        lua_close( L );
        return data;
}( ); // initializeParticleData
static std::vector<AnimationData> initializeAnimationData = []() -> std::vector<AnimationData> {
        lua_State* L = luaL_newstate();
        luaL_openlibs(L);
        lua_getglobal( L, "debug" );
        lua_getfield( L, -1, "traceback" );
        lua_replace( L, -2 );
        luaL_loadfile( L, "Game/Resources/Animations.lua" );
        if ( lua_pcall( L, 0, LUA_MULTRET, -2 ) ) {
            luaL_traceback( L, L, lua_tostring( L, -1 ), 1 );
            std::cout << "ERROR: " << lua_tostring( L, -1 ) << std::endl;
            throw( lua_tostring( L, -1 ) );
        }
        std::vector<AnimationData> data( AnimationMap.size( ) );
        if( lua_istable( L, -1 ) ) // Anon table
        {
            for( auto i = AnimationMap.begin(); i != AnimationMap.end(); ++i )
            {
                lua_getfield( L, -1, i->first.c_str( ) );
                if( lua_istable( L, -1 ) ) // Animation Definition
                {
                    lua_getfield( L, -1, "texture" );
                    if( lua_isnumber( L, -1 ) ) data[i->second].texture = (unsigned int)lua_tointeger( L, -1 );
                    lua_pop( L, 1 );
                    lua_getfield( L, -1, "frameSize" );
                    if( lua_istable( L, -1 ) )
                    {
                        int frameSize[2];
                        lua_pushnil( L );
                        lua_next( L, -2 );
                        if( lua_isnumber( L, -1 ) ) frameSize[0] = lua_tointeger( L, -1 );
                        lua_pop( L, 1 ); // value
                        lua_next( L, -2 );
                        if( lua_isnumber( L, -1 ) ) frameSize[1] = lua_tointeger( L, -1 );
                        lua_pop( L, 1 ); // value
                        lua_pop( L, 1 ); // nil (key)
                        data[i->second].frameSize = sf::Vector2i( frameSize[0], frameSize[1] );
                    }
                    lua_pop( L, 1 ); // frameSize table
                    lua_getfield( L, -1, "numFrames" );
                    if( lua_isnumber( L, -1 ) ) data[i->second].numFrames = (unsigned int)lua_tointeger( L, -1 );
                    lua_pop( L, 1 );
                    lua_getfield( L, -1, "duration" );
                    if( lua_isnumber( L, -1 ) ) data[i->second].duration = sf::milliseconds((unsigned int)lua_tointeger( L, -1 ));
                    lua_pop( L, 1 );
                    lua_getfield( L, -1, "isRepeated" );
                    if( lua_isboolean( L, -1 ) ) data[i->second].isRepeated = lua_toboolean( L, -1 );
                    lua_pop( L, 1 );
                }
                lua_pop( L, 1 ); // defintion table
            }
            lua_pop( L, 1 ); // anon table
        }else std::cout << "Error reading Projectiles.lua" << std::endl;
        lua_close( L );
        return data;
}( ); // initializeAnimationData
std::vector<PickupData>   initializePickupData( );

#endif // DATATABLES_HPP

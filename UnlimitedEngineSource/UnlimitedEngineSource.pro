TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

CONFIG += c++11
# SFML
LIBS += -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-window -lsfml-system

SOURCES += \
        main.cpp \
    lua/lapi.c \
    lua/lauxlib.c \
    lua/lbaselib.c \
    lua/lbitlib.c \
    lua/lcode.c \
    lua/lcorolib.c \
    lua/lctype.c \
    lua/ldblib.c \
    lua/ldebug.c \
    lua/ldo.c \
    lua/ldump.c \
    lua/lfunc.c \
    lua/lgc.c \
    lua/linit.c \
    lua/liolib.c \
    lua/llex.c \
    lua/lmathlib.c \
    lua/lmem.c \
    lua/loadlib.c \
    lua/lobject.c \
    lua/lopcodes.c \
    lua/loslib.c \
    lua/lparser.c \
    lua/lstate.c \
    lua/lstring.c \
    lua/lstrlib.c \
    lua/ltable.c \
    lua/ltablib.c \
    lua/ltests.c \
    lua/ltm.c \
    lua/lundump.c \
    lua/lutf8lib.c \
    lua/lvm.c \
    lua/lzio.c

HEADERS += \
    lua/lapi.h \
    lua/lauxlib.h \
    lua/lcode.h \
    lua/lctype.h \
    lua/ldebug.h \
    lua/ldo.h \
    lua/lfunc.h \
    lua/lgc.h \
    lua/llex.h \
    lua/llimits.h \
    lua/lmem.h \
    lua/lobject.h \
    lua/lopcodes.h \
    lua/lparser.h \
    lua/lprefix.h \
    lua/lstate.h \
    lua/lstring.h \
    lua/ltable.h \
    lua/ltests.h \
    lua/ltm.h \
    lua/lua.h \
    lua/luaconf.h \
    lua/lualib.h \
    lua/lundump.h \
    lua/lvm.h \
    lua/lzio.h

DISTFILES += \
    Media/Sansation.ttf \
    Media/Music/FuelLow.ogg \
    Media/Music/MenuTheme.ogg \
    Media/Music/MissionTheme.ogg \
    Media/Music/412806__chrisderweg__basic-drums-120-bpm.wav \
    Media/Shaders/Add.frag \
    Media/Shaders/Brightness.frag \
    Media/Shaders/DownSample.frag \
    Media/Shaders/GuassianBlur.frag \
    Media/Shaders/Fullpass.vert \
    Media/Sound/109340__cheesepuff__a-soldier-song-slow0.mp3 \
    Media/Sound/AlliedGunfire.wav \
    Media/Sound/Button.wav \
    Media/Sound/CollectAlly.wav \
    Media/Sound/CollectPickup.wav \
    Media/Sound/EnemyGunfire.wav \
    Media/Sound/Explosion1.wav \
    Media/Sound/FuelLow.wav \
    Media/Sound/LaunchMissile.wav \
    Media/Sound/LetsGo.wav \
    Media/Sound/PlayerDeath.wav \
    Media/Sound/ReturnToBase.wav \
    Media/Sound/TroopCountFull.wav \
    Media/Sound/SoundEffectCredits.txt \
    Media/Sound/Untitled Document \
    Media/Textures/soldier.jpg \
    Media/Textures/TitleScreen.jpg \
    Media/Textures/Buttons.png \
    Media/Textures/Entities.png \
    Media/Textures/Explosion.png \
    Media/Textures/fueltank.png \
    Media/Textures/grass.png \
    Media/Textures/GreenTank.png \
    Media/Textures/GreenTurret.png \
    Media/Textures/Helicopter.png \
    Media/Textures/HelicopterBlades.png \
    Media/Textures/HUDbg.png \
    Media/Textures/landingpad.png \
    Media/Textures/MainMenu.png \
    Media/Textures/Particle.png \
    Media/Textures/soldier.png \
    Media/Textures/trees.png \
    Media/Textures/Wall.png

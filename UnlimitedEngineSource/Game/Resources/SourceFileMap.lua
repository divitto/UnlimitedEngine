local Assets = require( "Game/Resources/Assets" );

local Tex   = Assets.Textures;
local Font  = Assets.Fonts;
local Sound = Assets.SoundEffects;
local Music = Assets.Music;

return {
    Textures = {
        [Tex.Helicopter]            = "Media/Textures/Helicopter.png";
        [Tex.GreenTank]             = "Media/Textures/GreenTank.png";
        [Tex.Ally]                  = "Media/Textures/soldier.png";
        [Tex.Entities]              = "Media/Textures/Entities.png";
        [Tex.Buttons]               = "Media/Textures/Buttons.png";
        [Tex.TitleScreen]           = "Media/Textures/TitleScreen.jpg";
        [Tex.MainMenu]              = "Media/Textures/MainMenu.png";
        [Tex.GameOver]              = "Media/Textures/GameOver.jpg";
        [Tex.Particle]              = "Media/Textures/Particle.png";
        [Tex.Explosion]             = "Media/Textures/Explosion.png";
        [Tex.Tree]                  = "Media/Textures/trees.png";
        [Tex.Fuel]                  = "Media/Textures/fueltank.png";
        [Tex.HUDBackground]         = "Media/Textures/HUDbg.png";
        [Tex.HelicopterBlades]      = "Media/Textures/HelicopterBlades.png";
        [Tex.Grass]                 = "Media/Textures/grass.png";
    };
    Fonts = {
        [Font.Default]              = "Media/Sansation.ttf";
    };
    SoundEffects = {
        [Sound.Explosion]           = "Media/Sound/Explosion1.wav";
        [Sound.PlayerShoot]         = "Media/Sound/AlliedGunfire.wav";
        [Sound.CollectPickup]       = "Media/Sound/CollectPickup.wav";
        [Sound.EnemyGunfire]        = "Media/Sound/EnemyGunfire.wav";
        [Sound.PlayerDeath]         = "Media/Sound/PlayerDeath.wav";
        [Sound.FuelLow]             = "Media/Sound/FuelLow.wav";
        [Sound.TroopCountFull]      = "Media/Sound/TroopCountFull.wav";
        [Sound.ReturnToBase]        = "Media/Sound/ReturnToBase.wav";
        [Sound.LetsGo]              = "Media/Sound/LetsGo.wav";
        [Sound.CollectAlly]         = "Media/Sound/CollectAlly.wav";
    },
    Music = {
        [Music.MenuTheme]           = "Media/Music/MenuTheme.ogg";
        [Music.GameTheme]           = "Media/Music/MissionTheme.ogg";
        [Music.FuelLow]             = "Media/Music/FuelLow.ogg";
    }
};

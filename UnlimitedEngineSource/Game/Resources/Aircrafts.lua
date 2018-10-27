local Textures      = require( "Game/Resources/Assets" ).Textures;
local Animations    = require( "Game/Resources/Animations" );

return {
    Map = {
        PlayerAircraft  = 0;
        GreenTank       = 1;
        Hellcat         = 2;
        Chopper         = 3;
    };

    PlayerAircraft = {
        id                  = "PlayerAircraft";
        hitpoints           = 20.0; -- two hit kill
        speed               = 175.0;
        texture             = Textures.Helicopter;
        --texRect             = { 0, 0, 48, 64 };     -- left, top, width, heght
        texRect = { 0, 0, 64, 64 };
        fireInterval        = 1000;                 -- millisecs
        hasRollAnimation    = false;
        fireRate            = 1;
        spreadLevel         = 1;
        missileAmmo         = 3;
        explosionAnimation  = "Explosion";          -- name of the effect in the animations table
        path = {
        };
    };  -- PlayerAircraft

    GreenTank = {
        id                  = "GreenTank";
        hitpoints           = 20;
        speed               = 50;
        texture             = Textures.GreenTank;
        --texRect             = { 228, 0, 60, 59 };
        texRect = { 0, 0, 48, 64 };
        fireInterval        = 4000;                 --milisecs
        hasRollAnimation    = false;
        fireRate            = 1;
        spreadLevel         = 1;
        missileAmmo         = 0;
        explosionAnimation  = "Explosion";          -- name of the effect in the animations table
        path = {
            [0] = {  90.0, 300.0  },
                  {  90.0, 300.0  },
                  { -90.0, 300.0 },
               };
    };  -- GreenTank

    Chopper = {
        id = "Chopper";
        hitpoints = 20;
        speed = 80;
        texture = Textures.Helicopter;
        texRect = { 0, 0, 64, 64 };
        fireInterval = 4000;
        hasRollAnimation = false;
        fireRate = 1;
        spreadLevel = 2;
        missileAmmo = 3;
        explosionAnimation = "Explosion";
        path = {};
    }; -- Chopper

    Hellcat = {
        id                  = "Hellcat";
        hitpoints           = 10;
        speed               = 200.0;
        texture             = Textures.Entities;
        texRect             = { 0, 0, 48, 64};
        fireInterval        = 2000;
        hasRollAnimation    = false;
        fireRate            = 1;
        spreadLevel         = 1;
        missileAmmo         = 0;
        explosionAnimation  = "Explosion";          -- name of the effect in the animations table
        path = {
           [0] = {  25.0, 100.0 },
                 { -25.0, 200.0 },
                 {  25.0, 100.0 }
        };
    };  -- Hellcat
};

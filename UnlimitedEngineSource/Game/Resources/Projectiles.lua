local Textures = require( "Game/Resources/Assets" ).Textures;

return {
    Map = {
        AlliedBullet    = 0;
        EnemyBullet     = 1;
        TurretBullet    = 2;
        Missile         = 3;
    };

    AlliedBullet = {
        damage  = 10;
        speed   = 300;
        texture = Textures.Entities;
        texRect = { 175, 64, 3, 14 };
    };

    EnemyBullet = {
        damage  = 10.0;
        speed   = 300.0;
        texture = Textures.Entities;
        texRect = { 178, 64, 3, 14 };
    };
    TurretBullet = {
        damage  = 10.0;
        speed   = 100.0;
        texture = Textures.Entities;
        texRect = { 178, 64, 3, 14 };
    };

    Missile     = {
        damage  = 200.0;
        speed   = 150.0;
        texture = Textures.Entities;
        texRect = { 160, 64, 15, 32 };
    };
};

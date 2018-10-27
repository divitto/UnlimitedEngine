local Textures = require( "Game/Resources/Assets" ).Textures;



--
--  THIS FILE IS CURRENTLY NOT IN USE SEE DATATABLES.CPP
--
--
--


return {
    Map = {
        HealthRefill    = 0;
        FireSpread      = 2;
        Fuel            = 4;
    };
    HealthRefill = {
        texture = Textures.Ally;
        --texRect = { 0, 64, 40, 40 };
        texRect = { 0, 0, 24, 48 };
        action = function( obj ) end;
    };
    FireSpread = {
        texture = Textures.Ally;
        --texRect = { 80, 64, 40, 40 };
        texRect = { 0, 0, 24, 48 };
        action = function( obj ) end;
    };
};

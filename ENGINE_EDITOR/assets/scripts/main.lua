-- Main Lua Script

run_script("assets/scripts/asteroids/entityDefs.lua") 
run_script("assets/scripts/asteroids/assetDefs.lua") 
run_script("assets/scripts/asteroids/utilities.lua") 

LoadAssets()

local entity = LoadEntity(ShipDefs["red_ship"])


main = {
    [1] = {
        update = function()

        end
    },
    [2] = {
        render = function()

        end
    },
}
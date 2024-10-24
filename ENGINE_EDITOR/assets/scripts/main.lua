-- Main Lua Script
run_script("assets/scripts/asteroids/entityDefs.lua") 
run_script("assets/scripts/asteroids/assetDefs.lua") 
run_script("assets/scripts/asteroids/utilities.lua") 
run_script("assets/scripts/asteroids/Ship.lua") 


LoadAssets()

local entity = LoadEntity(ShipDefs["red_ship"])
gShip = Ship:Create({id = entity})


main = {
    [1] = {
        update = function()
            gShip:UpdateShip()
        end
    },
    [2] = {
        render = function()

        end
    },
}
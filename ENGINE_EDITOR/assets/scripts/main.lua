-- Main Lua Script
run_script("assets/scripts/asteroids/entityDefs.lua") 
run_script("assets/scripts/asteroids/assetDefs.lua") 
run_script("assets/scripts/asteroids/utilities.lua") 
run_script("assets/scripts/asteroids/Ship.lua") 
run_script("assets/scripts/asteroids/Asteroid.lua") 

math.randomseed(os.time())
LoadAssets()
LoadBackground()

local entity = LoadEntity(ShipDefs["red_ship"])
gShip = Ship:Create({id = entity})



main = {
    [1] = {
        update = function()
            gShip:UpdateShip()
            UpdateAsteroids()
            SpawnAsteroid()
        end
    },
    [2] = {
        render = function()

        end
    },
}
-- Main Lua Script
run_script("assets/scripts/asteroids/utilities.lua") 
run_script("assets/scripts/asteroids/entityDefs.lua") 
run_script("assets/scripts/asteroids/assetDefs.lua") 
run_script("assets/scripts/asteroids/Ship.lua") 
run_script("assets/scripts/asteroids/Asteroid.lua") 
run_script("assets/scripts/asteroids/collision_system.lua") 
run_script("assets/scripts/asteroids/Projectile.lua") 
run_script("assets/scripts/asteroids/game_data.lua") 
run_script("assets/scripts/asteroids/Hud.lua") 

math.randomseed(os.time())
LoadAssets()
LoadBackground()

local entity = LoadEntity(ShipDefs["red_ship"])
gShip = Ship:Create({id = entity})
gCollisionSystem = CollisionSystem:Create()
gHud = Hud:Create()


main = {
    [1] = {
        update = function()
            gShip:UpdateShip()
            UpdateAsteroids()
            UpdateProjectiles()
            gCollisionSystem:Update()
            --SpawnAsteroid()
            --print("LIVES: " ..gData:NumLives())
            --print("SCORE: " ..gData:GetScore())
            gHud:Update()

            if not gData:IsGameOver() then
                SpawnAsteroid()
            else
                if Keyboard.just_pressed(KEY_ENTER) then
                    gData:Reset()
                    gHud:Reset()
                    gShip:Reset()
                    ResetAsteroids()
                    ResetProjectiles()
                end
            end
        end
    },
    [2] = {
        render = function()

        end
    },
}
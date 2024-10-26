-- Asteroids Game Loader
math.randomseed(os.time())

LoadScripts()
LoadAssets()
LoadBackground()

local entity = LoadEntity(ShipDefs["red_ship"])
gShip = Ship:Create({id = entity})
gCollisionSystem = CollisionSystem:Create()
gHud = Hud:Create()

bAsteroidsLoaded = true


function RunAsteroids()

	gShip:UpdateShip()
	UpdateAsteroids()
	UpdateProjectiles()
	gCollisionSystem:Update()
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

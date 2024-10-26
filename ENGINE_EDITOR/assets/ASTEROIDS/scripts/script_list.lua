script_list = 
{
	"assets/ASTEROIDS/scripts/utilities.lua",
	"assets/ASTEROIDS/scripts/game_data.lua",
	"assets/ASTEROIDS/scripts/assetDefs.lua",
	"assets/ASTEROIDS/scripts/entityDefs.lua",
	"assets/ASTEROIDS/scripts/Projectile.lua",
	"assets/ASTEROIDS/scripts/Ship.lua",
	"assets/ASTEROIDS/scripts/Asteroid.lua",
	"assets/ASTEROIDS/scripts/Hud.lua",
	"assets/ASTEROIDS/scripts/collision_system.lua",
}


function LoadScripts()
	for k, v in ipairs(script_list) do
		if not run_script(v) then
			print("Failed to load [" ..v .."]")
		end
	end
end
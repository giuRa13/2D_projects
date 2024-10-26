-- Main Lua Script

--run_script("assets/ASTEROIDS/scripts/script_list.lua") 
--run_script("assets/ASTEROIDS/scripts/main.lua") 
run_script("assets/scripts/TestProject/assetDefs.lua")
run_script("assets/scripts/TestProject/testmap1.lua")
run_script("assets/scripts/utilities.lua")

local tilemap = CreateTestMap()
assert(tilemap)
LoadAssets(AssetDefs)
LoadMap(tilemap)

--Music.play("music1")

main = {
    [1] = {
        update = function()
            --if bAsteroidsLoaded then
                --RunAsteroids()
            --end
            --if Keyboard.just_pressed(KEY_M) then
               --Music.stop()
            --end

        end
    },
    [2] = {
        render = function()

        end
    },
}
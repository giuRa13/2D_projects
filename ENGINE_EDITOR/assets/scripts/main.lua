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

            DrawLine(Line( vec2(50),  vec2(200),  Color(255, 0, 0, 255) ))
            DrawLine(Line( vec2(200, 50),  vec2(50, 200),  Color(0, 255, 0, 255) ))
            DrawRect(Rect( vec2(300, 300),  100,  100,  Color(0, 0, 255, 255) ))
            --DrawCircle(vec2(200, 200), 1.0, 100.0, Color(255, 255, 0, 255))

            DrawText( Text( 
                vec2(100.0, 50.0),
                "This is some text",
                "pixel",
                640.0,
                Color(255, 100, 0, 255)
            ))
            DrawText( Text(
                    vec2(50.0, 250.0),
                    "testing the wrap functionality, this is some text, wrap is 550",
                    "pixel",
                    550.0,
                    Color(255, 100, 0, 255)
            ))
        end
    },
}
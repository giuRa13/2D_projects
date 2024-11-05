-- Main Lua Script

--run_script("assets/ASTEROIDS/scripts/script_list.lua") 
--run_script("assets/ASTEROIDS/scripts/main.lua") 
run_script("assets/scripts/TestProject/testmap1.lua")
run_script("assets/scripts/TestProject/testmap2.lua")
run_script("assets/scripts/TestProject/assetDefs.lua")
run_script("assets/scripts/utilities.lua")
--run_script("assets/scripts/rain_generator.lua")
--run_script("assets/scripts/scene_rain.lua")
--run_script("assets/scripts/follow_cam.lua")
run_script("assets/scripts/events/event_manager.lua")
run_script("assets/scripts/events/collision_event.lua")
run_script("assets/scripts/systems/trigger_system.lua")

run_script("assets/scripts/game_states/game_state.lua")
run_script("assets/scripts/game_states/title_state.lua")
--------------------------------------------------------------------
--local tilemap = CreateTestMap()
--[[local tilemap = CreateTestPlatformerMap()
assert(tilemap)
LoadAssets(AssetDefs)
LoadMap(tilemap)
--------------------------------------------------------------------
Music.play("music1")
Sound.play("rain", -1, 1) -- (-1 ) = loop forever, channel 1
Sound.set_volume(1, 30)
---------------------------------------------------------------------
gCollisionEvent = CollisionEvent:Create()
gTriggerSystem = TriggerSystem:Create()
gCollisionEvent:SubscribeToEvent(gTriggerSystem)]]--
---------------------------------------------------------------------

LoadAssets(AssetDefs)

gFollowCam = nil 
gCollisionEvent = nil 
gTriggerSystem = nil 
gPlayer = nil 
gMainCam = Camera.get()
gMainCam.set_scale(2.0)


gStateStack = StateStack()
local title = TitleState:Create(gStateStack)
gStateStack:change_state(title)


main = {
    [1] = {
        update = function()

            gStateStack:update(0.016)

            if Keyboard.just_pressed(KEY_M) then
               Music.stop()
            end

            --UpdatePlayer(gPlayer)
            --gFollowCam:update()
            Debug()
        end
    },
    [2] = {
        render = function()

            gStateStack:render()

            --DrawLine(Line( vec2(50),  vec2(200),  Color(255, 0, 0, 255) ))
            --DrawLine(Line( vec2(200, 50),  vec2(50, 200),  Color(0, 255, 0, 255) ))
            --DrawRect(Rect( vec2(0, 0),  640,  480,  Color(255, 0, 0, 255) ))
            --DrawCircle(vec2(-100, -150), 1.0, 850.0, Color(0, 0, 0, 100))

            DrawText( Text( 
                vec2(100.0, 200.0),
                "This is some text",
                "pixel",
                640.0,
                Color(255, 150, 0, 255)
            ))
        end
    }
}
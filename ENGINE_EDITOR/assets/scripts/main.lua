-- Main Lua Script

ENGINE_run_script("assets/scripts/script_list.lua")
ENGINE_load_script_table(ScriptList)
LoadAssets(AssetDefs)
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

            --[[DrawText( Text( 
                vec2(100.0, 200.0),
                "This is some text",
                "pixel",
                640.0,
                Color(255, 150, 0, 255)
            ))]]--
        end
    }
}
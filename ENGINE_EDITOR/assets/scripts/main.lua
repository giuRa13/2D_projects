-- Main Lua Script

--run_script("assets/ASTEROIDS/scripts/script_list.lua") 
--run_script("assets/ASTEROIDS/scripts/main.lua") 
--run_script("assets/scripts/TestProject/testmap1.lua")
run_script("assets/scripts/TestProject/testmap2.lua")
run_script("assets/scripts/TestProject/assetDefs.lua")
run_script("assets/scripts/utilities.lua")
run_script("assets/scripts/rain_generator.lua")
run_script("assets/scripts/events/event_manager.lua")
run_script("assets/scripts/events/collision_event.lua")
run_script("assets/scripts/systems/trigger_system.lua")
--------------------------------------------------------------------

--local tilemap = CreateTestMap()
local tilemap = CreateTestPlantformerMap()
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
gCollisionEvent:SubscribeToEvent(gTriggerSystem)
---------------------------------------------------------------------

local rainGen = RainGenerator:Create({scale = 0.5})

gTimer = Timer()
gTimer:start()

--function GetRandomValue(min, max)
	--math.randomseed(get_ticks())
	--return math.random(min, max)
--end
---------------------------------------------------------------------
--[[local ball = Entity("", "")
local circle = ball:add_component(CircleCollider(21.0))
local transform = ball:add_component(Transform( vec2(320, 64), vec2(1, 1), 0))
local physAttr = PhysicsAttributes()
physAttr.eType = BodyType.Dynamic
physAttr.density = 100.0
physAttr.friction = 0.2
physAttr.restitution = 0.1
physAttr.radius = circle.radius * (1.0 / 12.0)
physAttr.gravityScale = 2.0 
physAttr.position = transform.position
physAttr.scale = transform.scale 
physAttr.bCircle = true 
physAttr.bFixedRotation = false 
ball:add_component(PhysicsComp(physAttr))
local sprite = ball:add_component(Sprite("ball", 42, 42, 0, 0, 0))
sprite:generate_uvs()

gFollowCam = FollowCamera(
    FollowCamParams({
        scale = 1,
        max_x = 20000,
        max_y = 2000,
        springback = 1.0
    }),
    ball
)

local bottomEnt = Entity("", "")
local bottomBox = bottomEnt:add_component(BoxCollider(10000, 16, vec2(0, 0)))
local bottomTrans = bottomEnt:add_component(Transform(vec2(0, 464), vec2(1, 1), 0))
local bottomPhys = PhysicsAttributes()
bottomPhys.eType = BodyType.Static
bottomPhys.density = 1000.0
bottomPhys.friction = 0.5 
bottomPhys.restitution = 0.0 
bottomPhys.gravityScale = 1.0 
bottomPhys.position = bottomTrans.position
bottomPhys.scale = bottomTrans.scale 
bottomPhys.boxSize = vec2(bottomBox.width, bottomBox.height)
bottomPhys.bBoxShape = true 
bottomPhys.bFixedRotation = true
bottomEnt:add_component(PhysicsComp(bottomPhys))]]--

--[[local leftEnt = Entity("", "")
local leftBox = leftEnt:add_component(BoxCollider(16, 464, vec2(0, 0)))
local leftTrans = leftEnt:add_component(Transform(vec2(0, 0), vec2(1, 1), 0))
local leftPhys = PhysicsAttributes()
leftPhys.eType = BodyType.Static
leftPhys.density = 1000.0
leftPhys.friction = 0.5 
leftPhys.restitution = 0.0 
leftPhys.gravityScale = 1.0 
leftPhys.position = leftTrans.position
leftPhys.scale = leftTrans.scale 
leftPhys.boxSize = vec2(leftBox.width, leftBox.height)
leftPhys.bBoxShape = true 
leftPhys.bFixedRotation = true
leftEnt:add_component(PhysicsComp(leftPhys))

local rightEnt = Entity("", "")
local rightBox = rightEnt:add_component(BoxCollider(16, 480, vec2(0, 0)))
local rightTrans = rightEnt:add_component(Transform(vec2(624, 0), vec2(1, 1), 0))
local rightPhys = PhysicsAttributes()
rightPhys.eType = BodyType.Static
rightPhys.density = 1000.0
rightPhys.friction = 0.5 
rightPhys.restitution = 0.0 
rightPhys.gravityScale = 1.0 
rightPhys.position = rightTrans.position
rightPhys.scale = rightTrans.scale 
rightPhys.boxSize = vec2(rightBox.width, rightBox.height)
rightPhys.bBoxShape = true 
rightPhys.bFixedRotation = true
rightEnt:add_component(PhysicsComp(rightPhys))

local topEnt = Entity("", "")
local topBox = topEnt:add_component(BoxCollider(608, 16, vec2(0, 0)))
local topTrans = topEnt:add_component(Transform(vec2(16, 0), vec2(1, 1), 0))
local topPhys = PhysicsAttributes()
topPhys.eType = BodyType.Static
topPhys.density = 1000.0
topPhys.friction = 0.5 
topPhys.restitution = 0.0 
topPhys.gravityScale = 1.0 
topPhys.position = topTrans.position
topPhys.scale = topTrans.scale 
topPhys.boxSize = vec2(topBox.width, topBox.height)
topPhys.bBoxShape = true 
topPhys.bFixedRotation = true
topEnt:add_component(PhysicsComp(topPhys))]]--
---------------------------------------------------------------------

--[[local ballCount = 0
local countEnt = Entity("", "")
countEnt:add_component(Transform(vec2(10,32), vec2(1, 1), 0))
countEnt:add_component(TextComponent("pixel", "Ball Count: ", Color(255, 150, 0, 255), 4, -1.0))

local valEnt = Entity("", "")
valEnt:add_component(Transform(vec2(352,32), vec2(1, 1), 0))
local valText = valEnt:add_component(TextComponent("pixel", " 0", Color(255, 150, 0, 255), 4, -1.0))]]--

function createBall()
    if(Mouse.just_released(LEFT_BTN)) then
        local pos = Mouse.world_position()
    
        local ball = Entity("", "")
        local circle = ball:add_component(CircleCollider(21.0))
        local transform = ball:add_component(Transform( vec2(pos.x, pos.y), vec2(1, 1), 0))
        local physAttr = PhysicsAttributes()
        physAttr.eType = BodyType.Dynamic
        physAttr.density = 100.0
        physAttr.friction = 0.2
        physAttr.restitution = 0.5
        physAttr.radius = circle.radius * (1.0 / 12.0)
        physAttr.gravityScale = 2.0 
        physAttr.position = transform.position
        physAttr.scale = transform.scale 
        physAttr.bCircle = true 
        physAttr.bFixedRotation = false 
        ball:add_component(PhysicsComp(physAttr))
        local sprite = ball:add_component(Sprite("ball", 42, 42, 0, 0, 0))
        sprite:generate_uvs()

        ballCount = ballCount + 1
    end
end

function updateEntity(entity)
	local physics = entity:get_component(PhysicsComp)
	local transform = entity:get_component(Transform)

	local velocity = physics:get_linear_velocity()
	if velocity.y > 0.0 then 
		physics:set_gravity_scale(15.0)
	else 
		physics:set_gravity_scale(5.0)
	end 
	if Keyboard.pressed(KEY_D) then 
		physics:set_linear_velocity(vec2(25, velocity.y))
	elseif Keyboard.pressed(KEY_A) then 
		physics:set_linear_velocity(vec2(-25, velocity.y))
	end 
	if Keyboard.just_pressed(KEY_W) then 
		physics:set_linear_velocity(vec2(velocity.x, 0))
		physics:linear_impulse(vec2(0, -30000))
	end
end
----------------------------------------------------------------------------

gPlayer = Entity("player", "")
local playerTransform = gPlayer:add_component(Transform(vec2(16, 416), vec2(1, 1), 0)) -- position 16 is 1 400 is 25
local sprite = gPlayer:add_component(Sprite("player", 32, 32, 0, 0, 2))
sprite:generate_uvs()
gPlayer:add_component(Animation(4, 9, 0, false, true))-- frame, framerate, offset, horizontal, loop
local circleCollider = gPlayer:add_component(CircleCollider(8, vec2(8,12))) -- radius, offset
local playerPhysAttr = PhysicsAttributes()
playerPhysAttr.eType = BodyType.Dynamic
playerPhysAttr.density = 75.0
playerPhysAttr.friction = 1.0
playerPhysAttr.restitution = 0.0
playerPhysAttr.position = playerTransform.position
playerPhysAttr.radius = circleCollider.radius
playerPhysAttr.bCircle = true
playerPhysAttr.bFixedRotation = true
playerPhysAttr.objectData = (ObjectData("player", "", true, false, gPlayer:id()))
gPlayer:add_component(PhysicsComp(playerPhysAttr))

bLeft = false
function UpdatePlayer(player)
    local physics = player:get_component(PhysicsComp)
    local velocity = physics:get_linear_velocity()
    local sprite = player:get_component(Sprite)
    -- stop if not pressing
    physics:set_linear_velocity(vec2(0, velocity.y))
    -- left or right
    if Keyboard.pressed(KEY_A) then
        physics:set_linear_velocity(vec2(-10, velocity.y))
        bLeft = true
        sprite.start_y = 3
    elseif Keyboard.pressed(KEY_D) then
        physics:set_linear_velocity(vec2(10, velocity.y))
        bLeft = false
        sprite.start_y = 2
    end
    -- jump (velocity negative way up, positive way down)
    if Keyboard.just_pressed(KEY_W) then
        physics:set_linear_velocity(vec2(velocity.x, 0))
        physics:linear_impulse(vec2(velocity.x -1500))
    end
    if velocity.y < 0 then
        physics:set_gravity_scale(2)
    elseif velocity.y > 0 then
        physics:set_gravity_scale(2)
    end
    -- reset idle animation
    if velocity.x == 0.0 then
        if bLeft then
            sprite.start_y = 1
        else
            sprite.start_y = 0
        end
    end

    sprite.uvs.v = sprite.start_y * sprite.uvs.uv_height
end

Camera.get().set_scale(2)
gFollowCam = FollowCamera(
    FollowCamParams({
        min_x = 0,
        min_y = 0,
        max_x = 640,
        max_y = 480,
        scale = 2,
    }),
    gPlayer
)
-----------------------------------------------------------------

--[[local objectData = ObjectData("test_tag", "test_group", true, true, 9919)
local userData = UserData.create_user_data(objectData)
local objData1 = userData:get_user_data()
print(objData1:to_string() .."\n")

userData:set_user_data(ObjectData("New Tag", "New group", false, true, 12112))
local objData2 = userData:get_user_data()
print(objData2:to_string())]]--


main = {
    [1] = {
        update = function()
            --if bAsteroidsLoaded then
                --RunAsteroids()
            --end
            if Keyboard.just_pressed(KEY_M) then
               Music.stop()
            end

            rainGen:Update(0.016) -- Add delta time
            UpdatePlayer(gPlayer)
            gFollowCam:update()

            Debug()

            local uda, udb = ContactListener.get_user_data()
            if uda and udb then
                --print("USER A: " ..uda:to_string())
                --print("USER B: " ..udb:to_string())
                gCollisionEvent:EmitEvent(uda, udb)
            end

            --createBall()
            --updateEntity(ball)
            --gFollowCam:update()
            --valText.textStr = tostring(ballCount)
        end
    },
    [2] = {
        render = function()

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
            --[[DrawText( Text(
                    vec2(50.0, 250.0),
                    "testing the wrap functionality, this is some text, wrap is 550",
                    "pixel",
                    550.0,
                    Color(255, 100, 0, 255)
            ))]]--
        end
    }
}
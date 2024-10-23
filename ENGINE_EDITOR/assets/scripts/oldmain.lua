-- old Main Lua Script

gEntity = Entity("TestEntity", "Groupy")
gEntity2 = Entity("TestEntity2", "Groupy")

local transform = gEntity:add_component( Transform(vec2(100, 100), vec2(5, 5), 0) )
local sprite = gEntity:add_component(Sprite("16map", 16.0, 16.0, 13, 13, 0) )
sprite:generate_uvs()

gEntity2:add_component( Transform(250, 100, 5, 5, 0) )
local sprite2 = gEntity2:add_component(Sprite("robot", 32.0, 32.0, 0, 2, 0))
sprite2:generate_uvs()
gEntity2:add_component(Animation(4, 10, 0, false))

local rotation = 0.0
local x_pos = 10.0
local scale = 1.0
local move_right = true
local value = 0


local view = Registry.get_entities(Transform)
--view:exclude(Sprite)
--view:exclude(Transform)

view:for_each(
    function(entity)
        print(entity:name())
    end
)

sprite:generate_uvs()

main = {
    [1] = {
        update = function()
            if rotation >= 360 then
                rotation = 0
            end

            if move_right and x_pos < 300 then
                x_pos = x_pos + 3
                value = value + 0.01
            elseif move_right and x_pos >= 300 then
                move_right = false
            end

            if not move_right and x_pos > 10 then
                x_pos = x_pos -3
                value = value - 0.01
            elseif not move_right and x_pos <= 10 then
                move_right = true
            end
        
            transform.position.x = x_pos
            transform.rotation = rotation

            if move_right then
                rotation = rotation + 9
            else
                rotation = rotation -9
            end

            scale = math.sin(value) * 10

            if scale >= 10 then
                scale = 10
            elseif scale <= 1 then
                scale = 1
            end

            if value >= 1 then
                value = 1
            elseif value <= 0 then
                value = 0
            end

            transform.scale = vec2(scale, scale)

            --local sprite2 = gEntity:get_component(Sprite)
            --print("Texture name: " ..sprite2.texture_name)

            if Keyboard.just_pressed(KEY_W) then
                print("Key W was just pressed")
            elseif Keyboard.just_pressed(KEY_S) then
                print("Key S was just pressed")
            elseif Keyboard.just_pressed(KEY_A) then
                print("Key A was just pressed")
            elseif Keyboard.just_pressed(KEY_D) then
                print("Key D was just pressed")
            end

            if Mouse.just_pressed(LEFT_BTN) then
                print("Mouse LEFT Button Pressed")
            elseif Mouse.just_pressed(MIDDLE_BTN) then
                print("Mouse MIDDLE Button Pressed")
            elseif Mouse.just_pressed(RIGHT_BTN) then
                print("Mouse RIGHT Button Pressed")
            end

            local mouse_x, mouse_y = Mouse.screen_position()
            print("Mouse pos[" ..mouse_x .."," ..mouse_y .."]")

        end
    },
    [2] = {
        render = function()

        end
    }
}
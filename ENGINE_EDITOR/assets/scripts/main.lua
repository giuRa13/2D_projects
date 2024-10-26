-- Main Lua Script

run_script("assets/ASTEROIDS/scripts/script_list.lua") 
run_script("assets/ASTEROIDS/scripts/main.lua") 

Music.play("music1")

main = {
    [1] = {
        update = function()
            if bAsteroidsLoaded then
                RunAsteroids()
            end

            if Keyboard.just_pressed(KEY_M) then
               Music.stop()
            end

        end
    },
    [2] = {
        render = function()

        end
    },
}
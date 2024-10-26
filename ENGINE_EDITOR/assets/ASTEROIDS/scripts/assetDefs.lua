AssetDefs =
{
    textures = 
    {
        { name = "ship", path = "assets/ASTEROIDS/textures/ship_blue.png", pixel_art = true},
        { name = "ast_1", path = "assets/ASTEROIDS/textures/ast_1.png", pixel_art = true},
        { name = "ast_2", path = "assets/ASTEROIDS/textures/ast_2.png", pixel_art = true},
        { name = "ast_3", path = "assets/ASTEROIDS/textures/ast_3.png", pixel_art = true},
        { name = "bg", path = "assets/ASTEROIDS/textures/darkPurple.png", pixel_art = true},
        { name = "proj_1", path = "assets/ASTEROIDS/textures/laserGreen.png", pixel_art = true},
        --{ name = "lives", path = "assets/textures/yellow_star.png", pixel_art = true},
        { name = "lives", path = "assets/ASTEROIDS/textures/green_shield.png", pixel_art = true},
        { name = "game_over", path = "assets/ASTEROIDS/textures/gameover.png", pixel_art = true},
        { name = "score", path = "assets/ASTEROIDS/textures/score.png", pixel_art = true},
        { name = "numbers", path = "assets/ASTEROIDS/textures/numbers.png", pixel_art = true},
        { name = "ship_explosion", path = "assets/ASTEROIDS/textures/explosion.png", pixel_art = true}
    },

    music = 
    {
        {name = "music1", path = "assets/ASTEROIDS/sounds/music/rhythm_factory.wav"}
    }
}


function LoadAssets()
    for k, v in pairs(AssetDefs.textures) do
        if not AssetManager.add_texture(v.name, v.path, v.pixel_art) then
            print("Failed to load texture [ " ..v.name .. " ] at path [ " ..v.path .." ]" )
        else
            print("Loaded texture [ " ..v.name .." ]")
        end
    end

    for k, v in pairs(AssetDefs.music) do
        if not AssetManager.add_music(v.name, v.path) then
            print("Failed to load Music [" ..v.name .."] at path [" ..v.path .."]")
        else
            print("Loaded Music [" ..v.name .."]")
        end
    end
end
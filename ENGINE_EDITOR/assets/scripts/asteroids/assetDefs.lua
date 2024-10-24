AssetDefs =
{
    textures = 
    {
        { name = "ship", path = "assets/textures/ship_red.png", pixel_art = true},
        { name = "ast_1", path = "assets/textures/ast_1.png", pixel_art = true},
        { name = "ast_2", path = "assets/textures/ast_2.png", pixel_art = true},
        { name = "bg", path = "assets/textures/darkPurple.png", pixel_art = true},
        { name = "proj_1", path = "assets/textures/laserGreen.png", pixel_art = true}
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

    -- load other assets
end
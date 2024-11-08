ActiveCharacters = {}
function AddActiveCharacter(entity_id, character)
	ActiveCharacters[entity_id] = character
end

function GetActiveCharacter(entity_id)
	assert(ActiveCharacters[entity_id], string.format("Character with ID [%d] does not exist.", entity_id))
	return ActiveCharacters[entity_id]
end

-- TODO: fix this to actually destroy the underlying entity
function ClearCharacters()
	for k, v in pairs(ActiveCharacters) do 
		ActiveCharacters[k] = nil 
	end 
end

function UpdateActiveCharacters(dt)
	for _, v in pairs(ActiveCharacters) do 
		v.m_Controller:update(dt)
	end 
end 
---------------------------------------------------------------
Projectiles = {}
function AddProjectile(projectile)
	Projectiles[projectile.m_EntityID] = projectile
end

function UpdateProjectiles(dt)
	for k, v in pairs(Projectiles) do 
		if v:TimesUp() then 
			v:Destroy()
			Projectiles[k] = nil
		else 
			v:Update(dt)
		end
	end
end

function ResetProjectiles()
	for k, v in pairs(Projectiles) do 
		v:Destroy()
		Projectiles[k] = nil
	end
end
---------------------------------------------------------------

function LoadEntity(def)
    assert(def, "Def does not exist")

    local tag = ""
    if def.tag then -- if its not nil
        tag = def.tag
    end

    local group = ""
    if def.group then
        group = def.group
    end

    local newEntity = Entity(tag, group)

    if def.components.transform then
        newEntity:add_component(
            Transform(
                def.components.transform.position,
                def.components.transform.scale,
                def.components.transform.rotation
            )
        )
    end
    if def.components.sprite then
        local sprite = newEntity:add_component(
            Sprite(
                def.components.sprite.texture,
                def.components.sprite.width,
                def.components.sprite.height,
                def.components.sprite.start_x,
                def.components.sprite.start_y,
                def.components.sprite.layer
            )
        )
        sprite:generate_uvs()
        sprite.bHidden = def.components.sprite.bHidden or false
    end
    if def.components.circle_collider then
        newEntity:add_component(
            CircleCollider(
                def.components.circle_collider.radius,
                def.components.circle_collider.offset
            )
        )
    end
    if def.components.animation then
        newEntity:add_component(
            Animation(
                def.components.animation.num_frames,
                def.components.animation.frame_rate,
                def.components.animation.frame_offset,
                def.components.animation.bVertical,
                def.components.animation.bLooped
            )
        )
    end
    if def.components.physics_attributes then 
		local physAttr = def.components.physics_attributes 
		local newPhysicsAttr = PhysicsAttributes()
		newPhysicsAttr.eType = physAttr.type 
		newPhysicsAttr.density = physAttr.density
		newPhysicsAttr.friction = physAttr.friction
		newPhysicsAttr.restitution = physAttr.restitution
		newPhysicsAttr.position = physAttr.position
		newPhysicsAttr.radius = physAttr.radius
		newPhysicsAttr.bCircle = physAttr.bCircle or false
		newPhysicsAttr.bFixedRotation = physAttr.bFixedRotation or false
		newPhysicsAttr.bIsSensor = physAttr.bIsSensor or false

		newPhysicsAttr.objectData = ObjectData(
			physAttr.object_data.tag,
			physAttr.object_data.group,
			physAttr.object_data.bCollider or false,
			physAttr.object_data.bTrigger or false,
            physAttr.object_data.bIsFriendly or false,
			newEntity:id()
		)
		
		newEntity:add_component(PhysicsComp(newPhysicsAttr))
	end

    return newEntity:id()
end


function GetRandomVelocity(min_speed, max_speed)
    return vec2(
        math.random(min_speed, max_speed),
        math.random(min_speed, max_speed)
    )
end

function GetRandomPosition()
    return vec2(
        math.random(WINDOW_WIDTH) + WINDOW_WIDTH,
        math.random(WINDOW_HEIGHT) + WINDOW_HEIGHT
    )
end

-- get Value at that Digit
function GetDigit(num, digit)
    local n = 10 ^ digit
    local n1 = 10 ^ (digit - 1)
	return math.floor((num % n) / n1)
end

-------------------------------------------------------------------------------------------------------------
Tileset = {}
Tileset.__index = Tileset
function Tileset:Create(params)
    local this = 
    {
        name = params.name,
        columns = params.columns,
        width = params.width,
        height = params.height,
        tilewidth = params.tilewidth,
        tileheight = params.tileheight,
        firstgid =  params.firstgid
    }

    this.rows = params.height / params.tileheight
    this.lastgid = math.floor(((this.rows * this.columns) + this.firstgid) -1)

    setmetatable(this, self)
    return this
end


function Tileset:TileIdExists(id)
    return id >= self.firstgid and id <= self.lastgid
end


function Tileset:GetTileStartXY(id)
    assert(self:TileIdExists(id), "Tile ID [" ..id .."] does not exists in tileset [" ..self.name .."]")
    local actualTileID = id - self.firstgid
    local start_y = math.floor(actualTileID / self.columns)
    local start_x = math.floor(actualTileID % self.columns)
    return start_x, start_y
end


function GetTileset(tilesets, id)
    assert(tilesets)
    for k, v in pairs(tilesets) do
        if v:TileIdExists(id) then
            return v
        end
    end
    return nil
end


function LoadMap(mapDef)
    assert(mapDef)
    local tilesets = {}
    for k, v in pairs(mapDef.tilesets) do
        local newTileset = Tileset:Create(
            {
                name = v.name,
                columns = v.columns,
                width = v.imagewidth,
                height = v.imageheight,
                tilewidth = v.tilewidth,
                tileheight = v.tileheight,
                firstgid = v.firstgid
            }
        )
        table.insert(tilesets, newTileset)
    end

    -- loop through the Layers and create tiles
    for k, v in pairs(mapDef.layers) do
        local rows = v.height - 1
        local cols = v.width 
        local layer = k - 1

        for row = 0, rows do
            for col = 1, cols do
                -- id = (row * cols) + col
                local id = v.data[row * cols + col]
                if id == 0 then
                    goto continue
                end
                local tileset = GetTileset(tilesets, id)
                assert(tileset, "Tileset does not exists with id: " ..id)
                local start_x, start_y = tileset:GetTileStartXY(id)

                local scale = 1
                local position = vec2(((col -1) * tileset.tilewidth * scale), (row * tileset.tileheight * scale))

                local tile = Entity("", "tiles")
                local transform = tile:add_component(Transform(position, vec2(scale, scale), 0))

                local bIsCollider = false

                if tileset.name == "collider"  or tileset.name == "trigger" or tileset.name == "ground" then
                    local width = tileset.tilewidth / scale
					local height = tileset.tileheight / scale
                    tile:add_component(
                        BoxCollider(
                            math.floor(width),
							math.floor(height),
                            vec2(0, 0)
                        )
                    )
                    if IsPhysicsEnabled() then
                        local physicsAttribs = PhysicsAttributes()
                        physicsAttribs.eType = BodyType.Static
                        physicsAttribs.density = 1000.0 -- 1000kg/m^2
                        physicsAttribs.friction = 0.0
                        physicsAttribs.restitution = 0.0
                        physicsAttribs.gravityScale = 0.0
                        physicsAttribs.position = transform.position
                        physicsAttribs.bFixedRotation = false
                        physicsAttribs.boxSize = vec2(tileset.tilewidth, tileset.tileheight)
                        physicsAttribs.bBoxShape = true

                        if tileset.name == "trigger" then
                            physicsAttribs.bIsSensor = true
                            physicsAttribs.objectData = ObjectData("", "hole_triggers", false, true, false, tile:id())
                        elseif tileset.name == "ground" then
                            physicsAttribs.bIsSensor = true
                            physicsAttribs.objectData = ObjectData("", "ground", true, true, true, tile:id())
                        end
                        tile:add_component(PhysicsComp(physicsAttribs))
                    end
                    bIsCollider = true
                end

                if not bIsCollider then
                    local sprite = tile:add_component(
                        Sprite(
                            tileset.name,
                            tileset.tilewidth,
                            tileset.tileheight,
                            start_x, start_y,
                            layer
                        )
                    )
                    sprite:generate_uvs()
                end
                
                ::continue::
            end
        end
    end

end


function LoadAssets(asset)
    for k, v in pairs(asset.textures) do
        local bTileset = false
        if v.bTileset then
            bTileset = v.bTileset
        end

        if not AssetManager.add_texture(v.name, v.path, v.pixel_art, bTileset) then
            ENGINE_error("Failed to load texture [%s] at path [%s]",v.name, v.path)
            --print("Failed to load texture [ " ..v.name .. " ] at path [ " ..v.path .." ]" )
        else
            ENGINE_log("Loaded Texture [%s]", v.name)
            --print("Loaded texture [ " ..v.name .." ]")
        end
    end

    for k, v in pairs(asset.music) do
        if not AssetManager.add_music(v.name, v.path) then
            ENGINE_error("Failed to load music [%s] at path [%s]",v.name, v.path)
            --print("Failed to load Music [" ..v.name .."] at path [" ..v.path .."]")
        else
            ENGINE_log("Loaded Music [%s]", v.name)
            --print("Loaded Music [" ..v.name .."]")
        end
    end

    for k, v in pairs(asset.sound_fx) do
        if not AssetManager.add_soundfx(v.name, v.path) then
            ENGINE_error("Failed to load soundfx [%s] at path [%s]",v.name, v.path)
            --print("Failed to load soundFX [" ..v.name .."] at path [" ..v.path .."]")
        else
            ENGINE_log("Loaded soundfx [%s]", v.name)
            --print("Loaded soundFX [" ..v.name .."]")
        end
    end 
end


function Debug()
    if Keyboard.just_pressed(KEY_B) then
        if IsRenderCollidersEnabled() then
            DisableRenderColliders()
        else
            EnableRenderColliders()
        end
    end
end

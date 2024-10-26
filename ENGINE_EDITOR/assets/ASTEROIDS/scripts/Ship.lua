Ship = {}
Ship.__index = Ship

function Ship:Create(def)
    local this = 
    {
        m_EntityID = def.id,
        m_ForwardSpeed = def.fwd_speed or 10,
        m_AngularSpeed = def.ang_speed or 4,
        m_DriftSpeed = def.drift_speed or 1,
        m_DriftAngle = vec2(
            math.cos(0),
            math.sin(0)
        ),
        m_CoolDown = def.cool_down or 0, -- time between shot
        
        m_CoolDownTimer = Timer(),
        m_DeathTimer = Timer(),
        m_InvincibleTimer = Timer(),

        m_bExplode = false,
        m_bDead = false,
        m_NumLives = gData:NumLives(),
    }

    setmetatable(this, self)
    return this
end


function Ship:UpdateShip()
    if self.m_bDead then
        return
    end

    if self.m_bExplode then
        self:UpdateExplosion()
        return -- not move
    end

    local ship = Entity(self.m_EntityID)
    local transform = ship:get_component(Transform)
    local sprite = ship:get_component(Sprite)

    local forward = vec2(
        math.cos(math.rad(transform.rotation)), -- X value
        math.sin(math.rad(transform.rotation))  -- Y value
    )

    if Keyboard.pressed(KEY_A) then
        transform.rotation = transform.rotation - self.m_AngularSpeed
    elseif Keyboard.pressed(KEY_D) then
        transform.rotation = transform.rotation + self.m_AngularSpeed
    end

    if Keyboard.pressed(KEY_W) then
        transform.position = transform.position + forward * self.m_ForwardSpeed
        self.m_DriftAngle = forward
    else
        transform.position = transform.position + self.m_DriftAngle * self.m_DriftSpeed
    end

    if not self.m_CoolDownTimer:is_running() then
        if Keyboard.just_pressed(KEY_SPACE) then
            local projectile = Projectile:Create(
                {
                    def = "proj_1",
                    dir = forward,
                    start_pos = vec2(
                        transform.position.x + sprite.width/2,
                        transform.position.y + sprite.height/2
                    ),
                    rotation = transform.rotation
                }
            )
            self.m_CoolDownTimer:start()
            AddProjectile(projectile)
            Sound.play("laser")
        end
    elseif self.m_CoolDownTimer:elapsed_ms() >= self.m_CoolDown then
        self.m_CoolDownTimer:stop()
    end

    self:CheckDeath()

    CheckPos(transform.position, sprite.width, sprite.height)
end


function Ship:CheckDeath()
    local numLives = gData:NumLives()

    if self.m_NumLives ~= numLives and not self.m_bExplode then
        self.m_bExplode = true
        local ship = Entity(self.m_EntityID)
        -- set Animation
        local animation = ship:get_component(Animation)
        animation.num_frames = 12
        animation.frame_rate = 12
        animation:reset()
        -- set Sprite
        local sprite = ship:get_component(Sprite)
        sprite.texture_name = "ship_explosion"
        sprite.width = 96
        sprite.height = 96
        sprite:generate_uvs()
        -- set Transform -- set Scale
        local transform = ship:get_component(Transform)
        transform.scale = vec2(2, 2)

        self.m_NumLives = numLives
        self.m_DeathTimer:start()
        Sound.play("ship_explosion")
    end

    if self.m_InvincibleTimer:is_running() then
        local ship = Entity(self.m_EntityID)
        local sprite = ship:get_component(Sprite)
        sprite.color.a = 150

        if self.m_InvincibleTimer:elapsed_ms() > 3000 then
            -- reset Collider
            local collider = ship:get_component(CircleCollider)
            collider.bColliding = false
            self.m_InvincibleTimer:stop()
            sprite.color.a = 255
        end
    end
end


function Ship:UpdateExplosion()
	if self.m_bExplode and self.m_DeathTimer:elapsed_ms() > 1000 then 
		local ship = Entity(self.m_EntityID)
		local animation = ship:get_component(Animation)

		if animation.current_frame >= animation.num_frames - 1 then
			-- Reset Animations
			animation.num_frames = 0
			animation.frame_rate = 0
			animation.current_frame = 0
			-- Reset the sprite
			local sprite = ship:get_component(Sprite)
			sprite.texture_name = "ship"
			sprite.width = 84
			sprite.height = 103 
			sprite:generate_uvs()
			sprite.start_x = 0
			-- Reset the scale -- transform
			local transform = ship:get_component(Transform)
			transform.scale = vec2(1, 1)
			-- Reset Explosion
			self.m_bExplode = false 
			self.m_DeathTimer:stop()
			self.m_InvincibleTimer:start()

			if gData:NumLives() == 0.0 then 
				if not self.m_bDead then 
					sprite.bHidden = true
				end
				self.m_bDead = true
			end
		end
	end
end


function Ship:Reset()
	self.m_bExplode = false 
	self.m_bDead = false 
	self.m_NumLives = gData:NumLives()
	self.m_DeathTimer:stop()
	self.m_InvincibleTimer:stop()
	self.m_CoolDownTimer:stop()

	-- Reset the Sprite and Box collider 
	local ship = Entity(self.m_EntityID)
	local sprite = ship:get_component(Sprite)
	sprite.bHidden = false 

	local circle_collider = ship:get_component(CircleCollider)
	circle_collider.bColliding = false
end
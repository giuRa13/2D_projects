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
        m_CoolDownTimer = Timer()
    }

    setmetatable(this, self)
    return this
end


function Ship:UpdateShip()
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
        end
    elseif self.m_CoolDownTimer:elapsed_ms() >= self.m_CoolDown then
        self.m_CoolDownTimer:stop()
    end

    CheckPos(transform.position, sprite.width, sprite.height)
end
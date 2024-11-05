GameState = {}
GameState.__index = GameState 

function GameState:Create(stack)
	local this = 
	{
		m_Stack = stack,
		m_bLeft = false,
		--m_SceneRain = SceneRain:Create(nil)
	}
	
	local state = State("game state")
	state:set_variable_table(this)
	state:set_on_enter( function() this:OnEnter() end )
	state:set_on_exit( function() this:OnExit() end )
	state:set_on_update( function(dt) this:OnUpdate(dt) end )
	state:set_on_render( function() this:OnRender() end )
	state:set_handle_inputs( function() this:HandleInputs() end )

	setmetatable(this, self)
	this:Initialize()
	return state
end


function GameState:Initialize()
	-- Create events
	if not gCollisionEvent then 
		gCollisionEvent = CollisionEvent:Create()
	end 
	if not gTriggerSystem then 
		gTriggerSystem = TriggerSystem:Create()
	end 
	gCollisionEvent:SubscribeToEvent(gTriggerSystem)

	-- Create the player
	if not gPlayer then 
		local character = Character:Create({name = "robot"})
		gPlayer = Entity(character.m_EntityID)
		AddActiveCharacter(gPlayer:id(), character)
		--[[gPlayer = Entity("player", "")
		local playerTransform = gPlayer:add_component(Transform(vec2(16, 368), vec2(1, 1), 0))
		local sprite = gPlayer:add_component(Sprite("player", 32, 32, 0, 0, 2))
		sprite:generate_uvs()
		gPlayer:add_component(Animation(4, 9, 0, false, true))
		local circleCollider = gPlayer:add_component(CircleCollider(8, vec2(8, 12)))
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
		-- Add Physics component to the player 
		gPlayer:add_component(PhysicsComp(playerPhysAttr))]]--
	end
	if not gFollowCam then 
		gFollowCam = FollowCamera(
			FollowCamParams(
			{
				min_x = 0,
				min_y = 0,
				max_x = 640,
				max_y = 480,
				scale = 2
			}
			), gPlayer
		)
	end
end


function GameState:OnEnter()
	LoadMap(CreateTestPlatformerMap())
	Sound.play("rain", -1, 1)
	Sound.set_volume(1, 20)
	Music.play("music1", -1)
	Music.set_volume(20)
end

function GameState:OnExit()
	-- TODO: Destroy all entities
end

function GameState:OnUpdate(dt)
	self:UpdateContacts()
	UpdateActiveCharacters(dt)
	gFollowCam:update()
	--self.m_SceneRain:UpdateRainGen(dt)
end

function GameState:OnRender()
end

function GameState:HandleInputs()
	if Keyboard.just_released(KEY_BACKSPACE) then 
		self.m_Stack:pop()
		return 
	end
end

function GameState:UpdateContacts()
	local uda, udb = ContactListener.get_user_data()
	if uda and udb then 
		gCollisionEvent:EmitEvent(uda, udb)
	end 
end





SceneRain = {} 
SceneRain.__index = SceneRain
function SceneRain:Create(params)
	params = params or {}
	local this = 
	{
		m_RainGenerator = params.rainGenerator or RainGenerator:Create(nil),
		m_Timer = Timer()
	}
	this.m_OnUpdate = params.onUpdate or function() this:Update() end 
	this.m_Timer:start() 

	setmetatable(this, self)
	return this 
end

function SceneRain:Update()
end

function SceneRain:UpdateRainGen(dt)
	if self.m_RainGenerator then 
		self.m_RainGenerator:Update(dt)
	end
end
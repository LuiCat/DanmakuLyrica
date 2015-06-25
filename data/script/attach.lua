Attach = Attach or {}

-- functions for attached bullet
-- function Attach.new()
--   return a new attach list
-- function Attach:attach(id)
--   attach a bullet
-- function Attach:clear()
--   detach all bullets
-- function Attach:bullet(x,y,sp,rt,t)
--   push a bullet and attach it
-- function Attach:setPos(x,y)
--   set properties of all attached bullet, the same below
-- function Attach:setRot(angle)
-- function Attach:setSpeed(v)
-- function Attach:setAcc(a)
-- function Attach:setRS(omega)
-- function Attach:offsetPos(deltax,deltay)
-- function Attach:offsetRot(deltaangle)
-- function Attach:offsetSpeed(deltav)

function Attach.new()
	local t = {}
	Attach.__index = Attach
	setmetatable(t,Attach)
	t.list=Attach.alloc();
	return t
end

function Attach:bullet(x,y,sp,rt,t)
	self:attach(bullet(x,y,sp,rt,t))
end

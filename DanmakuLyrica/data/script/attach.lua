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
-- function Attach:setPos(x,y[,delay])
--   set properties of all attached bullet, the same below
--   specify delay to schedule a delayed property changing
-- function Attach:setRot(angle[,delay])
-- function Attach:setSpeed(v[,delay])
-- function Attach:setAcc(a[,delay])
-- function Attach:setRS(omega[,delay])
-- function Attach:offsetPos(deltax,deltay[,delay])
-- function Attach:offsetRot(deltaangle[,delay])
-- function Attach:offsetSpeed(deltav[,delay])

function Attach.new()
	local t = {}
	Attach.__index = Attach
	setmetatable(t,Attach)
	t.list=Attach.alloc();
	return t
end

function Attach:bullet(x,y,sp,rt,t)
	local id = bullet(x,y,sp,rt,t)
	self:attach(id)
	return id
end

function Attach:spirit(x,y,sp,rt,life,hp)
	local id = spirit(x,y,sp,rt,life,hp)
	self:attach(id)
	return id
end

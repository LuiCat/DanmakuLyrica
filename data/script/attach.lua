
Attach = Attach or {}



function Attach.new()
	local t = {}
	Attach.__index = Attach
	setmetatable(t,Attach)
	t.list=Attach.alloc();
	return t
end


function Attach:bullet(x,y,sp,rt,t)
	self:attach(bullet(x,y,sp,rt,t))
	--print('yeah'..self.times)
end


function task(func)
	local co=coroutine.create(func)
	return pushTask(co)
end

-- function bullet(x,y,sp,rt[,type])
bullet = pushBullet

-- function now()
now = getTime

-- function registerBullet(name, pic, w, h, centerx, centery, facetype={'none','forward','spin'},
--                         judgetype={'round','rect','oval'}, judgew, judgeh)

-- function setCenter(x,y)
center = setCenter

-- function setAngle(rt)
angle = setAngle

-- function setBulletType(typeid|typename)
bulletType = setBulletType

function sleep(t)
	delay(t)
	coroutine.yield()
end

function sleepUntil(t)
	delayUntil(t)
	coroutine.yield()
end

function seq(str,t,...)
	local i=0
	local list=table.pack(...)
	while i<string.len(str) do
		i=i+1
		c=string.byte(str,i)-string.byte('0')
		if c>0 and c<=#list then
			list[c]()
		end
		sleep(t)
	end
end

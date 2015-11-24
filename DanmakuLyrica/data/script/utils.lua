
function task(func)
	local co=coroutine.create(func)
	return pushTask(co)
end

-- function bullet(x,y,sp,rt[,type])
bullet = pushBullet

-- function now()
now = getTime

-- function loadSound(filename[, (string)name][, (number)volume])
-- function loadStream(filename[, (string)name][, (number)volume])

-- function playSound([{name|soundid}])
-- function playSoundPitch([{name|soundid},] pitch)
sound = playSound
soundPitch = playSoundPitch

-- function registerBullet(name, pic, w, h, centerx, centery, facetype={'none','forward','spin'},
--                         judgetype={'round','rect','oval'}, judgew, judgeh)

-- function setCenter(x,y)
center = setCenter

-- function setAngle(rt)
angle = setAngle

-- function setBulletType(typeid|typename)
bulletType = setBulletType

-- function setSound([{name|soundid}])

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

function playScore(sound, centerPitch, delay, ...)
	local list = table.pack(...)
	local dly = delay
	local i,j,c,tp,obj,s,p
	for i = 1,#list do
		obj = list[i]
		tp = type(list[i])
		if tp=='string' then
			local oct,semi = 0,0
			for j = 1,#obj do
				c = obj:sub(j,j);
				if c == '+' then
					oct = oct+1
				elseif c == '-' then
					oct = oct-1
				elseif c == '#' then
					semi = semi+1
				elseif c == 'b' then
					semi = semi-1
				elseif c == '.' then
					sleep(dly)
				else
					s = string.find('1234567', c)
					if s ~= nil then
						p = centerPitch*2^(oct+(math.floor((s-3)*1.8+4.1)+semi)/12)
						--print(p)
						playSoundPitch(sound, p)
						semi = 0
						sleep(dly)
					end
				end
			end
		elseif tp=='number' then
			dly = obj
		end
	end
end



function task(func)
	local co=coroutine.create(func)
	pushTask(co)
end

now = getTime;
-- function now()

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

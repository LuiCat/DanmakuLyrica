
function task(func)
	local co=coroutine.create(func)
	pushTask(co)
end

function sleep(tick)
	delaySecs(tick)
	coroutine.yield()
end

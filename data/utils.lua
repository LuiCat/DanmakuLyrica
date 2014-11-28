

function newTask(func)
	local co=coroutine.create(func)
	registerTask(co)
end

function resumeTask(co)
	if coroutine.status(co)=='dead' then
		return false
	end
	coroutine.resume(co)
	return true
end

function sleep(tick)
	delayTicks(tick)
	coroutine.yield()
end

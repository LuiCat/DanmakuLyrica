
dofile('data\\utils.lua')

function main()
	local x=0;
	sleepUntil(0)
	while true do
		x=x+1
		seq('1212',1,test,function() print(now()); end)
		--test()
		--sleep(1)
	end
end

task(main);

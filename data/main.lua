
dofile('data\\utils.lua')

function main()
	local x=0;
	while true do
		print(x)
		x=x+1
		sleep(1)
	end
end

newTask(main);

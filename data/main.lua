
dofile('data\\utils.lua')

function main()
	local x=0;
	sleepUntil(0)
	while true do
		x=x+1
		seq('1010',1,test)
		--test()
		--sleep(1)
	end
end

function main2()
	local x=0;
	sleepUntil(0)
	while true do
		x=x+1
		seq('010010',2/3,test)
		--test()
		--sleep(1)
	end
end

task(main);
task(main2);


function main()
	local x=0
	local t=0
	setCenter(0,-80)
	setBulletType('PinkRice')
	sleepUntil(0)
	while true do
		x=x+2
		t=0;
		seq('1111',0.125,
		function()
			bullet(0,0,12+t*0.9,x*5.3)
			bullet(0,0,12+t*0.9,180+x*5.3)
			x=x+1
			t=t+1
		end)
	end
end

function main3()
	local i
	local c=0
	local di=67
	setCenter(0,-80)
	setBulletType('BlueRing')
	sleepUntil(0)
	while true do
		task(function()
			att=Attach.new()
			for i=0,36 do
				att:bullet(0,0,75.0+math.random()*30.0,math.random()*360)
			end
			att:setAcc(-60.0)
			att:setRS(3.0)
			sleep(1)
			att:setAcc(0.0)
		end)
		sleep(2)
		task(function()
			att=Attach.new()
			for i=0,36 do
				att:bullet(0,0,75.0+math.random()*30.0,math.random()*360)
			end
			att:setAcc(-60.0)
			att:setRS(-3.0)
			sleep(1)
			att:setAcc(0.0)
		end)
		sleep(2)
	end
end

task(main);
task(main3);

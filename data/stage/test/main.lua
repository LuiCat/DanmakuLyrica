
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
	att1=Attach.new()
	while true do
		for i=0,36 do
			att1:bullet(0,0,50.0,i*10)
		end
		for i=0,12 do
			att1:bullet(0,0,10.0,i*30)
		end
		att1:setRS(5.0)
		att1:clear()
		sleep(2)
		for i=0,36 do
			att1:bullet(0,0,50.0,i*10)
		end
		for i=0,12 do
			att1:bullet(0,0,10.0,i*30)
		end
		att1:setRS(-5.0)
		att1:clear()
		sleep(2)
	end
end

task(main);
task(main3);

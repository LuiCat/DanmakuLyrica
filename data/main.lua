registerBullet('BlueRing', 'etama1.png', 16, 16, 8, 8, 'None', 'Round', 4.0, 4.0);
registerBullet('PinkRice', 'etama2.png', 16, 16, 8, 8, 'Forward', 'Round', 4.0, 4.0);

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
		--test()
		--sleep(1)
	end
end

function main2()
	local i
	setCenter(0,-80)
	setBulletType('BlueRing')
	sleepUntil(0)
	while true do
		for i=0,71 do
			bullet(0,0,43,i*5)
		end
		sleep(2)
	end
end

task(main);
task(main2);

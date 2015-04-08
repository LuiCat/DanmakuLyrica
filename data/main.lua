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
	local c=0
	local di=67
	setCenter(0,-80)
	setBulletType('BlueRing')
	sleepUntil(0)
	local tf=function()
		sleep(8)
		att.setRS(di)
		att.setAcc(-10)
		sleep(1)
		att.setAcc(16)
		sleep(1)
		att.setRS(0)
		att.setAcc(-6)
		sleep(2)
		att.setAcc(0)
	end
	local t=task(tf)
	while true do
		for i=0,47 do
			attach(t,bullet(0,0,27,i*7.5))
		end
		sleep(2)
		c=c+1
		if c>=4 then
			c=0
			di=-di
			t=task(tf)
		end
	end
end

print(#att)
for i=1,#att do
	print(att[i])
end

task(main);
task(main2);

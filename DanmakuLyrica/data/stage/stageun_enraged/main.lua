
function task_1()
	for t = 1,4 do
		setAngle(playerAngle());
		for i=-2,2 do
			spirit(0,0,35+5*t,45*i,16,2);
		end
		local cl=function(c)
			return function()
				local dd=360/c;
				splash();
				setAngle(math.random()*360);
				setCenter((math.random()*40-20)*t,(math.random()*10-5)*t)
				if c > 60 then
					setBulletType('sharp0');
				else
					setBulletType('sharp1');
				end
				local att = Attach.new();
				for i = 0,c-1 do
					att:bullet(0, 0, 178+t*5, i*dd);
					att:bullet(0, 0, 170+t*5, i*dd);
				end
				att:setAcc(-150);
				att:setAcc(0,1);
			end
		end
		seq('2001001001001010', 0.5, cl(45), cl(120));
	end
end

function task_2()
	local id;
	sleepUntil(32);
	setBulletType('sharp0');
	for t = 1,4 do
		setAngle(playerAngle());
		for i=-2,1 do
			local id=spirit(0,0,30,45*i+22.5,16,3);
			task(function()
				setCenter(id);
				seq('101101011101011', 0.5, function()
					setAngle(math.random()*360);
					local att = Attach.new();
					for j = 0,2 do
						att:bullet(0, 0, 170-math.abs(j-1)*5, j*3);
						att:bullet(0, 0, 170-math.abs(j-1)*5, 120+j*3);
						att:bullet(0, 0, 170-math.abs(j-1)*5, 240+j*3);
					end
					att:setAcc(-120);
					att:setAcc(0,1);
				end);
			end);
		end
		sleep(4);
		moveBoss(math.random()*120-60,math.random()*20-150, 4);
		sleep(4);
	end
end

circle1 = function()
	splash();
	setBulletType('round1');
	setAngle(math.random()*360);
	for i = 0,19 do
		bullet(0, 0, 40, i*18);
		bullet(0, 0, 50, i*18);
		bullet(0, 0, 60, i*18);
	end
end

circle2 = function()
	splash();
	setBulletType('round0');
	setAngle(0);
	for i = 0,11 do
		for j = 0,10 do
			bullet(0, 0, 20+j*5, i*30);
		end
	end
end

circle3 = function()
	splash();
	setBulletType('round2');
	setAngle(playerAngle());
	for i = -3,3 do
		for j = 0,3 do
			bullet(0, 0, 40+j*7, i*15);
		end
	end
end

function task_3()
	local d = 7;
	sleepUntil(32*2);
	task(function()
		seq('102030121031', 1, circle1, circle2, circle3);
		seq('2003002010002000', 0.25, circle1, circle2, circle3);
		seq('100020103001203010003010', 0.5, circle1, circle2, circle3);
		seq('2003002010002000', 0.25, circle1, circle2, circle3);
	end);
	for i = 0,6 do
		sleep(2);
		local atts = Attach.new()
		setAngle(math.random()*360)
		for i=0,4 do
			atts:spirit(0,0,5,72*i,16,1);
		end
		atts:setAcc(3.0);
		atts:setRS(d);
		d = -d;
		moveBoss(math.random()*200-100,math.random()*20-150, 4);
		sleep(2);
	end
end

function task_4()
	sleepUntil(32*3-1);
	for i = 1,7 do
		moveBoss((i-4)*80,math.random()*50-170, 0.5);
		sleep(1);
		local id = spirit(0, 0, 0, 0, 32-i, 1);
		local t=i;
		task(function()
			setCenter(id);
			setBulletType('sharp2');
			sleep(8);
			local att = Attach.new();
			for j = 0,23 do
				att:bullet(0, 0, 160, j*15);
			end
			att:setAcc(-160);
			local att2 = Attach.new();
			for j = 0,23 do
				att2:bullet(0, 0, 160, j*15);
			end
			att2:setAcc(-160);
			sleep(1);
			att:setAcc(0);
			att:setSpeed(40);
			att:offsetRot(60);
			att2:setAcc(0);
			att2:setSpeed(40);
			att2:offsetRot(-60);
			sleepUntil(112);
			setBulletType('dot0');
			local cl = function(x)
				local res = function()
					if x == t then
						splash();
						local att = Attach.new();
						setAngle(playerAngle());
						for j = 0,35 do
							att:bullet(0, 0, 150, j*10);
							att:bullet(0, 0, 180, j*10);
						end
						att:setAcc(-150);
						att:setAcc(0,1);
						att:setAcc(30,2);
						att:setAcc(0,4);
					end
				end
				return res;
			end
			seq('4000600050170203',0.5,cl(1),cl(2),cl(3),cl(4),cl(5),cl(6),cl(7));
			seq('3004600215',0.5,cl(1),cl(2),cl(3),cl(4),cl(5),cl(6),cl(7));
			seq('73164020',0.125,cl(1),cl(2),cl(3),cl(4),cl(5),cl(6),cl(7));
			seq('175',0.5,cl(1),cl(2),cl(3),cl(4),cl(5),cl(6),cl(7));
		end);
	end
	moveBoss(0,-150, 2);
	sleepUntil(112);
	seq('4000500030260107',0.5,splash);
	seq('3002600413',0.5,splash);
	seq('75164020',0.125,splash);
	seq('135',0.5,splash);
end

function task_5()
	sleepUntil(32*4-2);
	task(function()
		setBulletType('scale2');
		sleepUntil(32*4);
		local t,x=0,0;
		for i = 0,31 do
			t=0;
			seq('1111',0.125,
			function()
				bullet(0,0,24+t*1.9,x*6.99)
				bullet(0,0,24+t*1.9,90+x*6.99)
				bullet(0,0,24+t*1.9,180+x*6.99)
				bullet(0,0,24+t*1.9,270+x*6.99)
				x=x+1
				t=t+1
			end)
		end
		for i = 0,29 do
			t=0;
			seq('1111',0.125,
			function()
				bullet(0,0,24+t*1.9,x*6.99)
				bullet(0,0,24+t*1.9,90+x*6.99)
				bullet(0,0,24+t*1.9,180+x*6.99)
				bullet(0,0,24+t*1.9,270+x*6.99)
				x=x-1
				t=t+1
			end)
		end
	end)
	for i = 0,3 do
		task(function()
			local att = Attach.new();
			local t=i;
			local id = att:spirit(0, 0, 120, 60*(t%2*2-1), 12, 6);
			setCenter(id);
			att:setAcc(-60);
			sleep(2);
			att:offsetRot(85*(1-t%2*2))
			att:setAcc(0);
			att:setSpeed(20);
			att:setRS(5*(1-t%2*2));
			for k = 0,7 do
				setAngle(playerAngle()+math.random()*10-5);
				setBulletType('dot1');
				att = Attach.new();
				for j = -4,4 do
					att:bullet(0, 0, 97, j*15);
				end
				att:setAcc(-90);
				att:setAcc(0,1);
				att:setAcc(12,16-k*2);
				att:setAcc(0,20-k*2);
				setBulletType('sharp0');
				setAngle(playerAngle());
				att = Attach.new();
				for j = -2,2 do
					att:bullet(0, 0, 110, math.tan(j*0.5)*33);
					att:bullet(0, 0, 125, math.tan(j*0.5)*33);
					att:bullet(0, 0, 140, math.tan(j*0.5)*33);
				end
				att:setAcc(-80);
				att:setAcc(0,1);
				sleep(1);
			end
		end)
		sleep(8);
	end
	sleep(1);
	clearScene();
end

function task_6()
	setBulletType('sharp2');
	sleepUntil(32*5);
	for i = 0,2 do
		splash();
		task(function()
			setAngle(math.random()*360);
			local att = Attach.new();
			for j = 0,59 do
				att:bullet(0, 0, 180+i*15, j*6);
				att:bullet(0, 0, 190+i*15, (j+0.5)*6);
			end
			att:setAcc(-150);
			sleep(1);
			att:setAcc(0);
		end);
		sleep(2);
	end
	sleep(2);
	setBulletType('round1');
	local px,py=playerPos();
	local sa=function()
		cx,cy=getCenterPos();
		setAngle(math.atan2(px-cx,py-cy)/math.pi*180);
	end
	for i = 1,7 do
		if i > 3 then
			task(function()
				setAngle(playerAngle());
				setBulletType('sharp0');
				local att = Attach.new();
				local id=att:spirit(0,0,50,0,8,4);
				setCenter(id);
				att:setAcc(-2);
				seq('1010001010010010100100100010001', 0.25, function()
					setAngle(playerAngle()+180);
					att = Attach.new();
					for j = -26,26 do
						att:bullet(0, 0, 160, j*6);
					end
					att:setAcc(-160);
					att:setAcc(0,1);
					att:setSpeed(18,2);
				end);
			end);
		end
		task(function()
			seq('12121212',1,
			function()
				sa();
				local att = Attach.new();
				for j = 0,9 do
					att:bullet(0,0,220,j*36);
					att:bullet(0,0,230,j*36);
				end
				att:setAcc(-180);
				att:setAcc(0,1);
			end,
			function()
				sa();
				local att = Attach.new();
				for j = 0,9 do
					att:bullet(0,0,220,j*36+18);
					att:bullet(0,0,230,j*36+18);
				end
				att:setAcc(-180);
				att:setAcc(0,1);
			end);
		end);
		if i == 7 then
			break;
		end
		task(function()
			local att = Attach.new();
			local t=i;
			local id = att:spirit(0, 0, 240, 110*(t%2*2-1), 20, 10);
			setCenter(id);
			setBulletType('scale2');
			sleep(1);
			att:setAcc(-240);
			sleep(1);
			att:setRot(90*(1-t%2*2));
			att:setAcc(0);
			att:setSpeed(30);
			for k = 0,7 do
				setAngle(playerAngle()+math.random()*10-5);
				att = Attach.new();
				for j = -4,4 do
					att:bullet(0, 0, 125+math.random()*12, j*15);
					att:bullet(0, 0, 165+math.random()*12, j*15);
				end
				att:setAcc(-120);
				att:setAcc(0,1);
				sleep(2);
			end
		end)
		sleep(4);
		moveBoss(math.random()*120-60,math.random()*20-150, 4);
		sleep(0.5);
		px,py=playerPos();
		sleep(3.5);
	end
	sleep(4);
	moveBoss(0, -150, 4);
	sleep(3.5);
	clearScene();
end

function task_7()
	sleepUntil(32*7);
	setAngle(playerAngle());
	local att = Attach.new();
	for i = -1,1 do
		att:spirit(0,0,42,i*10,8,1);
	end
	att:setAcc(-5);
	sleep(3.5);
	setBulletType('round0');
	att = Attach.new();
	for i = -2,2 do
		for j = 0,4 do
			att:bullet(0,0,260+j*10,i*5);
		end
	end
	att:setAcc(-200);
	att:setAcc(0,1);
	sleep(1);
	setBulletType('round2');
	att = Attach.new();
	for i = -4.5,4.5 do
		for j = 0,2 do
			att:bullet(0,0,260+j*10,i*6);
		end
	end
	att:setAcc(-200);
	att:setAcc(0,1);
	sleep(1);
	setBulletType('round1');
	att = Attach.new();
	for i = 0,89 do
		att:bullet(0,0,360,i*4);
	end
	att:setAcc(-300);
	att:setAcc(0,1);
	sleep(2.5);
end

function task_8_spirit(hp)
	return function()
		local att = Attach.new();
		local id = att:spirit(0,0,0,0,18,hp);
		setCenter(id);
		task(function()
			for i = 0,199 do
				setBulletType('dot1');
				setAngle(playerAngle()+180);
				for j = -1,1 do
					bullet(0,0,7,j*120);
				end
				sleep(0.5/3);
			end
		end)
		task(function()
			for i = 0,19 do
				setBulletType('dot2');
				setAngle(playerAngle());
				for j = 0,8 do
					bullet(0,0,120,j*40);
				end
				sleep(1);
			end
		end)
		local x,y,cx,cy,s;
		for i = 0,1 do
			sleep(4);
			x,y=playerPos();
			cx,cy=centerPos();
			x,y=x-cx,y-cy;
			s=math.min(math.sqrt(x*x+y*y)*0.5,200);
			att:setSpeed(s);
			att:setRot(playerAngle());
			sleep(2);
			att:setSpeed(0);
			sleep(2);
		end
		sleep(4);
	end
end

function task_8()
	sleepUntil(32*7+8);
	task(task_8_spirit(10));
	moveBoss(0,-600,8);
	sleep(16);
	setCenter(0,0,200);
	task(task_8_spirit(10));
	sleep(16);
	setCenter(-360,200);
	task(task_8_spirit(4));
	sleep(4);
	setCenter(360,200);
	task(task_8_spirit(4));
	sleep(2);
	setCenter(0,-280);
	task(task_8_spirit(4));
	sleep(2);
	setCenter(-360,-260);
	task(task_8_spirit(4));
	sleep(2);
	setCenter(360,-260);
	task(task_8_spirit(4));
	sleep(2);
	setCenter(0,200);
	task(task_8_spirit(4));
end

function task_9()
	sleepUntil(32*9+5);
	moveBoss(0,-150,2);
	sleep(2);
	clearScene();
	splash();
	sleep(1);
	task(function()
		setBulletType('sharp2');
		seq('1000000010000000100010001010111',1,function()
			local x=math.random()*200-100;
			local y=math.random()*60-30;
			for j = 0,149 do
				bullet(x, y, 40, j*2.4);
				bullet(x, -300-y, 40, j*2.4);
				bullet(800-x, y, 40, j*2.4);
				bullet(-800-x, y, 40, j*2.4);
			end
		end);
	end);
	setBulletType('round0');
	local att;
	for i = 0,14 do
		if i%2 == 0 then
			setAngle(playerAngle());
			for j = 0,4 do
				spirit(0,0,50,j*72+36,16,2);
			end
		end
		att = Attach.new();
		for j = 0,14 do
			att:bullet(0,0,230,j*24+6);
			att:bullet(0,0,250,j*24+6);
			att:bullet(0,0,270,j*24+6);
		end
		att:setAcc(-180);
		sleep(1);
		att:setAcc(0);
		att = Attach.new();
		for j = 0,14 do
			att:bullet(0,0,230,j*24+18);
			att:bullet(0,0,250,j*24+18);
			att:bullet(0,0,270,j*24+18);
		end
		att:setAcc(-180);
		sleep(1);
		att:setAcc(0);
	end
	setBulletType('round1');
	setAngle(0);
	for i = 0,2 do
		splash();
		att = Attach.new();
		for j = 0,119 do
			att:bullet(0,0,300+20*i,j*3+i*1.5);
		end
		att:setAcc(-240);
		att:setAcc(0,1);
		sleep(2/3);
	end
end

function task_10()
	setBulletType('round0');
	sleepUntil(32*10+8);
	clearScene();
	for i = 0,2 do
		splash();
		local att = Attach.new();
		for j = 0,59 do
			att:bullet(0,0,240,j*6+i*3);
			att:bullet(0,0,250,j*6+i*3);
		end
		att:setAcc(-180);
		att:setAcc(0,1);
		sleep(2);
	end
	splash();
	moveBoss(math.random()*80-40,math.random()*20-150, 4);
	sleep(2);
	setBulletType('round1');
	local px,py=playerPos();
	local sa=function()
		cx,cy=getCenterPos();
		setAngle(math.atan2(px-cx,py-cy)/math.pi*180);
	end
	for i = 1,23 do
		if i > 3 then
			task(function()
				setAngle(playerAngle());
				setBulletType('sharp0');
				local att = Attach.new();
				local id=att:spirit(0,0,50,0,8,4);
				setCenter(id);
				att:setAcc(-2);
				seq('1010001010010010100100100010001', 0.25, function()
					setAngle(playerAngle()+180);
					att = Attach.new();
					for j = -26,26 do
						att:bullet(0, 0, 160, j*6);
					end
					att:setAcc(-120);
					att:setAcc(0,1);
					att:setSpeed(18,3);
				end);
			end);
		end
		task(function()
			seq('12121212',1,
			function()
				sa();
				local att = Attach.new();
				if i<16 then
					for j = 0,9 do
						att:bullet(0,0,220,j*36);
						att:bullet(0,0,230,j*36);
					end
				elseif i<20 then 
					setBulletType('round0');
					for j = 0,11 do
						att:bullet(0,0,220,j*30);
						att:bullet(0,0,230,j*30);
					end
				else
					setBulletType('round0');
					for j = 0,11 do
						att:bullet(0,0,216,j*30);
						att:bullet(0,0,223,j*30);
						att:bullet(0,0,230,j*30);
					end
				end
				att:setAcc(-180);
				att:setAcc(0,1);
			end,
			function()
				sa();
				local att = Attach.new();
				if i<16 then
					for j = 0,9 do
						att:bullet(0,0,220,j*36+18);
						att:bullet(0,0,230,j*36+18);
					end
				elseif i<20 then
					setBulletType('round0');
					for j = 0,11 do
						att:bullet(0,0,220,j*30+15);
						att:bullet(0,0,230,j*30+15);
					end
				else
					setBulletType('round0');
					for j = 0,11 do
						att:bullet(0,0,218,j*30+15);
						att:bullet(0,0,225,j*30+15);
						att:bullet(0,0,232,j*30+15);
					end
				end
				att:setAcc(-180);
				att:setAcc(0,1);
			end);
		end);
		task(function()
			local att = Attach.new();
			local t=i;
			local id = att:spirit(0, 0, 240, 110*(t%2*2-1), 20, 10);
			setCenter(id);
			setBulletType('scale2');
			sleep(1);
			att:setAcc(-240);
			sleep(1);
			att:setRot(90*(1-t%2*2));
			att:setAcc(0);
			att:setSpeed(30);
			for k = 0,7 do
				setAngle(playerAngle()+math.random()*10-5);
				att = Attach.new();
				for j = -4,4 do
					att:bullet(0, 0, 125+math.random()*12, j*15);
					att:bullet(0, 0, 165+math.random()*12, j*15);
				end
				att:setAcc(-120);
				att:setAcc(0,1);
				sleep(2);
			end
		end)
		sleep(4);
		moveBoss(math.random()*80-40,math.random()*20-150, 4);
		sleep(0.5);
		px,py=playerPos();
		sleep(3.5);
	end
	clearScene();
end

function task_11()
	setBulletType('sharp1');
	sleepUntil(32*16+13.5);
	for i = 0,3 do
		task(function()
			setBulletType('dot0');
			for j = 0,6 do
				task(function()
					setAngle(j*360/7+playerAngle());
					setCenter(spirit(0,0,45,180,16,2));
					for t = 0,7 do
						bullet(0,0,50-t*5,60);
						bullet(0,0,45-t*5,20);
						bullet(0,0,45-t*5,-20);
						bullet(0,0,50-t*5,-60);
						sleep(1);
					end
				end);
			end
		end);
		task(function()
			local att;
			for t = 0,3 do
				att = Attach.new();
				for j = 0,17 do
					att:bullet(0,0,190,j*20);
					att:bullet(0,0,195,j*20+10);
				end
				att:setAcc(-160);
				sleep(1);
				att:setAcc(0);
				att = Attach.new();
				for j = 0,17 do
					att:bullet(0,0,195,j*20);
					att:bullet(0,0,190,j*20+10);
				end
				att:setAcc(-160);
				sleep(1);
				att:setAcc(0);
			end
		end);
		sleep(4);
		task(function()
			setBulletType('dot0');
			for j = 0,4 do
				task(function()
					setAngle(j*72+36+playerAngle());
					setCenter(spirit(0,0,45,180,16,2));
					for t = 0,7 do
						bullet(0,0,50-t*5,60);
						bullet(0,0,45-t*5,20);
						bullet(0,0,45-t*5,-20);
						bullet(0,0,50-t*5,-60);
						sleep(1);
					end
				end);
			end
		end);
		moveBoss(math.random()*200-100,math.random()*20-150, 3);
		sleep(4);
	end
	for i = 0,3 do
		task(function()
			local att;
			for t = 0,3 do
				att = Attach.new();
				for j = 0,17 do
					att:bullet(0,0,190,j*20);
					att:bullet(0,0,195,j*20+10);
				end
				att:setAcc(-160);
				sleep(1);
				att:setAcc(0);
				att = Attach.new();
				for j = 0,17 do
					att:bullet(0,0,195,j*20);
					att:bullet(0,0,190,j*20+10);
				end
				att:setAcc(-160);
				sleep(1);
				att:setAcc(0);
			end
		end);
		sleep(4);
		moveBoss(math.random()*200-100,math.random()*20-150, 3);
		sleep(4);
	end
	setBulletType('round0');
	for j = 0,179 do
		bullet(0,0,180,j*2);
	end
	setBulletType('sharp1');
	for i = 0,255 do
		setAngle(math.random()*360);
		for j = 0,14 do
			bullet(0,0,160+i*4,j*24);
		end
		if i > 63 then
			clearScene();
		end
		sleep(4/128);
	end
	sleep(0.5);
	moveBoss(0,-600,1);
end

function punch()
	setBulletType('round0');
	sleepUntil(32*18+25.8125);
	local px,py = playerPos();
	moveBoss(px,600);
	local att = Attach.new();
	for i = 0,200 do
		local r,a = math.sqrt(math.random())*100,math.random()*math.pi*2;
		att:bullet(r*math.sin(a),r*math.cos(a),0,180);
	end
	sleep(0.5);
	att:setSpeed(9600);
	moveBoss(px,-600,0.125);
end

function test2()
	setBulletType('sharp1');
	for i=0,7 do
		bullet(0, 0, 40, 0);
		sleep(4);
	end
end

function main()
	--jumpTime(180);
	setCenter(boss_id, 0, 0);
	sleepUntil(0);
	--task(test2);
	---[[
	task(task_1);
	task(task_2);
	task(task_3);
	task(task_4);
	task(task_5);
	task(task_6);
	task(task_7);
	task(task_8);
	task(task_9);
	task(task_10);
	task(task_11);--]]
	--task(punch);
end

task(main);


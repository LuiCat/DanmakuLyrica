setCurrentPath('data/sound/')
loadSound('tick.wav', 'tick', 0.5)
loadSound('pa0.wav', 'pa0', 1.0)
loadSound('pa1.wav', 'pa1', 0.8)
loadSound('fa.ogg', 'fa0', 1)
loadSound('fa.ogg', 'fa1', 0.8)
loadSound('ah.ogg', 'ah', 1)

function main()
	local x=0
	local t=0
	setCenter(0,-80)
	setBulletType('sharp2')
	sleepUntil(0)
	while true do
		--x=x;
		t=0;
		seq('1111',0.125,
		function()
			bullet(0,0,12+t*1.5,x*9.29)
			bullet(0,0,12+t*1.5,120+x*9.29)
			bullet(0,0,12+t*1.5,240+x*9.29)
			x=x+1
			t=t+1
		end)
	end
end

function main3()
	local i
	local c=0
	local di=67
	local sp
	local d=-7
	setCenter(0,-80)
	setBulletType('dot1')
	--setSound('tick')
	sleepUntil(0)
	while true do
	
		task(function()
			local att=Attach.new()
			for i=0,17 do
				setAngle(i*20+12)
				sp = 60.0+math.random()*20.0
				for j=0,2 do
					att:bullet(0,0,sp,j*1.5)
				end
			end
			att:setAcc(-50.0)
			att:setRS(3.0)
			sleep(1)
			att:setAcc(3.0)
			att:offsetRot(30.0)
			sleep(2)
			att:setAcc(0)
		end)
		
		local atts = Attach.new()
		setAngle(math.random()*360)
		for i=0,4 do
			atts:spirit(0,0,5,72*i,16,2);
		end
		atts:setAcc(3.0)
		atts:setRS(d)
		d = -d
	
		sleep(2)
		
		task(function()
			local att=Attach.new()
			for i=0,17 do
				setAngle(i*20+12)
				sp = 60.0+math.random()*20.0
				for j=0,2 do
					att:bullet(0,0,sp,j*1.5)
				end
			end
			att:setAcc(-50.0)
			att:setRS(-3.0)
			sleep(1)
			att:setAcc(5.0)
			att:offsetRot(-30.0)
			sleep(2)
			att:setAcc(0)
		end)
		
		--[[
		atts = Attach.new()
		setAngle(math.random()*360)
		for i=0,3 do
			atts:spirit(0,0,5,90*i,16,2);
		end
		atts:setAcc(3.0)
		atts:setRS(-9.0)]]
		
		sleep(2)
		
	end
end

function fa()
	sleepUntil(0)
	while true do
		sound('pa0')
		sleep(1)
		sound('pa1')
		sleep(1)
	end
end

function fas()
	sleepUntil(15.8)
	sound('ah')
	sleep(32.2)
	playScore('fa0', 0.6674, 0.5,
				'-6..+1..3.b3..-7..+1.',
				'2..-#5.67+1-76.3....',
				'-6..+1..3.#4..3..#2.',
				'-7..#5..7.+3.....#5.',
				'6..3..6.7..b3..7.',
				'6.#5b3-7.+3-3.#57+321-7#5',
				'-6..+1..3.2..-7..5.',
				'..-3.#1.3.6.......')
	task(function()
		playScore('fa0', 1.3348, 0.5,
				'-3..6..+1.-7..#5..6.',
				'-#5..3.#4#56#53.1....',
				'-3..6..+1.#2..#1..-7.',
				'-#5..3..#5.7.....7.',
				'1..-6..+1.b3..-7..+b3.',
				'2.3.-3.#5..3#5776#53',
				'-4..6..+1.-7..5..2.',
				'..-3.#1.3.6.......')
	end)
	playScore('fa1', 1.3348, 0.5,
			'-6..+1..3.b3..-7..+1.',
			'2..-#5.67+1-76.3....',
			'-6..+1..3.#4..3..#2.',
			'-7..#5..7.+3.....#5.',
			'6..3..6.7..b3..7.',
			'6.#5b3-7.+3-3.#57+321-7#5',
			'-6..+1..3.2..-7..5.',
			'..-#1.-6.+#1.3.......')
	task(function()
		playScore('fa0', 1.122, 0.5,
				'-3..6..+1.-7..#5..6.',
				'-#5..3.#4#56#53.1....',
				'-3..6..+1.#2..#1..-7.',
				'-#5..3..#5.7.....7.',
				'1..-6..+1.b3..-7..+b3.',
				'2.3.-3.#5..3#5776#53',
				'-4..6..+1.-7..5..2.',
				'..-3.#1.3.6.......')
	end)
	playScore('fa1', 1.122, 0.5,
			'-6..+1..3.b3..-7..+1.',
			'2..-#5.67+1-76.3....',
			'-6..+1..3.#4..3..#2.',
			'-7..#5..7.+3.....#5.',
			'6..3..6.7..b3..7.',
			'6.#5b3-7.+3-3.#57+321-7#5',
			'-6..+1..3.2..-7..5.',
			'..-#1.-6.+#1.3.......')
end

task(main);
task(main3);
--task(fa);
--task(fas);

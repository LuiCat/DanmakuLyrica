setCurrentPath('data/sound/')
loadSound('tick.wav', 'tick', 0.5)
loadSound('pa0.wav', 'pa0', 0.7)
loadSound('pa1.wav', 'pa1', 0.7)
loadSound('fa.ogg', 'fa0', 1)
loadSound('fa.ogg', 'fa1', 0.8)
loadSound('ah.ogg', 'ah', 1)

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
	--setSound('tick')
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
			att:setAcc(20.0, 1)
			att:offsetRot(30.0, 1)
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
			att:setAcc(20.0, 1)
			att:offsetRot(-30.0, 1)
		end)
		sleep(2)
	end
end

function fa()
	sleepUntil(0)
	while true do
		soundPitch('pa0', 0.9)
		sleep(1)
		soundPitch('pa1', 1.0)
		sleep(1)
	end
end

function fas1()
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
		playScore('fa0', 0.6674, 0.5,
				'-3..6..+1.-7..#5..6.',
				'-#5..3.#4#56#53.1....',
				'-3..6..+1.#2..#1..-7.',
				'-#5..3..#5.7.....7.',
				'1..-6..+1.b3..-7..+b3.',
				'2.3.-3.#5..3#5776#53',
				'-4..6..+1.-7..5..2.',
				'..-3.#1.3.6.......')
	end)
	playScore('fa1', 0.6674, 0.5,
			'-6..+1..3.b3..-7..+1.',
			'2..-#5.67+1-76.3....',
			'-6..+1..3.#4..3..#2.',
			'-7..#5..7.+3.....#5.',
			'6..3..6.7..b3..7.',
			'6.#5b3-7.+3-3.#57+321-7#5',
			'-6..+1..3.2..-7..5.',
			'........-3.......')
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
			'........-3.......')
end

task(main);
task(main3);
task(fa);
task(fas1);
task(fas2);


const int cubeSize = 25;
const int orbAm = 20;
Orb orbs[orbAm];

elapsedMillis sincePrint = 0;
float dt;

unsigned long randomSeedVar = 0;

uint32_t lastCalc; // variable to keep track of the loops per second


void demoSetup()
{
	EEPROM.get(0, randomSeedVar);
	randomSeedVar++;
	EEPROM.put(0, randomSeedVar);
	randomSeed(randomSeedVar);
	//while (!Serial) {};

	resetDemo();
}

void resetDemo() {
	lastCalc = micros();
	for (int i = 0; i < orbAm; i++) {
		orbs[i].init(random(300, 850) / 100., random(300, 900) / 100., i * 137.508);
	}
}

extern float touchVals[36];
float xSpd, ySpd, zSpd, rSpd = 0;

void demoLoop()
{
	reset_ledLib();
	for (int i = 0; i < orbAm; i++) {
		orbs[i].move(dt);
		orbs[i].render();
		renderInterrupt();
	}

	int touchLedStart = 15625;
	int touchLedLen = 287;
	float touchsegLen = touchLedLen / 36.;

	if (frontTouchAm == 2) {
		float spd = 0;
		float touch1Pos = 0;
		float touch1Dist = 0;
		float touch2Pos = 0;
		float touch2Dist = 0;
		int t = 1;
		for (uint8_t j = 0; j < MAX_TOUCH_AM; j++) {
			if (!touches[j].active) continue;
			if (touches[j].pos >= 12 && touches[j].pos < 24) {
				if (t == 1) {
					touch1Pos = touches[j].pos;
					touch1Dist = touches[j].movedDist();
				}
				else {
					touch2Pos = touches[j].pos;
					touch2Dist = touches[j].movedDist();
				}
				t = -1;
			}
		}
		if (touch1Pos > touch2Pos) spd = touch1Dist - touch2Dist;
		else spd = touch2Dist - touch1Dist;
		scaleOrbs(spd * 0.1 + 1);
	}

	for (uint8_t j = 0; j < MAX_TOUCH_AM; j++) {
		if (!touches[j].active) continue;

		float pos = touches[j].pos;
		float spd = touches[j].movedDist() * 2;


		if (pos < 12)
			if (leftTouchAm == 2) ySpd = spd;
			else zSpd = -spd;
		else if (pos < 24) {
			if (frontTouchAm != 2)
				xSpd = spd;
		}
		else
			rSpd = spd;


		float spread = touches[j].pow * 0.5;
		float position = touchLedLen - touchsegLen * pos * 0.75 - touchLedLen / 2 - touchsegLen / 2;
		if (pos > 23) position += touchLedLen;
		int startLed = position - spread / 2;
		int endLed = position + spread / 2;

		for (int i = startLed; i <= endLed; i++) {
			if (i >= 0 && i < touchLedLen) {
				float dist = constrain(1 - abs(i - position) / spread * 2, 0, 1);
				Color col(touches[j].hue, 100, 100, HSB_MODE);
				Color c = _get_ledLib(touchLedStart + i);
				c.add(col, dist);
				leds.setPixel(touchLedStart + i, c.red(), c.green(), c.blue());
			}
		}
	}


	translateOrbs(xSpd, ySpd, zSpd);
	if (abs(rSpd) > 0.01) rotateOrbs(rSpd * 0.1);

	xSpd *= 0.97;
	ySpd *= 0.97;
	zSpd *= 0.97;
	rSpd *= 0.97;


	show_ledLib();



	calcDt();
	if (sincePrint > 1000) {
		Serial.print("FPS: ");
		Serial.println(getFPS());
		Serial.println(touchSampleCounter);
		Serial.println();
		touchSampleCounter = 0;
		sincePrint = 0;
	}

}



void translateOrbs(float x, float y, float z) {
	for (int i = 0; i < orbAm; i++) {
		orbs[i].translate(x, y, z);
	}
}

void rotateOrbs(float r) {
	for (int i = 0; i < orbAm; i++) {
		orbs[i].rotate(r);
	}
}

void scaleOrbs(float s) {
	for (int i = 0; i < orbAm; i++) {
		orbs[i].scale(s);
	}
}


int getLedId(int x, int y, int z) {
	int id = y;
	id += 15000 - z * 625; // 24*25*25 - z*25*25;
	id += 600 - x * 25;  // 25*24 - x*25;

	return id;
}



void calcDt() {
	uint32_t currTime = micros();
	dt = (currTime - lastCalc) / 1000000.;  // assume one frame per second
	lastCalc = currTime;
}

float getFPS() {
	return 1 / dt;
}


void _set_ledLib(int pixel, byte r, byte g, byte b) {
	leds.setPixel(pixel, r, g, b);
}


void set_ledLib(byte x, byte y, byte z, byte r, byte g, byte b) {
	leds.setPixel(getLedId(x, y, z), r, g, b);
}


void show_ledLib() {
	leds.show();
}

uint32_t bufsize = ledsPerStrip * 384;
void reset_ledLib() {
	memset(drawingMemory, 0, bufsize);
}

Color get_ledLib(byte x, byte y, byte z) {
	uint32_t conn = leds.getPixel(getLedId(x, y, z));  // retrieve the color that has already been saved
	byte b = conn & 255;       // unpack the color
	byte g = conn >> 8 & 255;
	byte r = conn >> 16 & 255;
	Color pixelCol(r, g, b, RGB_MODE);
	return pixelCol;
}

Color _get_ledLib(int id) {
	uint32_t conn = leds.getPixel(id);  // retrieve the color that has already been saved
	byte b = conn & 255;       // unpack the color
	byte g = conn >> 8 & 255;
	byte r = conn >> 16 & 255;
	Color pixelCol(r, g, b, RGB_MODE);
	return pixelCol;
}

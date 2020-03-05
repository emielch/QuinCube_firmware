// 
// 
// 

#include "Orb.h"

extern void renderInterrupt();

Orb::Orb()
{
	xSpd = 0;
	ySpd = 0;
	zSpd = 0;
}

void Orb::init(float _size, float _spd, float _hue)
{
	xPos = random(1, cubeSize - 1);
	yPos = random(1, cubeSize - 1);
	zPos = random(1, cubeSize - 1);

	_spd *= 1000;
	xSpd = random(-_spd, _spd) / 1000.;
	ySpd = random(-_spd, _spd) / 1000.;
	zSpd = random(-_spd, _spd) / 1000.;

	//if (abs(xSpd) < 1) xSpd = 1;
	//if (abs(ySpd) < 1) ySpd = 1;
	//if (abs(zSpd) < 1) zSpd = 1;

	size = _size / 2.;
	hue = _hue;


}

float bouncePow = 10;
float resetDist = 5;

void Orb::move(float dt)
{
	hue += 5 * dt;
	if (hue >= 360) hue -= 360;
	c.setHSB(hue, 100, 30);


	xPos += xSpd * dt;
	yPos += ySpd * dt;
	zPos += zSpd * dt;

	//if (xPos > cubeSize) {
	//	xSpd -= bouncePow * dt;
	//}
	//else if (xPos < 0) {
	//	xSpd += bouncePow * dt;
	//}

	//if (yPos > cubeSize) {
	//	ySpd -= bouncePow * dt;
	//}
	//else if (yPos < 0) {
	//	ySpd += bouncePow * dt;
	//}

	//if (zPos > cubeSize) {
	//	zSpd -= bouncePow * dt;
	//}
	//else if (zPos < 0) {
	//	zSpd += bouncePow * dt;
	//}





	/// check for reset
	if (xPos > cubeSize + size) {
		reset(-size, -1, -1);
	}
	else if (xPos < -size) {
		reset(cubeSize + size, -1, -1);
	}
	else if (yPos > cubeSize + size) {
		reset(-1, -size, -1);
	}
	else if (yPos < -size) {
		reset(-1, cubeSize + size, -1);
	}
	else if (zPos > cubeSize + size) {
		reset(-1, -1, -size);
	}
	else if (zPos < -size) {
		reset(-1, -1, cubeSize + size);
	}
}


void Orb::reset(int x, int y, int z)
{
	if (x == -1) x = random(1, cubeSize - 1);
	if (y == -1) y = random(1, cubeSize - 1);
	if (z == -1) z = random(1, cubeSize - 1);
	xPos = x;
	yPos = y;
	zPos = z;

	//float _spd = random(300, 900) / 100.;
	//_spd *= 1000;
	//xSpd = random(-_spd, _spd) / 1000.;
	//ySpd = random(-_spd, _spd) / 1000.;
	//zSpd = random(-_spd, _spd) / 1000.;
}

void Orb::translate(float x, float y, float z)
{
	xPos += x;
	yPos += y;
	zPos += z;
}

void Orb::rotate(float r)
{
	float xNew = (xPos - cubeSize * 0.5) * cos(r) - (zPos - cubeSize * 0.5) * sin(r);
	float zNew = (xPos - cubeSize * 0.5) * sin(r) + (zPos - cubeSize * 0.5) * cos(r);

	xPos = xNew + cubeSize * 0.5;
	zPos = zNew + cubeSize * 0.5;
}

void Orb::scale(float s)
{
	xPos = (xPos - cubeSize * 0.5) * s + cubeSize * 0.5;
	zPos = (zPos - cubeSize * 0.5) * s + cubeSize * 0.5;
	size *= s;
	xSpd *= s;
	ySpd *= s;
	zSpd *= s;
}


void Orb::render()
{
	int x1 = constrain(xPos - size, 0, cubeSize - 1);
	int x2 = constrain(xPos + size, 0, cubeSize - 1);
	int y1 = constrain(yPos - size, 0, cubeSize - 1);
	int y2 = constrain(yPos + size, 0, cubeSize - 1);
	int z1 = constrain(zPos - size, 0, cubeSize - 1);
	int z2 = constrain(zPos + size, 0, cubeSize - 1);

	for (int x = x1; x <= x2; x++) {
		for (int y = y1; y <= y2; y++) {
			for (int z = z1; z <= z2; z++) {
				float Dx = x - xPos;
				float Dy = y - yPos;
				float Dz = z - zPos;
				float dist = sqrt(Dx * Dx + Dy * Dy + Dz * Dz);

				float bri = size - dist;
				bri = bri / size;
				if (bri <= 0) continue;


				Color pixCol = get_ledLib(x, y, z);
				pixCol.add(c, bri);

				set_ledLib(x, y, z, pixCol.red(), pixCol.green(), pixCol.blue());


			}
		}
	}
}



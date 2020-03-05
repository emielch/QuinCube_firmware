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
	xPos = random(1, cubeSize-1);
	yPos = random(1, cubeSize-1);
	zPos = random(1, cubeSize-1);

	_spd *= 1000;
	xSpd = random(-_spd, _spd)/1000.;
	ySpd = random(-_spd, _spd) / 1000.;
	zSpd = random(-_spd, _spd) / 1000.;

	//if (abs(xSpd) < 1) xSpd = 1;
	//if (abs(ySpd) < 1) ySpd = 1;
	//if (abs(zSpd) < 1) zSpd = 1;

	size = _size;
	hue = _hue;


}

float bouncePow = 10;

void Orb::move(float dt)
{ 
	hue += 5 * dt;
	if (hue >= 360) hue -= 360;
	c.setHSB(hue, 100, 30);


	xPos += xSpd * dt;
	yPos += ySpd * dt;
	zPos += zSpd * dt;

	if (xPos > cubeSize) {
		xSpd -= bouncePow * dt;
	}
	else if (xPos < 0) {
		xSpd += bouncePow * dt;
	}

	if (yPos > cubeSize) {
		ySpd -= bouncePow * dt;
	}
	else if (yPos < 0) {
		ySpd += bouncePow * dt;
	}

	if (zPos > cubeSize) {
		zSpd -= bouncePow * dt;
	}
	else if (zPos < 0) {
		zSpd += bouncePow * dt;
	}
}

void Orb::translate(float x, float y, float z)
{
	xPos += x;
	yPos += y;
	zPos += z;
}


void Orb::render()
{  
	int x1 = constrain(xPos - size / 2, 0, cubeSize-1);
	int x2 = constrain(xPos + size / 2, 0, cubeSize-1);
	int y1 = constrain(yPos - size / 2, 0, cubeSize-1);
	int y2 = constrain(yPos + size / 2, 0, cubeSize-1);
	int z1 = constrain(zPos - size / 2, 0, cubeSize-1);
	int z2 = constrain(zPos + size / 2, 0, cubeSize-1);

	for (int x = x1; x <= x2; x++) {
		for (int y = y1; y <= y2; y++) {
			for (int z = z1; z <= z2; z++) {
				float Dx = x - xPos;
				float Dy = y - yPos;
				float Dz = z - zPos;
				float dist = sqrt(Dx * Dx + Dy * Dy + Dz * Dz);

				float bri = (size / 2) - dist;
				bri = bri / (size / 2);
				if (bri <= 0) continue;
       

				Color pixCol = get_ledLib(x, y, z);
				pixCol.add(c, bri);

				set_ledLib(x, y, z, pixCol.red(), pixCol.green(), pixCol.blue());


			}
		}
	}
}

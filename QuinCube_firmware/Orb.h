// Orb.h

#ifndef _ORB_h
#define _ORB_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Colore.h>

extern int ledAm;
extern const int cubeSize;
extern void set_ledLib(byte x, byte y, byte z, byte r, byte g, byte b);
extern Color get_ledLib(byte x, byte y, byte z);


class Orb {

public:
	Orb();
	void init(float _size, float _spd, float _hue);
	void move(float dt);
	void translate(float x, float y, float z);
	void render();


private:
	float xPos, yPos, zPos, size, xSpd, ySpd, zSpd;
	Color c;
	float hue;
};

#endif

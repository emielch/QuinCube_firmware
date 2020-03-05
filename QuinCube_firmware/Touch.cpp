// 
// 
// 

#include "Touch.h"

Touch::Touch()
{
	active = false;
	pos = 0;
	pow = 0;
	hue = 0;

}

void Touch::init(float _hue)
{
	active = true;
	pos = 0;
	pow = 0;
	hue = _hue;
	posDiff = 0;
}

void Touch::newPos(float _pos)
{
	posDiff += _pos - pos;
	pos = _pos;
}

float Touch::movedDist()
{
	float returnVal = posDiff;
	posDiff = 0;
	return returnVal;
}

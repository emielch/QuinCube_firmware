bool touchChecked[SENS_AM];


void updateTouch() {
	for (int i = 0; i < SENS_AM; i++) touchChecked[i] = false;


	for (int i = 0; i < MAX_TOUCH_AM; i++) {
		if (!touches[i].active) continue;

		
		int pos = touches[i].pos;
		float newPos = 0;
		float newPow = 0;
		while (pos >= 0) {
			touchChecked[pos] = true;
			if (touchVals[pos] > 1) {
				newPos += pos * touchVals[pos];
				newPow += touchVals[pos];
				pos--;
			}
			else break;
		}
		pos = touches[i].pos + 1;
		while (pos <= SENS_AM) {
			touchChecked[pos] = true;
			if (touchVals[pos] > 1) {
				newPos += pos * touchVals[pos];
				newPow += touchVals[pos];
				pos++;
			}
			else break;
		}
		if (newPow != 0) {
			newPos /= newPow;
			touches[i].newPos(newPos);
			touches[i].pow = touches[i].pow*0.5 + newPow*0.5;

			if(findTouchOnPos(&touches[i])) touches[i].active = false;
		}
		else touches[i].active = false;
	}


	float newPos = 0;
	float newPow = 0;
	Touch* prevTouch = NULL;

	for (int i = 0; i < SENS_AM; i++) {
		if (touchVals[i] > 5 && !touchChecked[i]) {
			newPos += i * touchVals[i];
			newPow += touchVals[i];
		}
		else {
			if (newPow != 0) {
				newPos /= newPow;

				createTouch(newPos, newPow);
			}
			newPos = 0;
			newPow = 0;
			prevTouch = NULL;
		}
	}
}

float hue = 0;

Touch* createTouch(float pos, float pow) {
	for (uint8_t i = 0; i < MAX_TOUCH_AM; i++) {
		if (touches[i].active) continue;
		touches[i].init(hue);
		hue += 137.508;
		if (hue >= 360) hue -= 360;
		touches[i].pos = pos;
		touches[i].pow = pow;
		return &touches[i];
	}
	return NULL;
}


Touch* findTouchOnPos(Touch* t) {
	for (uint8_t i = 0; i < MAX_TOUCH_AM; i++) {
		if (t == &touches[i]) continue;
		if (!touches[i].active) continue;

		if ((int)touches[i].pos == (int)t->pos) return &touches[i];
	}
	return NULL;
}
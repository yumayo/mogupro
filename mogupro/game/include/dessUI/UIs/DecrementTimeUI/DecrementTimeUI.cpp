#include "DecrementTimeUI.h"

void DecrementTimeUI::timeUpdate()
{
	if (flame <= 0 &&
		seconds <= 0 &&
		minutes <= 0) {
		time_zero = true;
	}else if (start) {
		time_zero = false;
		flame--;
		if (flame < 0) {
			flame = 59;
			seconds--;
		}
		if (seconds < 0) {
			seconds = 59;
			minutes--;
		}
	}
}

void DecrementTimeUI::deltaTimeUpdate(float delta)
{
	if (flame <= 0 &&
		seconds <= 0 &&
		minutes <= 0) {
		time_zero = true;
	}
	else if (start) {
		time_zero = false;
		flame -= delta * 40;
		if (flame < 0) {
			flame = 59;
			dt = 0;
			seconds--;
		}
		if (seconds < 0) {
			seconds = 59;
			minutes--;
		}
	}
}
#include "IncrementTimeUI.h"

void IncrementTimeUI::timeUpdate()
{
	if (start) {
		flame++;
		if (flame >= 60) {
			flame = 0;
			seconds++;
		}
		if (seconds >= 60) {
			seconds = 0;
			minutes++;
		}
	}
}
void IncrementTimeUI::deltaTimeUpdate(float delta)
{
	if (start) {
		dt += delta*60;
		flame = dt;
		if (flame >= 60) {
			flame = 0;
			dt = 0;
			seconds++;
		}
		if (seconds >= 60) {
			seconds = 0;
			minutes++;
		}
	}
}
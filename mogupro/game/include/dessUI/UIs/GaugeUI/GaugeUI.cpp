#include "GaugeUI.h"

void GaugeUI::gaugeChangeX(float value, float max)
{
	
	if (value > max) {
		is_max = true;
		is_min = false;
		gauge_size.x = max;
	}
	else if (value == max) {
		is_min = false;
		is_max = true;
		gauge_size.x = (value / max) * gauge_max_x;
	}
	else {
		is_min = false;
		is_max = false;
		gauge_size.x = (value / max) * gauge_max_x;
	}
	if (value <= 0) {
		is_min = true;
		gauge_size.x = 0;
	}
}

void GaugeUI::gaugeChangeY(float value, float max)
{
	if (value >= max) {
		is_max = true;
	}
	else {
		is_max = false;
		gauge_size.y = value / max * gauge_max_x;
	}
}
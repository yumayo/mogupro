#pragma once
#include"../../UIBase.h"

class DecrementTimeUI : public UIBase
{
private:
	std::string font_path;
	bool start;
	bool time_zero;
	float dt;
	int flame;
	int seconds;
	int minutes;
public:
	DecrementTimeUI(des::Vec2f _pos, des::Vec2f _size, des::Vec4d _color) :UIBase(_pos, _size, _color)
	{
		start = false;
		time_zero = false;
		own_ui = UITYPE::DecrementTimeUI;
		color = des::Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
		flame = 0;
		seconds = 0;
		minutes = 0;
		dt = 0;
	};

	void timeUpdate();
	void deltaTimeUpdate(float delta);
	std::string timeGetFlame() {
		if (9 >= flame) {
			return "0" + std::to_string(flame);
		}
		return std::to_string(flame);
	}
	std::string timeGetSeconds() {
		if (9 >= seconds) {
			return "0" + std::to_string(seconds);
		}
		return std::to_string(seconds);
	}
	std::string timeGetMinutes() {
		return std::to_string(minutes);
	}
	std::string fontGetPath() {
		return font_path;
	}
	float fontGetSize() {
		return size.x;
	}
	bool timeOver() {
		return time_zero;
	}



	void timeStart() {
		start = true;
	}
	void timeStop() {
		start = false;
	}
	void timeSetFlame(const int& flame) {
		this->flame = flame;
	}
	void timeSetSeconds(const int& seconds) {
		this->seconds = seconds;
	}
	void timeSetMinutes(const int& minutes) {
		this->minutes = minutes;
	}
	void fontSetPath(const std::string& font_path) {
		this->font_path = font_path;
	}
	void fontSetSize(const float& size) {
		this->size.x = size;
	}
};
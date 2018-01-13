#pragma once
#include"../../UIBase.h"

class IncrementTimeUI : public UIBase
{
private:
	std::string font_path;
	bool start;
	float dt;
	int flame;
	int seconds;
	int minutes;
public:
	IncrementTimeUI(des::Vec2f _pos, des::Vec2f _size, des::Vec4d _color) :UIBase(_pos, _size, _color)
	{
		start = false;
		own_ui = UITYPE::IncrementTimeUI;
		color = des::Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
		flame = 0;
		seconds = 0;
		minutes = 0;
		dt = 0;
	};

	void timeUpdate();
	void deltaTimeUpdate(float delta);
	
	void timeSetFlame(const int& flame) {
		this->flame = flame;
	}
	void timeSetSeconds(const int& seconds) {
		this->seconds = seconds;
	}
	void timeSetMinutes(const int& minutes) {
		this->minutes = minutes;
	}
	std::string timeGetFlame() {
		if (9 >= static_cast<int>(flame)) {
			return "0" + std::to_string(static_cast<int>(flame));
		}
		return std::to_string(static_cast<int>(flame));
	}
	std::string timeGetSeconds() {
		if (9 >= static_cast<int>(seconds)) {
			return "0" + std::to_string(static_cast<int>(seconds));
		}
		return std::to_string(static_cast<int>(seconds));
	}
	std::string timeGetMinutes() {
		return std::to_string(static_cast<int>(minutes));
	}
	std::string fontGetPath() {
		return font_path;
	}
	float fontGetSize() {
		return size.x;
	}
	void timeStart() {
		start = true;
	}
	void timeStop() {
		start = false;
	}
	void fontSetPath(const std::string& font_path) {
		this->font_path = font_path;
	}
	void fontSetSize(const float& size) {
		this->size.x = size;
	}
};
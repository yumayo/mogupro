#pragma once
#include "../../UIBase.h"


class AnimationUI : public UIBase
{
private:
	des::Vec2i seats;
	des::Vec2f cut_size;
	float between;
	bool looping;
	bool animation_end;
public:
	AnimationUI(des::Vec2f _pos, des::Vec2f _size, des::Vec4d _color) :UIBase(_pos, _size, _color)
	{
		own_ui = UITYPE::AnimationUI;
		seats = des::Vec2f::zero();
		cut_size = des::Vec2f::zero();
		between = 0;
		looping = false;
		animation_end = false;
	};
	float animationGetCutSizeX() {
		return cut_size.x;
	}
	float animationGetCutSizeY() {
		return cut_size.y;
	}
	int animationGetSeatsX() {
		return seats.x;
	}
	int animationGetSeatsY() {
		return seats.y;
	}
	bool animationGetIsLooping() {
		return looping;
	}
	bool animationGetIsEnd() {
		return animation_end;
	}
	float animationGetBetween() {
		return between;
	}

	void animationSetCutSize(const float& cut_size_x, const float& cut_size_y) {
		cut_size.x = cut_size_x;
		cut_size.y = cut_size_y;
	}
	void animationSetSeats(const int& seats_x, const int& seats_y) {
		seats.x = seats_x;
		seats.y = seats_y;
	}

	void animationSetLooping(const bool& looping) {
		this->looping = looping;
	}
	void animationSetIsEnding(const bool& animation_end) {
		this->animation_end = animation_end;
	}
	void animationSetBetween(const float& between) {
		this->between = between;
	}
	void EaseOutUpdate();
	
};
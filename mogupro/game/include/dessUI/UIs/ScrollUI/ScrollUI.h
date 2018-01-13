#pragma once
#include"../../UIBase.h"

class ScrollUI : public UIBase
{
private:
	std::vector<EasingBuf> ease_collision_buf;
	des::Vec4f font_color;
	des::Vec2f font_pos;
	std::string text;
	float font_scale;
public:
	ScrollUI(des::Vec2f _pos, des::Vec2f _size, des::Vec4d _color) :UIBase(_pos, _size, _color)
	{
		own_ui = UITYPE::ScrollUI;
		font_color = des::Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	};

	void animation();
	void update();

	std::string fontGetText() {
		return text;
	}
	bool collisionToUI(des::Vec2f _pos);
	bool collisionToUI(des::Vec2f _pos, des::Vec2f _size);
	void fontSetText(const std::string& text) {
		this->text = text;
	}
	void fontSetColor(const float& color_r, const float& color_g, const float& color_b, const float& color_a) {
		font_color.x = color_r;
		font_color.y = color_g;
		font_color.z = color_b;
		font_color.w = color_a;
	}
	void fontSetPos(const float& pos_x, const float& pos_y) {
		font_pos.x = pos_x;
		font_pos.y = pos_y;
	}
	void fontSetScale(const float& scale) {
		font_scale = scale;
	}
	
	float fontGetPosX() { return font_pos.x; }
	float fontGetPosY() { return font_pos.y; }
	float fontGetScale() {
		return font_scale;
	}
	float fontGetColorR() {
		return font_color.x;
	}
	float fontGetColorG() {
		return font_color.y;
	}
	float fontGetColorB() {
		return font_color.z;
	}
	float fontGetColorA() {
		return font_color.w;
	}

	void EaseCollisionApply(std::string start, float end[4], std::function<float(float, float, float)> ease, float time);
	void EaseCollisionUpdate();
};
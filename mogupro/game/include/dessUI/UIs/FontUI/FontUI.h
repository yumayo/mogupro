#pragma once
#include"../../UIBase.h"

class FontUI : public UIBase
{
private:
	std::string font_path;
	std::string text;
	des::Vec4f font_color;
	des::Vec2f font_pos;
	float font_scale;
public:
	FontUI(des::Vec2f _pos, des::Vec2f _size, des::Vec4d _color) :UIBase(_pos, _size, _color) 
	{
		own_ui = UITYPE::FontUI;
		color = des::Vec4f(0.0f,0.0f,0.0f,1.0f);
	};
	std::string fontGetText() {
		return text;
	}
	std::string fontGetPath() {
		return font_path;
	}
	float fontGetSize() {
		return size.x;
	}
	void fontSetPath(const std::string& font_path) {
		this->font_path = font_path;
	}
	void fontSetSize(const float& size) {
		this->size.x = size;
	}

	void fontSetText(const std::string& text) {
		this->text = text;
	}
	void fontSetColor(const float& color_r, const float& color_g, const float& color_b, const float& color_a) {
		font_color.x = color_r;
		font_color.y = color_g;
		font_color.z = color_b;
		font_color.w = color_a;
	}
	
	void fontSetScale(const float& scale) {
		font_scale = scale;
	}
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

};
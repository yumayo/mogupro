#pragma once
#include "myLib/DesLib.h"
#include "picojson.h"
#include "Easing/Easing.h"
#include <fstream>
#include <cassert>
#include <sstream>
#include <iostream>
#include <functional>

enum class UITYPE {
	CollisionUI,
	FontUI,
	GaugeUI,
	IncrementTimeUI,
	DecrementTimeUI,
	AnimationUI,
	ScrollUI,
	NormalUI
};

struct EasingBuf
{
	EasingBuf(std::string _start, float _end[4], std::function<float(float, float, float)> _ease, float _time) {
		start = _start;
		for (int i = 0; i < 4; i++) {
			end[i] = _end[i];
		}
		ease = _ease;
		time = _time;
	}
	std::string start;
	float end[4];
	std::function<float(float, float, float)> ease;
	float time;
};

class UIBase
{
protected:

	des::Vec2f pos;
	des::Vec2f size;
	des::Vec4f color;

	std::vector<std::function<float(float, float, float)>> easing_function;
	std::vector<std::function<void()>> ease_out_function;
	std::string texture_path;

	std::vector<EasingBuf> ease_in_buf;
	std::vector<EasingBuf> ease_end_buf;
	std::vector<EasingBuf> ease_update_buf;


	std::ifstream fs;
    
	int start;
	bool end;
	bool is_active;
	int end_count;


public:
	UITYPE own_ui;
	UIBase(des::Vec2f _pos, des::Vec2f _size, des::Vec4d _color);
	//
	virtual void update();

	void EaseInApply(std::string start, float end[4], std::function<float(float, float, float)> ease, float time);
	void EaseOutApply(std::string start, float end[4], std::function<float(float, float, float)> ease, float time);
	void EaseUpdateApply(std::string start, float end[4], std::function<float(float, float, float)> ease, float time);
	virtual void EaseCollisionApply(std::string start, float end[4], std::function<float(float, float, float)> ease, float time) {}


	void EaseUpdate();
	void EaseInUpdate();
	void EaseOutUpdate();



	///////////////////////ゲッター///////////////////////////////
	//コンスト参照にしてないのは動かなかったから
	float getPosX() {
		return pos.x;
	}
	float getPosY() {
		return pos.y;
	}
	float getSizeX() {
		return size.x;
	}
	float getSizeY() {
		return size.y;
	}
	float getColorR() {
		return color.x;
	}
	float getColorG() {
		return color.y;
	}
	float getColorB() {
		return color.z;
	}
	float getColorA() {
		return color.w;
	}

	bool isEasingEnd() {
		if (c_Easing::isEnd(pos.x) &&
			c_Easing::isEnd(pos.y) &&
			c_Easing::isEnd(size.x) &&
			c_Easing::isEnd(size.y) &&
			c_Easing::isEnd(color.x) &&
			c_Easing::isEnd(color.y) &&
			c_Easing::isEnd(color.z) &&
			c_Easing::isEnd(color.w)) {
			return true;
		}
		return false;
	}

	std::string getTexturePath() {
		return texture_path;
	}
    const bool& getActive() {
        return is_active;
    }
	const bool& NextFlag() {
		return !is_active;
	}
	UITYPE getUIType() {
		return own_ui;
	}
	
	//----------------------------------------------------------


	////////////////////セッター///////////////////////////////
	void setPos(const float& pos_x, const float& pos_y) {
		pos.x = pos_x;
		pos.y = pos_y;
	}
	void setSize(const float& size_x, const float& size_y) {
		size.x = size_x;
		size.y = size_y;
	}
	void setColor(const float& color_r, const float& color_g, const float& color_b, const float& color_a) {
		color.x = color_r;
		color.y = color_g;
		color.z = color_b;
		color.w = color_a;
	}

	void setTexturePath(std::string path) {
		texture_path = path;
	}
	void setEnd() {
		if (is_active) {
			end = true;
		}
	}
    void setActive(const bool& active) {
        if ( is_active == true &&
            active == false) {
            end = true;
			return;
        }
        is_active = active;
    }
    
	//---------------------------------------------------------


	//点との当たり判定
	virtual bool collisionToUI(des::Vec2f _pos) { 
		if (!is_active) return false;
		return false; 
	};
	//四角との当たり判定
	virtual bool collisionToUI(des::Vec2f _pos, des::Vec2f _size) {
		if (!is_active) return false;
		return false;
	};

	//GaugeUI
	virtual float gaugeGetPosX() { return 0.0f; }
	virtual float gaugeGetPosY() { return 0.0f; }
	virtual float gaugeGetSizeX() { return 0.0f; }
	virtual float gaugeGetSizeY() { return 0.0f; }
	virtual std::string gaugeGetTexturePath() { return "GaugeUIで定義してください"; }

	virtual bool gaugeGetIsMax() { return false; }

	virtual void gaugeSetPos(const float& gauge_pos_x, const float& gauge_pos_y) {}
	virtual void gaugeSetSize(const float& gauge_size_x, const float& gauge_size_y) {}
	virtual void gaugeSetTexturePath(const std::string& path) {}
	//横
	//変動する値
	//最大値
	virtual void gaugeChangeX(float value, float max) {}
	//縦
	//変動する値
	//最大値
	virtual void gaugeChangeY(float value, float max) {}

	//FontUI
	
	virtual std::string fontGetText() {
		return "何も入力されていません";
	}
	virtual std::string fontGetPath() {
		return "何も入力されていません";
	}
	virtual float fontGetSize() {
		return 0.0f;
	}
	virtual void fontSetText(const std::string& text) {}
	virtual void fontSetText(const float& value) {}
	virtual void fontSetPath(const std::string& font_path) {}
	virtual void fontSetSize(const float& size) {}

	//TimeUI
	virtual std::string timeGetFlame() {
		return "何も入力されていません";
	}
	virtual std::string timeGetSeconds() {
		return "何も入力されていません";
	}
	virtual std::string timeGetMinutes() {
		return "何も入力されていません";
	}
	virtual bool timeOver() {
		return false;
	}

	virtual void timeSetFlame(const int& flame) {}
	virtual void timeSetSeconds(const int& seconds) {}
	virtual void timeSetMinutes(const int& minutes) {}

	virtual void timeStart() {}
	virtual void timeStop() {}
	virtual void timeUpdate() {}
	virtual void deltaTimeUpdate(float delta) {}


	//AnimationUI
	virtual float animationGetCutSizeX() { return 0.0f; }
	virtual float animationGetCutSizeY() { return 0.0f; }
	virtual int animationGetSeatsX() { return 1; }
	virtual int animationGetSeatsY() { return 1; }
	virtual bool animationGetIsLooping() { return false; }
	virtual bool animationGetIsEnd() { return false; }
	virtual float animationGetBetween() { return 0.0f; }

	virtual void animationSetCutSize(const float& cut_size_x, const float& cut_size_y) {}
	virtual void animationSetSeats(const int& seats_x, const int& seats_y) {}

	virtual void animationSetLooping(const bool& looping) {}
	virtual void animationSetIsEnding(const bool& animation_end) {}
	virtual void animationSetBetween(const float& between) {}

	//ScrollUI
	virtual float fontGetColorR() {
		return 0.0f;
	}
	virtual float fontGetColorG() {
		return 0.0f;
	}
	virtual float fontGetColorB() {
		return 0.0f;
	}
	virtual float fontGetColorA() {
		return 0.0f;
	}
	virtual float fontGetScale() {
		return 0.0f;
	}
	virtual float fontGetPosX() { return 0.0f; }
	virtual float fontGetPosY() { return 0.0f; }

	virtual void fontSetColor(const float& color_r, const float& color_g, const float& color_b, const float& color_a) {}
	virtual void fontSetPos(const float& pos_x, const float& pos_y) {}
	virtual void fontSetScale(const float& scale) {}
};




#pragma once
#include <unordered_map>
#include <functional>
#include <cmath>

#define M_PI 3.14159265358979


class c_Easinginfo
{
public:
	c_Easinginfo(float _begincount, float _endcoount, float _beginpos, float _endpos, std::function<float(float, float, float)> _func) {
		func = _func;
		begincount = _begincount;
		endcoount = _endcoount;
		beginpos = _beginpos;
		endpos = _endpos;

	};
	float getValue(const float& count) {

		return func((count - begincount) / (endcoount - begincount), beginpos, endpos);

	}
	bool isEreas(const float&  count) {
		return count > endcoount;
	}
	float getEndCount() {
		return endcoount;
	}

	std::function<float(float, float, float)> func;
	float begincount;
	float endcoount;
	float beginpos;
	float endpos;
};
class c_AnimInfo
{
public:
	c_AnimInfo() {
		count = 0;
		is_active = true;
	}
	void apply(float& _terget,
		float endValue,
		std::function<float(float, float, float)> _easeFunction,
		float _time) {
		if (info.size() == 0) {
			info.push_back(c_Easinginfo(0, _time, _terget, endValue, _easeFunction));
			terget = &_terget;
			return;
		}
		if (info.size() > 0) {
			info.push_back(
				c_Easinginfo(info.back().endcoount,
					info.back().endcoount + _time,
					info.back().endpos,
					endValue,
					_easeFunction));
		}

	}

	void countIncrement(const float& delta) {
		if (!is_active)return;
		if (info.size() > 0) {
			*terget = info.begin()->getValue(count);
			count += delta;
			if (info.begin()->isEreas(count)) {
				*terget = info.begin()->getValue(info.begin()->getEndCount());
				info.erase(info.begin());
			}
		}
	}
	void clear() {
		count = 0;
		info.clear();
	}

	float* terget;
	std::vector<c_Easinginfo> info;
	float count;
	bool is_active;
};

class c_Easing
{
public:

	//static c_Easing singl() {
	//	static c_Easing hoge;
	//	return hoge;
	//}
	//イージングを積み上げる関数
	//イージングさせたい値
	//イージングさせた後の値
	//イージング関数
	//frame数
	static void apply(
		float& _terget,
		float _endValue,
		std::function<float(float, float, float)> _easeFunction,
		float _time = 0.5f) {

		easinginfo[&_terget].apply(_terget, _endValue, _easeFunction, _time);
	}
	//積み上げたイージングを全部削除
	static void clear(float& _terget) {
		easinginfo[&_terget].clear();
	}
	static void clear() {
		easinginfo.clear();
	}
	//イージングを止める
	static void stop(float& _terget) {
		easinginfo[&_terget].is_active = false;
	}
	//イージングを再開する
	static void start(float& _terget) {
		easinginfo[&_terget].is_active = true;
	}
	//全てのイージングが終わったらtrueを返す関数
	static bool isEnd(float& _terget) {
		return easinginfo.find(&_terget) == easinginfo.end();
	}
	//drawの最後に呼ぶ関数
    static void update(float delta) {
		for (auto& it : easinginfo) { it.second.countIncrement(delta); };


		for (auto i = easinginfo.begin(); i != easinginfo.end();)
		{
			if (i->second.info.size() == 0) {
				i = easinginfo.erase(i);
				continue;
			}
			i++;
		}
	}
private:
	static std::unordered_map <float*, c_AnimInfo>  easinginfo;

};


//イージング関数群
namespace EasingFunction {
	static float Linear(float t, float b, float e) {
		return (e - b) * t + b;
	}
	static float BackIn(float t, float b, float e) {
		float s = 1.70158f;
		return (e - b) * t * t * ((s + 1) * t - s) + b;
	}
	static float BackOut(float t, float b, float e) {
		float s = 1.70158f;
		t -= 1.0;
		return (e - b) * (t * t * ((s + 1) * t + s) + 1) + b;
	}
	static float BackInOut(float t, float b, float e) {
		float s = 1.70158f * 1.525f;
		if ((t /= 0.5) < 1) return (e - b) / 2 * (t * t * ((s + 1) * t - s)) + b;
		t -= 2;
		return (e - b) / 2 * (t * t * ((s + 1) * t + s) + 2) + b;
	}
	static float BounceOut(float t, float b, float e) {
		if (t < (1 / 2.75f)) {
			return (e - b) * (7.5625f * t * t) + b;
		}
		else if (t < (2 / 2.75f)) {
			t -= (1.5f / 2.75f);
			return (e - b) * (7.5625f * t * t + 0.75f) + b;
		}
		else if (t < (2.5 / 2.75)) {
			t -= (2.25f / 2.75f);
			return (e - b) * (7.5625f * t * t + 0.9375f) + b;
		}
		else {
			t -= (2.625f / 2.75f);
			return (e - b) * (7.5625f * t * t + 0.984375f) + b;
		}
	}
	static  float BounceIn(float t, float b, float e) {
		return (e - b) - BounceOut(1.0f - t, 0.f, e - b) + b;
	}
	static float BounceInOut(float t, float b, float e) {
		if (t < 0.5f) return BounceIn(t * 2.f, 0.f, e - b) * 0.5f + b;
		else         return BounceOut(t * 2.f - 1.0f, 0.f, e - b) * 0.5f + (e - b) * 0.5f + b;
	}
	static float CircIn(float t, float b, float e) {
		return -(e - b) * (std::sqrt(1 - t * t) - 1) + b;
	}
	static float CircOut(float t, float b, float e) {
		t -= 1.0;
		return (e - b) * std::sqrt(1 - t * t) + b;
	}
	static float CircInOut(float t, float b, float e) {
		if ((t /= 0.5) < 1) return -(e - b) / 2 * (std::sqrt(1 - t * t) - 1) + b;
		t -= 2;
		return (e - b) / 2 * (std::sqrt(1 - t * t) + 1) + b;
	}
	static float CubicIn(float t, float b, float e) {
		return (e - b) * t * t * t + b;
	}
	static float CubicOut(float t, float b, float e) {
		t -= 1.0;
		return (e - b) * (t * t * t + 1) + b;
	}
	static float CubicInOut(float t, float b, float e) {
		if ((t /= 0.5) < 1) return (e - b) / 2 * t * t * t + b;
		t -= 2;
		return (e - b) / 2 * (t * t * t + 2) + b;
	}
	static float ElasticIn(float t, float b, float e) {
		if (t == 0) return b;
		if (t == 1) return e;

		float p = 0.3f;
		float a = e - b;
		float s = p / 4.f;
		t -= 1.f;
		return -(a * std::pow(2.f, 10.f * t) * std::sin((t - s) * (2.f * float(M_PI)) / p)) + b;
	}
	static float ElasticOut(float t, float b, float e) {
		if (t == 0) return b;
		if (t == 1) return e;

		float p = 0.3f;
		float a = e - b;
		float s = p / 4.f;
		return (a * std::pow(2.f, -10.f * t) * std::sin((t - s) * (2.f * float(M_PI)) / p) + a + b);
	}
	static float ElasticInOut(float t, float b, float e) {
		if (t == 0) return b;
		if ((t /= 0.5) == 2) return e;

		float p = 0.3f * 1.5f;
		float a = e - b;
		float s = p / 4.f;
		if (t < 1.f) {
			t -= 1.f;
			return -0.5f * (a * std::pow(2.f, 10.f * t) * std::sin((t - s) * (2.f * float(M_PI)) / p)) + b;
		}
		t -= 1;
		return a * std::pow(2.f, -10.f * t) * std::sin((t - s) * (2.f * float(M_PI)) / p) * 0.5f + a + b;
	}
	static float ExpoIn(float t, float b, float e) {
		return (t == 0) ? b : (e - b) * std::pow(2.f, 10.f * (t - 1.f)) + b;
	}
	static float ExpoOut(float t, float b, float e) {
		return (t == 1.f) ? e : (e - b) * (-std::pow(2.f, -10.f * t) + 1.f) + b;
	}
	static float ExpoInOut(float t, float b, float e) {
		if (t == 0) return b;
		if (t == 1) return e;
		if ((t /= 0.5f) < 1) return (e - b) / 2.f * std::pow(2.f, 10.f * (t - 1.f)) + b;
		return (e - b) / 2.f * (-std::pow(2.f, -10.f* --t) + 2.f) + b;
	}
	static float QuadIn(float t, float b, float e) {
		return (e - b) * t * t + b;
	}
	static float QuadOut(float t, float b, float e) {
		return -(e - b) * t * (t - 2) + b;
	}
	static float QuadInOut(float t, float b, float e) {
		if ((t /= 0.5) < 1) return (e - b) / 2 * t * t + b;
		--t;
		return -(e - b) / 2 * (t * (t - 2) - 1) + b;
	}
	static float QuartIn(float t, float b, float e) {
		return (e - b) * t * t * t * t + b;
	}
	static float QuartOut(float t, float b, float e) {
		t -= 1.0;
		return -(e - b) * (t * t * t * t - 1) + b;
	}
	static float QuartInOut(float t, float b, float e) {
		if ((t /= 0.5) < 1) return (e - b) / 2 * t * t * t * t + b;
		t -= 2;
		return -(e - b) / 2 * (t * t * t * t - 2) + b;
	}
	static float QuintIn(float t, float b, float e) {
		return (e - b) * t * t * t * t * t + b;
	}
	static float QuintOut(float t, float b, float e) {
		t -= 1.0;
		return (e - b) * (t * t * t * t * t + 1) + b;
	}
	static float QuintInOut(float t, float b, float e) {
		if ((t /= 0.5) < 1) return (e - b) / 2 * t * t * t * t * t + b;
		t -= 2;
		return (e - b) / 2 * (t * t * t * t * t + 2) + b;
	}
	static float SineIn(float t, float b, float e) {
		return -(e - b) * std::cos(t * (float() / 2.f)) + (e - b) + b;
	}
	static float SineOut(float t, float b, float e) {
		return (e - b) * std::sin(t * (float(M_PI) / 2.f)) + b;
	}
	static float SineInOut(float t, float b, float e) {
		return -(e - b) / 2.f * (std::cos(float(M_PI) * t) - 1.f) + b;
	}
};

///実際の使い方
/*

c_Easing::apply(動かしたい値, 動かした後の値, EasingFunction::QuadOut,60);
c_Easing::clear(mPos.x); イージングを削除する
c_Easing::isEnd(mPos.x);　イージングが終わったらtrueを返す関数
c_Easing::stop(mPos.x);		イージングを止める
c_Easing::start(mPos.x);	イージングを再開する
*/

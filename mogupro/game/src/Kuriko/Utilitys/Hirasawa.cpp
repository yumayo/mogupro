#include"Hirasawa.h"
#define M_PI 3.14159265359
float EasingLinear(float t, float b, float e) {
	return (e - b) * t + b;
}
float EasingBackIn(float t, float b, float e) {
	float s = 1.70158f;
	return (e - b) * t * t * ((s + 1) * t - s) + b;
}
float EasingBackOut(float t, float b, float e) {
	float s = 1.70158f;
	t -= 1.0f;
	return (e - b) * (t * t * ((s + 1) * t + s) + 1) + b;
}
float EasingBackInOut(float t, float b, float e) {
	float s = 1.70158f * 1.525f;
	if ((t /= 0.5f) < 1.0f) return (e - b) / 2.0f * (t * t * ((s + 1.0f) * t - s)) + b;
	t -= 2;
	return (e - b) / 2.f * (t * t * ((s + 1.f) * t + s) + 2.f) + b;
}
float EasingBounceOut(float t, float b, float e) {
	if (t < (1.f / 2.75f)) {
		return (e - b) * (7.5625 * t * t) + b;
	}
	else if (t < (2.f / 2.75f)) {
		t -= (1.5f / 2.75f);
		return (e - b) * (7.5625 * t * t + 0.75) + b;
	}
	else if (t < (2.5f / 2.75f)) {
		t -= (2.25f / 2.75f);
		return (e - b) * (7.5625 * t * t + 0.9375) + b;
	}
	else {
		t -= (2.625f / 2.75f);
		return (e - b) * (7.5625f * t * t + 0.984375f) + b;
	}
}
float EasingBounceIn(float t, float b, float e) {
	return (e - b) - EasingBounceOut(1.0f - t, 0.f, e - b) + b;
}
float EasingBounceInOut(float t, float b, float e) {
	if (t < 0.5f) return EasingBounceIn(t * 2.f, 0, e - b) * 0.5f + b;
	else         return EasingBounceOut(t * 2.f - 1.0f, 0, e - b) * 0.5f + (e - b) * 0.5f + b;
}
float EasingCircIn(float t, float b, float e) {
	return -(e - b) * (std::sqrt(1.f - t * t) - 1.f) + b;
}
float EasingCircOut(float t, float b, float e) {
	t -= 1.0f;
	return (e - b) * std::sqrt(1.f - t * t) + b;
}
float EasingCircInOut(float t, float b, float e) {
	if ((t /= 0.5f) < 1.f) return -(e - b) / 2.f  * (std::sqrt(1.f - t * t) - 1.f) + b;
	t -= 2.f;
	return (e - b) / 2.f  * (std::sqrt(1.f - t * t) + 1.f) + b;
}
float EasingCubicIn(float t, float b, float e) {
	return (e - b) * t * t * t + b;
}
float EasingCubicOut(float t, float b, float e) {
	t -= 1.0f;
	return (e - b) * (t * t * t + 1.f) + b;
}
float EasingCubicInOut(float t, float b, float e) {
	if ((t /= 0.5f) < 1.f) return (e - b) / 2.f  * t * t * t + b;
	t -= 2.f;
	return (e - b) / 2.f  * (t * t * t + 2.f) + b;
}
float EasingElasticIn(float t, float b, float e) {
	if (t == 0.f) return b;
	if (t == 1.f) return e;

	float p = 0.3f;
	float a = e - b;
	float s = p / 4.f;
	t -= 1.f;
	return -(a * std::pow(2.f, 10.f  * t) * std::sin((t - s) * (2.f  * M_PI) / p)) + b;
}
float EasingElasticOut(float t, float b, float e) {
	if (t == 0) return b;
	if (t == 1) return e;

	float p = 0.3f;
	float a = e - b;
	float s = p / 4.f;
	return (a * std::pow(2.f, -10.f  * t) * std::sin((t - s) * (2.f  * M_PI) / p) + a + b);
}
float EasingElasticInOut(float t, float b, float e) {
	if (t == 0) return b;
	if ((t /= 0.5f) == 2) return e;

	float p = 0.3f * 1.5f;
	float a = e - b;
	float s = p / 4.f;
	if (t < 1) {
		t -= 1;
		return -0.5f * (a * std::pow(2.f, 10.f  * t) * std::sin((t - s) * (2.f  * M_PI) / p)) + b;
	}
	t -= 1.f;
	return a * std::pow(2.f, -10.f  * t) * std::sin((t - s) * (2.f  * M_PI) / p) * 0.5f + a + b;
}
float EasingExpoIn(float t, float b, float e) {
	return (t == 0) ? b : (e - b) * std::pow(2.f, 10.f  * (t - 1)) + b;
}
float EasingExpoOut(float t, float b, float e) {
	return (t == 1) ? e : (e - b) * (-std::pow(2.f, -10.f  * t) + 1) + b;
}
float EasingExpoInOut(float t, float b, float e) {
	if (t == 0) return b;
	if (t == 1) return e;
	if ((t /= 0.5f) < 1) return (e - b) / 2.f * std::pow(2.f, 10.f * (t - 1)) + b;
	return (e - b) / 2.f * (-std::pow(2.f, -10.f * --t) + 2.f) + b;
}
float EasingQuadIn(float t, float b, float e) {
	return (e - b) * t * t + b;
}
float EasingQuadOut(float t, float b, float e) {
	return -(e - b) * t * (t - 2.f) + b;
}
float EasingQuadInOut(float t, float b, float e) {
	if ((t /= 0.5f) < 1) return (e - b) / 2.f * t * t + b;
	--t;
	return -(e - b) / 2.f * (t * (t - 2.f) - 1.f) + b;
}
float EasingQuartIn(float t, float b, float e) {
	return (e - b) * t * t * t * t + b;
}
float EasingQuartOut(float t, float b, float e) {
	t -= 1.0f;
	return -(e - b) * (t * t * t * t - 1.f) + b;
}
float EasingQuartInOut(float t, float b, float e) {
	if ((t /= 0.5f) < 1.f) return (e - b) / 2.f * t * t * t * t + b;
	t -= 2;
	return -(e - b) / 2.f * (t * t * t * t - 2.f) + b;
}
float EasingQuintIn(float t, float b, float e) {
	return (e - b) * t * t * t * t * t + b;
}
float EasingQuintOut(float t, float b, float e) {
	t -= 1.0f;
	return (e - b) * (t * t * t * t * t + 1.f) + b;
}
float EasingQuintInOut(float t, float b, float e) {
	if ((t /= 0.5f) < 1.f) return (e - b) / 2.f * t * t * t * t * t + b;
	t -= 2.f;
	return (e - b) / 2.f * (t * t * t * t * t + 2.f) + b;
}
float EasingSineIn(float t, float b, float e) {
	return -(e - b) * std::cos(t * (M_PI / 2.f)) + (e - b) + b;
}
float EasingSineOut(float t, float b, float e) {
	return (e - b) * std::sin(t * (M_PI / 2.f)) + b;
}
float EasingSineInOut(float t, float b, float e) {
	return -(e - b) / 2.f * (std::cos(M_PI * t) - 1.f) + b;
}
float EasingReturn(float t, float s_pos, float e_size) {
	return s_pos + -4.f*e_size*(t - 0.5f)*(t - 0.5f) + e_size;
}
std::function<float(float, float, float)>Easing::getEasing[] = {
	EasingBackIn,EasingBackInOut,EasingBackOut,
	EasingBounceIn,EasingBounceInOut,EasingBounceOut,
	EasingCircIn,EasingCircInOut,EasingCircOut,
	EasingCubicIn,EasingCubicInOut,EasingCubicOut,
	EasingElasticIn,EasingElasticInOut,EasingElasticOut,
	EasingExpoIn,EasingExpoInOut,EasingExpoOut,
	EasingLinear,
	EasingQuadIn,EasingQuadInOut,EasingQuadOut,
	EasingQuartIn,EasingQuartInOut,EasingQuartOut,
	EasingQuintIn,EasingQuintInOut,EasingQuintOut,
	EasingSineIn,EasingSineInOut,EasingSineOut,
	EasingReturn
};

void Easing::tCount(float & t, float add)
{
	t += 1.0f / (60.0f*add);
	if (t > 1) {
		t = 1.0f;
	}
}

void Easing::tCount(float & t, float add, float max)
{
	t += 1.0f / (60.0f*add);
	if (t > max) {
		t = max;
	}
}

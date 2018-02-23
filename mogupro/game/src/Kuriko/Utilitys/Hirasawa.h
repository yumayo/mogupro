#pragma once
#include<iostream>
#include<functional>

float EasingLinear(float t, float b, float e);

float EasingBackIn(float t, float b, float e);

float EasingBackOut(float t, float b, float e);

float EasingBackInOut(float t, float b, float e);

float EasingBounceOut(float t, float b, float e);

float EasingBounceIn(float t, float b, float e);

float EasingBounceInOut(float t, float b, float e);

float EasingCircIn(float t, float b, float e);

float EasingCircOut(float t, float b, float e);

float EasingCircInOut(float t, float b, float e);

float EasingCubicIn(float t, float b, float e);

float EasingCubicOut(float t, float b, float e);

float EasingCubicInOut(float t, float b, float e);

float EasingElasticIn(float t, float b, float e);

float EasingElasticOut(float t, float b, float e);

float EasingElasticInOut(float t, float b, float e);

float EasingExpoIn(float t, float b, float e);

float EasingExpoOut(float t, float b, float e);

float EasingExpoInOut(float t, float b, float e);

float EasingQuadIn(float t, float b, float e);

float EasingQuadOut(float t, float b, float e);

float EasingQuadInOut(float t, float b, float e);

float EasingQuartIn(float t, float b, float e);

float EasingQuartOut(float t, float b, float e);

float EasingQuartInOut(float t, float b, float e);

float EasingQuintIn(float t, float b, float e);

float EasingQuintOut(float t, float b, float e);

float EasingQuintInOut(float t, float b, float e);

float EasingSineIn(float t, float b, float e);

float EasingSineOut(float t, float b, float e);

float EasingSineInOut(float t, float b, float e);

float EasingReturn(float t, float s_pos, float e_size);
class Easing {
public:
	enum EasingType{
		BackIn, BackInOut, BackOut,
		BounceIn, BounceInOut, BounceOut,
		CircIn, CircInOut, CircOut,
		CubicIn, CubicInOut, CubicOut,
		ElasticIn, ElasticInOut, ElasticOut,
		ExpoIn, ExpoInOut, ExpoOut,
		Linear,
		QuadIn, QuadInOut, QuadOut,
		QuartIn, QuartInOut, QuartOut,
		QuintIn, QuintInOut, QuintOut,
		SineIn, SineInOut, SineOut,
		Return
	};
	static void tCount(float& t, float add);
	static void tCount(float& t, float add,float max);
	static std::function<float(float, float, float)> getEasing[];
};


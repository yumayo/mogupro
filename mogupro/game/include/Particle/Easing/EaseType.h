#pragma once
#include <functional>

enum EaseType
{
    NONE,
    Linear,
    BackIn,
    BackOut,
    BackInOut,
    BounceOut,
    BounceIn,
    BounceInOut,
    CircIn,
    CircOut,
    CircInOut,
    CubicIn,
    CubicOut,
    CubicInOut,
    ElasticIn,
    ElasticOut,
    ElasticInOut,
    ExpoIn,
    ExpoOut,
    ExpoInOut,
    QuadIn,
    QuadOut,
    QuadInOut,
    QuartIn,
    QuartOut,
    QuartInOut,
    QuintIn,
    QuintOut,
    QuintInOut,
    SineIn,
    SineOut,
    SineInOut,
};

std::function<float( float, float, float )> getEaseFunc( EaseType ease_type );

float EaseLinear( float t, float b, float e );
float EaseBackIn( float t, float b, float e );
float EaseBackOut( float t, float b, float e );
float EaseBackInOut( float t, float b, float e );
float EaseBounceOut( float t, float b, float e );
float EaseBounceIn( float t, float b, float e );
float EaseBounceInOut( float t, float b, float e );
float EaseCircIn( float t, float b, float e );
float EaseCircOut( float t, float b, float e );
float EaseCircInOut( float t, float b, float e );
float EaseCubicIn( float t, float b, float e );
float EaseCubicOut( float t, float b, float e );
float EaseCubicInOut( float t, float b, float e );
float EaseElasticIn( float t, float b, float e );
float EaseElasticOut( float t, float b, float e );
float EaseElasticInOut( float t, float b, float e );
float EaseExpoIn( float t, float b, float e );
float EaseExpoOut( float t, float b, float e );
float EaseExpoInOut( float t, float b, float e );
float EaseQuadIn( float t, float b, float e );
float EaseQuadOut( float t, float b, float e );
float EaseQuadInOut( float t, float b, float e );
float EaseQuartIn( float t, float b, float e );
float EaseQuartOut( float t, float b, float e );
float EaseQuartInOut( float t, float b, float e );
float EaseQuintIn( float t, float b, float e );
float EaseQuintOut( float t, float b, float e );
float EaseQuintInOut( float t, float b, float e );
float EaseSineIn( float t, float b, float e );
float EaseSineOut( float t, float b, float e );
float EaseSineInOut( float t, float b, float e );
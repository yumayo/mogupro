#include "Particle/Easing/EaseType.h"
#include <cinder/CinderMath.h>

std::function<float( float, float, float )> getEaseFunc( EaseType ease_type )
{
    switch ( ease_type )
    {
        case NONE:
            return nullptr;
        case Linear:
            return EaseLinear;
        case BackIn:
            return EaseBackIn;
        case BackOut:
            return EaseBackOut;
        case BackInOut:
            return EaseBackInOut;
        case BounceOut:
            return EaseBounceOut;
        case BounceIn:
            return EaseBounceIn;
        case BounceInOut:
            return EaseBounceInOut;
        case CircIn:
            return EaseCircIn;
        case CircOut:
            return EaseCircOut;
        case CircInOut:
            return EaseCircInOut;
        case CubicIn:
            return EaseCubicIn;
        case CubicOut:
            return EaseCubicOut;
        case CubicInOut:
            return EaseCubicInOut;
        case ElasticIn:
            return EaseElasticIn;
        case ElasticOut:
            return EaseElasticOut;
        case ElasticInOut:
            return EaseElasticInOut;
        case ExpoIn:
            return EaseExpoIn;
        case ExpoOut:
            return EaseExpoOut;
        case ExpoInOut:
            return EaseExpoInOut;
        case QuadIn:
            return EaseQuadIn;
        case QuadOut:
            return EaseQuadOut;
        case QuadInOut:
            return EaseQuadInOut;
        case QuartIn:
            return EaseQuartIn;
        case QuartOut:
            return EaseQuartOut;
        case QuartInOut:
            return EaseQuartInOut;
        case QuintIn:
            return EaseQuintIn;
        case QuintOut:
            return EaseQuintOut;
        case QuintInOut:
            return EaseQuintInOut;
        case SineIn:
            return EaseSineIn;
        case SineOut:
            return EaseSineOut;
        case SineInOut:
            return EaseSineInOut;
    }
}

float EaseLinear( float t, float b, float e )
{
    return ( e - b ) * t + b;

}
float EaseBackIn( float t, float b, float e )
{
    float s = 1.70158f;
    return ( e - b ) * t * t * ( ( s + 1 ) * t - s ) + b;

}
float EaseBackOut( float t, float b, float e )
{
    float s = 1.70158f;
    t -= 1.0;
    return ( e - b ) * ( t * t * ( ( s + 1 ) * t + s ) + 1 ) + b;

}
float EaseBackInOut( float t, float b, float e )
{
    float s = 1.70158f * 1.525f;
    if ( ( t /= 0.5 ) < 1 )
        return ( e - b ) / 2 * ( t * t * ( ( s + 1 ) * t - s ) ) + b;
    t -= 2;
    return ( e - b ) / 2 * ( t * t * ( ( s + 1 ) * t + s ) + 2 ) + b;
}
float EaseBounceOut( float t, float b, float e )
{
    if ( t < ( 1 / 2.75f ) )
    {
        return ( e - b ) * ( 7.5625f * t * t ) + b;
    }
    else if ( t < ( 2 / 2.75f ) )
    {
        t -= ( 1.5f / 2.75f );
        return ( e - b ) * ( 7.5625f * t * t + 0.75f ) + b;
    }
    else if ( t < ( 2.5 / 2.75 ) )
    {
        t -= ( 2.25f / 2.75f );
        return ( e - b ) * ( 7.5625f * t * t + 0.9375f ) + b;
    }
    else
    {
        t -= ( 2.625f / 2.75f );
        return ( e - b ) * ( 7.5625f * t * t + 0.984375f ) + b;
    }
}
float EaseBounceIn( float t, float b, float e )
{
    return ( e - b ) - EaseBounceOut( 1.0f - t, 0.f, e - b ) + b;
}
float EaseBounceInOut( float t, float b, float e )
{
    if ( t < 0.5f )
        return EaseBounceIn( t * 2.f, 0.f, e - b ) * 0.5f + b;
    else
        return EaseBounceOut( t * 2.f - 1.0f, 0.f, e - b ) * 0.5f + ( e - b ) * 0.5f + b;
}
float EaseCircIn( float t, float b, float e )
{
    return -( e - b ) * ( std::sqrt( 1 - t * t ) - 1 ) + b;
}
float EaseCircOut( float t, float b, float e )
{
    t -= 1.0;
    return ( e - b ) * std::sqrt( 1 - t * t ) + b;
}
float EaseCircInOut( float t, float b, float e )
{
    if ( ( t /= 0.5 ) < 1 )
        return -( e - b ) / 2 * ( std::sqrt( 1 - t * t ) - 1 ) + b;
    t -= 2;
    return ( e - b ) / 2 * ( std::sqrt( 1 - t * t ) + 1 ) + b;
}
float EaseCubicIn( float t, float b, float e )
{
    return ( e - b ) * t * t * t + b;
}
float EaseCubicOut( float t, float b, float e )
{
    t -= 1.0;
    return ( e - b ) * ( t * t * t + 1 ) + b;
}
float EaseCubicInOut( float t, float b, float e )
{
    if ( ( t /= 0.5 ) < 1 )
        return ( e - b ) / 2 * t * t * t + b;
    t -= 2;
    return ( e - b ) / 2 * ( t * t * t + 2 ) + b;
}
float EaseElasticIn( float t, float b, float e )
{
    if ( t == 0 ) return b;
    if ( t == 1 ) return e;

    float p = 0.3f;
    float a = e - b;
    float s = p / 4.f;
    t -= 1.f;
    return -( a * std::pow( 2.f, 10.f * t ) * std::sin( ( t - s ) * ( 2.f * float( M_PI ) ) / p ) ) + b;
}
float EaseElasticOut( float t, float b, float e )
{
    if ( t == 0 ) return b;
    if ( t == 1 ) return e;

    float p = 0.3f;
    float a = e - b;
    float s = p / 4.f;
    return ( a * std::pow( 2.f, -10.f * t ) * std::sin( ( t - s ) * ( 2.f * float( M_PI ) ) / p ) + a + b );
}
float EaseElasticInOut( float t, float b, float e )
{
    if ( t == 0 ) return b;
    if ( ( t /= 0.5 ) == 2 ) return e;

    float p = 0.3f * 1.5f;
    float a = e - b;
    float s = p / 4.f;
    if ( t < 1.f )
    {
        t -= 1.f;
        return -0.5f * ( a * std::pow( 2.f, 10.f * t ) * std::sin( ( t - s ) * ( 2.f * float( M_PI ) ) / p ) ) + b;
    }
    t -= 1;
    return a * std::pow( 2.f, -10.f * t ) * std::sin( ( t - s ) * ( 2.f * float( M_PI ) ) / p ) * 0.5f + a + b;
}
float EaseExpoIn( float t, float b, float e )
{
    return ( t == 0 ) ? b : ( e - b ) * std::pow( 2.f, 10.f * ( t - 1.f ) ) + b;
}
float EaseExpoOut( float t, float b, float e )
{
    return ( t == 1.f ) ? e : ( e - b ) * ( -std::pow( 2.f, -10.f * t ) + 1.f ) + b;
}
float EaseExpoInOut( float t, float b, float e )
{
    if ( t == 0 ) return b;
    if ( t == 1 ) return e;
    if ( ( t /= 0.5f ) < 1 )
        return ( e - b ) / 2.f * std::pow( 2.f, 10.f * ( t - 1.f ) ) + b;
    return ( e - b ) / 2.f * ( -std::pow( 2.f, -10.f* --t ) + 2.f ) + b;
}
float EaseQuadIn( float t, float b, float e )
{
    return ( e - b ) * t * t + b;
}
float EaseQuadOut( float t, float b, float e )
{
    return -( e - b ) * t * ( t - 2 ) + b;
}
float EaseQuadInOut( float t, float b, float e )
{
    if ( ( t /= 0.5 ) < 1 )
        return ( e - b ) / 2 * t * t + b;
    --t;
    return -( e - b ) / 2 * ( t * ( t - 2 ) - 1 ) + b;
}
float EaseQuartIn( float t, float b, float e )
{
    return ( e - b ) * t * t * t * t + b;
}
float EaseQuartOut( float t, float b, float e )
{
    t -= 1.0;
    return -( e - b ) * ( t * t * t * t - 1 ) + b;
}
float EaseQuartInOut( float t, float b, float e )
{
    if ( ( t /= 0.5 ) < 1 )
        return ( e - b ) / 2 * t * t * t * t + b;
    t -= 2;
    return -( e - b ) / 2 * ( t * t * t * t - 2 ) + b;
}
float EaseQuintIn( float t, float b, float e )
{
    return ( e - b ) * t * t * t * t * t + b;
}
float EaseQuintOut( float t, float b, float e )
{
    t -= 1.0;
    return ( e - b ) * ( t * t * t * t * t + 1 ) + b;
}
float EaseQuintInOut( float t, float b, float e )
{
    if ( ( t /= 0.5 ) < 1 )
        return ( e - b ) / 2 * t * t * t * t * t + b;
    t -= 2;
    return ( e - b ) / 2 * ( t * t * t * t * t + 2 ) + b;
}
float EaseSineIn( float t, float b, float e )
{
    return -( e - b ) * std::cos( t * ( float( M_PI ) / 2.f ) ) + ( e - b ) + b;
}
float EaseSineOut( float t, float b, float e )
{
    return ( e - b ) * std::sin( t * ( float( M_PI ) / 2.f ) ) + b;
}
float EaseSineInOut( float t, float b, float e )
{
    return -( e - b ) / 2.f * ( std::cos( float( M_PI ) * t ) - 1.f ) + b;
}
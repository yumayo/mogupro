#pragma once
#include <cinder/Vector.h>
#include <Utility/cUserPointer.hpp>
namespace Game
{
namespace Light
{
class cLineLightParam
{
public:
	cLineLightParam( cinder::vec3 beginPosition, cinder::vec3 endPosition, cinder::vec3 color, float radius )
		: beginPosition( beginPosition )
		, endPosition( endPosition )
		, color( color )
		, radius( radius )
	{
	}
public:
	void reAttachLine( Utility::softptr<cLineLightParam> handle, cinder::vec3 beginPosition, cinder::vec3 endPosition );
	void reAttachRadius( Utility::softptr<cLineLightParam> handle, float radius );
	void reAttachLineWithRadius( Utility::softptr<cLineLightParam> handle, cinder::vec3 beginPosition, cinder::vec3 endPosition, float radius );
	inline cinder::vec3 getBeginPosition( )
	{
		return beginPosition;
	}
	inline cinder::vec3 getEndPosition( )
	{
		return endPosition;
	}
	inline float getRadius( )
	{
		return radius;
	}
private:
	cinder::vec3 beginPosition;
	cinder::vec3 endPosition;
	float radius;
public:
	cinder::vec3 color;
};
using LineLightHandle = Utility::softptr<cLineLightParam>;
}
}

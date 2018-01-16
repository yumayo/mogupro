#pragma once
#include <memory>
#include <cinder/Vector.h>
#include <Utility/cUserPointer.hpp>
namespace Game
{
namespace Light
{
class cLineLightParam
{
public:
	cLineLightParam( int id, cinder::vec3 beginPosition, cinder::vec3 endPosition, cinder::vec3 color, float radius )
		: id( id )
		, beginPosition( beginPosition )
		, endPosition( endPosition )
		, color( color )
		, radius( radius )
	{
	}
	~cLineLightParam( );
public:
	inline int getId( ) const
	{
		return id;
	}
	void reAttachLine( cinder::vec3 beginPosition, cinder::vec3 endPosition );
	void reAttachRadius( float radius );
	void reAttachLineWithRadius( cinder::vec3 beginPosition, cinder::vec3 endPosition, float radius );
	inline cinder::vec3 getBeginPosition( ) const
	{
		return beginPosition;
	}
	inline cinder::vec3 getEndPosition( ) const
	{
		return endPosition;
	}
	inline float getRadius( ) const
	{
		return radius;
	}
private:
	cinder::vec3 beginPosition;
	cinder::vec3 endPosition;
	float radius;
public:
	cinder::vec3 color;
private:
	int id;
};
using LineLightHandle = Utility::hardptr<cLineLightParam>;
}
}

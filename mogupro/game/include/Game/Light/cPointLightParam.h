#pragma once
#include <memory>
#include <cinder/Vector.h>
#include <Utility/cUserPointer.hpp>
namespace Game
{
namespace Light
{
class cPointLightParam
{
public:
	cPointLightParam( int id, cinder::vec3 position, cinder::vec3 color, float radius )
		: id( id )
		, position( position )
		, color( color )
		, radius( radius )
	{
	}
	~cPointLightParam( );
public:
	inline int getId( ) const
	{
		return id;
	}
	void reAttachPosition( cinder::vec3 position );
	void reAttachRadius( float radius );
	void reAttachPositionWithRadius( cinder::vec3 position, float radius );
	inline cinder::vec3 getPosition( ) const
	{
		return position;
	}
	inline float getRadius( ) const
	{
		return radius;
	}
private:
	cinder::vec3 position;
	float radius;
public:
	cinder::vec3 color;
private:
	int id;
};
using PointLightHandle = Utility::hardptr<cPointLightParam>;
}
}

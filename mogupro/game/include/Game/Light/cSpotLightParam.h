#pragma once
#include <memory>
#include <cinder/Vector.h>
#include <Utility/cUserPointer.hpp>
namespace Game
{
namespace Light
{
class cSpotLightParam
{
public:
	cSpotLightParam( int id, cinder::vec3 position, cinder::vec3 direction, cinder::vec3 color, float radius )
		: id( id )
		, position( position )
		, direction( direction )
		, color( color )
		, radius( radius )
	{
	}
	~cSpotLightParam( );
public:
	inline int getId( ) const
	{
		return id;
	}
	void reAttachPosition( cinder::vec3 position );
	void reAttachDirection( cinder::vec3 direction );
	void reAttachRadius( float radius );
	void reAttachPositionWithDirection( cinder::vec3 position, cinder::vec3 direction );
	void reAttachPositionWithRadius( cinder::vec3 position, float radius );
	void reAttachDirectionWithRadius( cinder::vec3 direction, float radius );
	void reAttachPositionWithDirectionWithRadius( cinder::vec3 position, cinder::vec3 direction, float radius );
	inline cinder::vec3 getPosition( ) const
	{
		return position;
	}
	inline cinder::vec3 getDirection( ) const
	{
		return direction;
	}
	inline float getRadius( ) const
	{
		return radius;
	}
private:
	cinder::vec3 position;
	cinder::vec3 direction;
	float radius;
public:
	cinder::vec3 color;
private:
	int id;
};
using SpotLightHandle = Utility::hardptr<cSpotLightParam>;
}
}

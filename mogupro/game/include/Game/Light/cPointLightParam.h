#pragma once
#include <memory>
#include <cinder/Vector.h>
#include <Utility/cUserPointer.hpp>
namespace Game
{
namespace Light
{
class cPointLightParam : std::enable_shared_from_this<cPointLightParam>
{
public:
	cPointLightParam( cinder::vec3 position, cinder::vec3 color, float radius )
		: position( position )
		, color( color )
		, radius( radius )
		, alreadyRemove( false )
	{
	}
	~cPointLightParam( );
public:
	void reAttachPosition( Utility::softptr<cPointLightParam> handle, cinder::vec3 position );
	void reAttachRadius( Utility::softptr<cPointLightParam> handle, float radius );
	void reAttachPositionWithRadius( Utility::softptr<cPointLightParam> handle, cinder::vec3 position, float radius );
	inline cinder::vec3 getPosition( )
	{
		return position;
	}
	inline float getRadius( )
	{
		return radius;
	}
private:
	cinder::vec3 position;
	float radius;
public:
	cinder::vec3 color;
private:
	bool alreadyRemove;
};
using PointLightHandle = Utility::hardptr<cPointLightParam>;
using PointLightSoftHandle = Utility::softptr<cPointLightParam>;
}
}

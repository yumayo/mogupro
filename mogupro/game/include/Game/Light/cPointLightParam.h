#pragma once
#include <cinder/Vector.h>
namespace Game
{
namespace Light
{
class cPointLightParam
{
public:
    cPointLightParam( cinder::vec3 position, cinder::vec3 color, float radius )
		: color( color )
		, radius( radius )
	{
		attachPosition( position );
	}
public:
	inline void attachPosition( cinder::vec3 position )
	{
		this->position = position;
	}
	inline cinder::vec3 getPosition( )
	{
		return position;
	}
private:
	cinder::vec3 position;
public:
	cinder::vec3 color;
	float radius;
};
}
}

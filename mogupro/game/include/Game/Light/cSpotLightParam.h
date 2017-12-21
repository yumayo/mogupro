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
		cSpotLightParam(ci::vec3 position, ci::vec3 direction, ci::vec3 color, float angle)
			: position(position)
			, direction(direction)
			, color(color)
			, angle(angle)
		{
		};
    ~cSpotLightParam();
private:
	ci::vec3 position;
	ci::vec3 direction;
	float angle;
public:
	ci::vec3 color;
private:
	int id;
};
	using SpotLightHandle = Utility::hardptr<cSpotLightParam>;
}
}

#pragma once
#include <Utility/cSingletonAble.h>
#include <cinder/Vector.h>
#include <map>
#include <vector>
#include <boost/optional.hpp>
namespace Game
{
struct PointLightParam
{
	PointLightParam( cinder::vec3 position, cinder::vec3 color, float radius )
		: position( position )
		, color( color )
		, radius( radius )
	{

	}
	cinder::vec3 position;
	cinder::vec3 color;
	float radius;
};
class cLightManager : public Utility::cSingletonAble<cLightManager>
{
public:
	cLightManager( ) = default;
	~cLightManager( ) = default;
	void setup( );
	void update( );
	boost::optional<PointLightParam&> getLight( unsigned int handle );
	std::map<unsigned int, PointLightParam> const& getPointLights( ) const;
	unsigned int addPointLight( cinder::vec3 position, cinder::vec3 color, float radius );
	void removePointLight( unsigned int handle );
private:
	std::map<unsigned int, PointLightParam> mPointLights;

	// 後にプレイヤーマネージャーの方でやって貰う予定。
	std::vector<unsigned int> mPointLightHandles;
};
}

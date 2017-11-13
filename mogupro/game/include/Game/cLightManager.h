#pragma once
#include <Utility/cSingletonAble.h>
#include <cinder/Vector.h>
#include <set>
#include <vector>
#include <Utility/cUserPointer.hpp>
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
	std::set<Utility::hardptr<PointLightParam>> const& getPointLights( ) const;
	Utility::softptr<PointLightParam> addPointLight( cinder::vec3 position, cinder::vec3 color, float radius );
	void removePointLight( Utility::softptr<PointLightParam> handle );
private:
	std::set<Utility::hardptr<PointLightParam>> mPointLights;

	// 後にプレイヤーマネージャーの方でやって貰う予定。
	std::vector<Utility::softptr<PointLightParam>> mPointLightHandles;
};
}

#pragma once
#include <Utility/cSingletonAble.h>
#include <cinder/Vector.h>
#include <map>
#include <set>
#include <vector>
#include <Utility/cUserPointer.hpp>
#include <Game/Light/cPointLightParam.h>
namespace Game
{
class cLightManager : public Utility::cSingletonAble<cLightManager>
{
public:
	cLightManager( ) = default;
	~cLightManager( ) = default;
	void setup( );
	void update( );
	std::set<Utility::hardptr<Light::cPointLightParam>> const& getPointLights( ) const;
	Utility::softptr<Light::cPointLightParam> addPointLight( cinder::vec3 position, cinder::vec3 color, float radius );
	void removePointLight( Utility::softptr<Light::cPointLightParam> handle );
private:
	std::set<Utility::hardptr<Light::cPointLightParam>> mPointLights;
	std::map<int, std::set<Utility::softptr<Light::cPointLightParam>>> mPointLightsMap;
private:
	void reAppend( int chunkId, Utility::softptr<Light::cPointLightParam> handle );

	// 後にプレイヤーマネージャーの方でやって貰う予定。
	std::vector<Utility::softptr<Light::cPointLightParam>> mPointLightHandles;
};
}

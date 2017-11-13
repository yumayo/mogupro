#pragma once
#include <Utility/cSingletonAble.h>
#include <cinder/Vector.h>
#include <map>
#include <set>
#include <vector>
#include <Utility/cUserPointer.hpp>
#include <Game/Light/cPointLightParam.h>
#include <boost/optional.hpp>
namespace Game
{
class cLightManager : public Utility::cSingletonAble<cLightManager>
{
public:
	cLightManager( ) = default;
	~cLightManager( ) = default;
	void setup( );
	void update( );

	std::set<Utility::hardptr<Light::cPointLightParam>>const& getPointLights( ) const;
	boost::optional<std::set<Utility::softptr<Light::cPointLightParam>>const&> getPointLights( int chunkId ) const;
	Utility::softptr<Light::cPointLightParam> addPointLight( cinder::vec3 position, cinder::vec3 color, float radius );
	void removePointLight( Utility::softptr<Light::cPointLightParam> handle );
	void attachChunk( Utility::softptr<Light::cPointLightParam> handle );
	void detachChunk( Utility::softptr<Light::cPointLightParam> handle );
private:
	std::set<Utility::hardptr<Light::cPointLightParam>> mPointLights;
	std::map<int, std::set<Utility::softptr<Light::cPointLightParam>>> mPointLightsMap;
private:
	std::vector<Utility::softptr<Light::cPointLightParam>> mPointLightHandles;
};
}

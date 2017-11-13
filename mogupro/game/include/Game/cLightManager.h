#pragma once
#include <Utility/cSingletonAble.h>
#include <cinder/Vector.h>
#include <map>
#include <set>
#include <vector>
#include <Utility/cUserPointer.hpp>
#include <Game/Light/cPointLightParam.h>
#include <Game/Light/cLineLightParam.h>
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

public: // ポイントライト
	std::set<Utility::hardptr<Light::cPointLightParam>>const& getPointLights( ) const;
	boost::optional<std::set<Utility::softptr<Light::cPointLightParam>>const&> getPointLights( int chunkId ) const;
	Utility::softptr<Light::cPointLightParam> addPointLight( cinder::vec3 position, cinder::vec3 color, float radius );
	void removePointLight( Utility::softptr<Light::cPointLightParam> handle );
	void attachChunk( Utility::softptr<Light::cPointLightParam> handle );
	void detachChunk( Utility::softptr<Light::cPointLightParam> handle );
private:
	std::set<Utility::hardptr<Light::cPointLightParam>> mPointLights;
	std::map<int, std::set<Utility::softptr<Light::cPointLightParam>>> mPointLightsMap;

public: // ラインライト
	std::set<Utility::hardptr<Light::cLineLightParam>>const& getLineLights( ) const;
	boost::optional<std::set<Utility::softptr<Light::cLineLightParam>>const&> getLineLights( int chunkId ) const;
	Utility::softptr<Light::cLineLightParam> addLineLight( cinder::vec3 beginPosition, cinder::vec3 endPosition, cinder::vec3 color, float radius );
	void removeLineLight( Utility::softptr<Light::cLineLightParam> handle );
	void attachChunk( Utility::softptr<Light::cLineLightParam> handle );
	void detachChunk( Utility::softptr<Light::cLineLightParam> handle );
private:
	std::set<Utility::hardptr<Light::cLineLightParam>> mLineLights;
	std::map<int, std::set<Utility::softptr<Light::cLineLightParam>>> mLineLightsMap;

private: // プレイヤー更新用。
	std::vector<Utility::softptr<Light::cPointLightParam>> mPointLightHandles;
	std::vector<cinder::vec3> mPlayerPositionBuffer;
	std::vector<Utility::softptr<Light::cLineLightParam>> mLineLightHandles;
};
}

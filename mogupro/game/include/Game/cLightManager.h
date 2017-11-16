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
	boost::optional<std::set<Light::PointLightHandle>const&> getPointLights( int chunkId ) const;
	Light::PointLightHandle addPointLight( cinder::vec3 position, cinder::vec3 color, float radius );
	void removePointLight( Light::PointLightHandle handle );
	void attachChunk( Light::PointLightHandle handle );
	void detachChunk( Light::PointLightHandle handle );
private:
	std::set<Utility::hardptr<Light::cPointLightParam>> mPointLights;
	std::map<int, std::set<Light::PointLightHandle>> mPointLightsMap;

public: // ラインライト
	std::set<Utility::hardptr<Light::cLineLightParam>>const& getLineLights( ) const;
	boost::optional<std::set<Light::LineLightHandle>const&> getLineLights( int chunkId ) const;
	Light::LineLightHandle addLineLight( cinder::vec3 beginPosition, cinder::vec3 endPosition, cinder::vec3 color, float radius );
	void removeLineLight( Light::LineLightHandle handle );
	void attachChunk( Light::LineLightHandle handle );
	void detachChunk( Light::LineLightHandle handle );
private:
	std::set<Utility::hardptr<Light::cLineLightParam>> mLineLights;
	std::map<int, std::set<Utility::softptr<Light::cLineLightParam>>> mLineLightsMap;

private: // プレイヤー更新用。
	std::vector<Light::PointLightHandle> mPointLightHandles;
	std::vector<cinder::vec3> mPlayerPositionBuffer;
	std::vector<Light::LineLightHandle> mLineLightHandles;
};
}

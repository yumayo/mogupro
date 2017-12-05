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
	friend class Light::cPointLightParam;
	std::set<Light::PointLightHandle>const& getPointLights( ) const;
	boost::optional<std::set<Light::PointLightSoftHandle>const&> getPointLights( int chunkId ) const;
	Light::PointLightHandle addPointLight( cinder::vec3 position, cinder::vec3 color, float radius );
private:
	void removePointLight( Light::PointLightHandle handle );
	void attachChunk( Light::PointLightHandle handle );
	void detachChunk( Light::PointLightHandle handle );
private:
	std::set<Light::PointLightHandle> mPointLights;
	std::map<int, std::set<Light::PointLightSoftHandle>> mPointLightsMap;

public: // ラインライト
	friend class Light::cLineLightParam;
	std::set<Light::LineLightHandle>const& getLineLights( ) const;
	boost::optional<std::set<Light::LineLightSoftHandle>const&> getLineLights( int chunkId ) const;
	Light::LineLightHandle addLineLight( cinder::vec3 beginPosition, cinder::vec3 endPosition, cinder::vec3 color, float radius );
private:
	void removeLineLight( Light::LineLightHandle handle );
	void attachChunk( Light::LineLightHandle handle );
	void detachChunk( Light::LineLightHandle handle );

private:
	std::set<Light::LineLightHandle> mLineLights;
	std::map<int, std::set<Light::LineLightSoftHandle>> mLineLightsMap;

private: // プレイヤー更新用。
	std::vector<Light::PointLightHandle> mPointLightHandles;
	std::vector<cinder::vec3> mPlayerPositionBuffer;
	std::vector<Light::LineLightHandle> mLineLightHandles;
};
}

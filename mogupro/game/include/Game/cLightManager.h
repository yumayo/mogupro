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
#include <Utility/cIdentifier.h>
#include <Game/Light/LightDefine.h>
namespace Game
{
class cLightManager : public Utility::cSingletonAble<cLightManager>
{
public:
	cLightManager( );
	~cLightManager( ) = default;
	void setup( );
	void update( );

public: // ポイントライト
	friend class Light::cPointLightParam;
	std::map<int, Light::cPointLightParam const*>const& getPointLights( ) const;
	boost::optional<std::set<Light::cPointLightParam const*>const&> getPointLights( int chunkId ) const;
	Light::PointLightHandle addPointLight( cinder::vec3 position, cinder::vec3 color, float radius );
private:
	void removePointLight( int id, Light::cPointLightParam const* param );
	void attachChunk( Light::cPointLightParam const* param );
	void detachChunk( Light::cPointLightParam const* param );
private:
	Utility::cIdentifier mPointLightIdGenerator;
	std::map<int, Light::cPointLightParam const*> mPointLights;
	std::map<int, std::set<Light::cPointLightParam const*>> mPointLightsMap;

public: // ラインライト
	friend class Light::cLineLightParam;
	std::map<int, Light::cLineLightParam const*>const& getLineLights( ) const;
	boost::optional<std::set<Light::cLineLightParam const*>const&> getLineLights( int chunkId ) const;
	Light::LineLightHandle addLineLight( cinder::vec3 beginPosition, cinder::vec3 endPosition, cinder::vec3 color, float radius );
private:
	void removeLineLight( int id, Light::cLineLightParam const* param );
	void attachChunk( Light::cLineLightParam const* param );
	void detachChunk( Light::cLineLightParam const* param );
private:
	Utility::cIdentifier mLineLightIdGenerator;
	std::map<int, Light::cLineLightParam const*> mLineLights;
	std::map<int, std::set<Light::cLineLightParam const*>> mLineLightsMap;
};
}

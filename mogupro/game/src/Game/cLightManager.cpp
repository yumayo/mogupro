#include <Game/cLightManager.h>
#include <Game/cGemManager.h>
#include <Game/cPlayerManager.h>
#include <Game/cFieldManager.h>
#include <random>
namespace Game
{
void cLightManager::setup( )
{
}
void cLightManager::update( )
{
}
#pragma region ポイントライト
std::map<int, Light::cPointLightParam const*> const & cLightManager::getPointLights( ) const
{
	return mPointLights;
}
boost::optional<std::set<Light::cPointLightParam const*> const&> cLightManager::getPointLights( int chunkId ) const
{
	auto itr = mPointLightsMap.find( chunkId );
	if ( itr != mPointLightsMap.end( ) )
	{
		return itr->second;
	}
	else
	{
		return boost::none;
	}
}
Light::PointLightHandle cLightManager::addPointLight( cinder::vec3 position, cinder::vec3 color, float radius )
{
	int id = mPointLightId += 1;
	auto handle = std::make_shared<Light::cPointLightParam>( id, position, color, radius );
	mPointLights.insert( std::make_pair( id, handle.get( ) ) );
	attachChunk( handle.get( ) );
	return handle;
}
void cLightManager::removePointLight( int id, Light::cPointLightParam const* param )
{
	detachChunk( param );
	mPointLights.erase( id );
}
void cLightManager::attachChunk( Light::cPointLightParam const* param )
{
	for ( int id : cFieldManager::getInstance( )->getChunkId( param->getPosition( ), param->getRadius( ) ) )
	{
		mPointLightsMap[id].insert( param );
	}
}
void cLightManager::detachChunk( Light::cPointLightParam const* param )
{
	for ( int id : cFieldManager::getInstance( )->getChunkId( param->getPosition( ), param->getRadius( ) ) )
	{
		mPointLightsMap[id].erase( param );
	}
}
#pragma endregion
#pragma region ラインライト
std::map<int, Light::cLineLightParam const*> const & cLightManager::getLineLights( ) const
{
	return mLineLights;
}
boost::optional<std::set<Light::cLineLightParam const*>const&> cLightManager::getLineLights( int chunkId ) const
{
	auto itr = mLineLightsMap.find( chunkId );
	if ( itr != mLineLightsMap.end( ) )
	{
		return itr->second;
	}
	else
	{
		return boost::none;
	}
}
Light::LineLightHandle cLightManager::addLineLight( cinder::vec3 beginPosition, cinder::vec3 endPosition, cinder::vec3 color, float radius )
{
	int id = mLineLightId += 1;
	auto handle = std::make_shared<Light::cLineLightParam>( id, beginPosition, endPosition, color, radius );
	mLineLights.insert( std::make_pair( id, handle.get( ) ) );
	attachChunk( handle.get( ) );
	return handle;
}
void cLightManager::removeLineLight( int id, Light::cLineLightParam const* param )
{
	detachChunk( mLineLights[id] );
	mLineLights.erase( id );
}
void cLightManager::attachChunk( Light::cLineLightParam const* param )
{
	// 始点を判定に使った簡易なもの。
	for ( int id : cFieldManager::getInstance( )->getChunkId( param->getBeginPosition( ), param->getRadius( ) ) )
	{
		mLineLightsMap[id].insert( param );
	}
}
void cLightManager::detachChunk( Light::cLineLightParam const* param )
{
	for ( int id : cFieldManager::getInstance( )->getChunkId( param->getBeginPosition( ), param->getRadius( ) ) )
	{
		mLineLightsMap[id].erase( param );
	}
}
#pragma endregion
}

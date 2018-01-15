#include <Game/cLightManager.h>
#include <Game/cGemManager.h>
#include <Game/cPlayerManager.h>
#include <Game/cFieldManager.h>
#include <random>
namespace Game
{
cLightManager::cLightManager( )
	: mPointLightIdGenerator( 0, Light::MAX_POINT_LIGHT_NUM )
	, mLineLightIdGenerator( 0, Light::MAX_LINE_LIGHT_NUM )
{
}
void cLightManager::setup( )
{
}
void cLightManager::update( )
{
	{
		auto it = mPointLightsMap.cbegin( );
		while ( it != mPointLightsMap.cend( ) )
		{
			if ( it->second.empty( ) )
			{
				mPointLightsMap.erase( it++ );
			}
			else
			{
				++it;
			}
		}
	}
	{
		auto it = mLineLightsMap.cbegin( );
		while ( it != mLineLightsMap.cend( ) )
		{
			if ( it->second.empty( ) )
			{
				mLineLightsMap.erase( it++ );
			}
			else
			{
				++it;
			}
		}
	}
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
	const int id = mPointLightIdGenerator.createId( );
	auto handle = std::make_shared<Light::cPointLightParam>( id, position, color, radius );
	mPointLights.insert( std::make_pair( id, handle.get( ) ) );
	attachChunk( handle.get( ) );
	return handle;
}
void cLightManager::removePointLight( int id, Light::cPointLightParam const* param )
{
	mPointLightIdGenerator.removeId( id );
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
	const int id = mLineLightIdGenerator.createId( );
	auto handle = std::make_shared<Light::cLineLightParam>( id, beginPosition, endPosition, color, radius );
	mLineLights.insert( std::make_pair( id, handle.get( ) ) );
	attachChunk( handle.get( ) );
	return handle;
}
void cLightManager::removeLineLight( int id, Light::cLineLightParam const* param )
{
	mLineLightIdGenerator.removeId( id );
	detachChunk( mLineLights[id] );
	mLineLights.erase( id );
}
void cLightManager::attachChunk( Light::cLineLightParam const* param )
{
	auto direction = param->getEndPosition( ) - param->getBeginPosition( );
	direction = glm::normalize( direction );
	auto distance = glm::distance( param->getBeginPosition( ), param->getEndPosition( ) );
	auto center = param->getBeginPosition( ) + direction * distance * 0.5F;
	for ( int id : cFieldManager::getInstance( )->getChunkId( center, param->getRadius( ) * 2.0F + distance * 0.5F ) )
	{
		mLineLightsMap[id].insert( param );
	}
}
void cLightManager::detachChunk( Light::cLineLightParam const* param )
{
	auto direction = param->getEndPosition( ) - param->getBeginPosition( );
	direction = glm::normalize( direction );
	auto distance = glm::distance( param->getBeginPosition( ), param->getEndPosition( ) );
	auto center = param->getBeginPosition( ) + direction * distance * 0.5F;
	for ( int id : cFieldManager::getInstance( )->getChunkId( center, param->getRadius( ) * 2.0F + distance * 0.5F ) )
	{
		mLineLightsMap[id].erase( param );
	}
}
#pragma endregion
}

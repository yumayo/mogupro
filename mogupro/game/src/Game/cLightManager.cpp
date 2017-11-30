#include <Game/cLightManager.h>
#include <Game/cGemManager.h>
#include <Game/cPlayerManager.h>
#include <Game/cFieldManager.h>
#include <random>
namespace Game
{
void cLightManager::setup( )
{
	auto const& players = Game::cPlayerManager::getInstance( )->getPlayers( );
	for ( int i = 0; i < players.size( ); ++i )
	{
		cinder::vec3 color;
		switch ( players[i]->getWhichTeam( ) )
		{
		case Game::Player::Team::Blue:
			color = { 0, 0, 1 };
			break;
		case Game::Player::Team::Red:
			color = { 1,0,0 };
			break;
		default:
			break;
		}
		mPointLightHandles.emplace_back( Game::cLightManager::getInstance( )->addPointLight( players[i]->getPos( ), color, 1.0F ) );
	}
	auto const& activePlayer = Game::cPlayerManager::getInstance( )->getActivePlayer( );
	cinder::vec3 color;
	switch ( activePlayer->getWhichTeam( ) )
	{
	case Game::Player::Team::Blue:
		color = { 0.05, 0.05, 0.1 };
		break;
	case Game::Player::Team::Red:
		color = { 0.1, 0.05, 0.05 };
		break;
	default:
		break;
	}
	for ( int i = 0; i < 20; ++i )
	{
		mPlayerPositionBuffer.emplace_back( activePlayer->getPos( ) );
	}
	for ( int i = 0; i < 20 - 1; ++i )
	{
		mLineLightHandles.emplace_back( Game::cLightManager::getInstance( )->addLineLight( activePlayer->getPos( ), activePlayer->getPos( ), color, 0.4F ) );
	}
	//for ( int i = 0; i < GemManager->getGems( ).size( ); ++i )
	//{
	//	Game::cLightManager::getInstance( )->addPointLight( GemManager->getGems( )[i]->getPos( ), GemManager->getGems( )[i]->getColor( ), 0.0F );
	//}
}
void cLightManager::update( )
{
	auto const& players = Game::cPlayerManager::getInstance( )->getPlayers( );
	for ( int i = 0; i < players.size( ); ++i )
	{
		if (players[i]->getWhichTeam() == Player::Blue) {
			mPointLightHandles[i]->color = ci::vec3(1 - players[i]->getStatus().hp / Player::MAX_HP, 0, 1);
		}
		if (players[i]->getWhichTeam() == Player::Red) {
			mPointLightHandles[i]->color = ci::vec3(1, 0,1 - players[i]->getStatus().hp / Player::MAX_HP);
		}
		mPointLightHandles[i]->reAttachPosition( mPointLightHandles[i], players[i]->getPos( ) );
	}

	static int c = 0; c++;
	if ( c % 10 == 0 )
	{
		mPlayerPositionBuffer.erase( mPlayerPositionBuffer.begin( ) );
		mPlayerPositionBuffer.emplace_back( Game::cPlayerManager::getInstance( )->getActivePlayer( )->getPos( ) + ci::vec3(0, -0.3, 0) );
	}

	for ( int i = 0; i < mPlayerPositionBuffer.size( ) - 1; ++i )
	{
		mLineLightHandles[i]->reAttachLine( mLineLightHandles[i], mPlayerPositionBuffer[i], mPlayerPositionBuffer[i + 1] );
	}
}
std::set<Utility::hardptr<Light::cPointLightParam>> const & cLightManager::getPointLights( ) const
{
	return mPointLights;
}
boost::optional<std::set<Light::PointLightHandle> const&> cLightManager::getPointLights( int chunkId ) const
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
	auto temp = mPointLights.insert( std::make_shared<Light::cPointLightParam>( position, color, radius ) );
	if ( !temp.second )
	{
		throw std::runtime_error( "やばい." );
	}
	auto handle = ( *temp.first );
	attachChunk( Light::PointLightHandle( handle ) );
	return handle;
}
void cLightManager::removePointLight( Light::PointLightHandle handle )
{
	detachChunk( handle );
	mPointLights.erase( handle );
}
void cLightManager::attachChunk( Light::PointLightHandle handle )
{
	for ( int id : cFieldManager::getInstance( )->getChunkId( handle->getPosition( ), handle->getRadius( ) ) )
	{
		mPointLightsMap[id].insert( handle );
	}
}
void cLightManager::detachChunk( Light::PointLightHandle handle )
{
	for ( int id : cFieldManager::getInstance( )->getChunkId( handle->getPosition( ), handle->getRadius( ) ) )
	{
		mPointLightsMap[id].erase( handle );
	}
}
std::set<Utility::hardptr<Light::cLineLightParam>> const & cLightManager::getLineLights( ) const
{
	return mLineLights;
}
boost::optional<std::set<Light::LineLightHandle>const&> cLightManager::getLineLights( int chunkId ) const
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
	auto temp = mLineLights.insert( std::make_shared<Light::cLineLightParam>( beginPosition, endPosition, color, radius ) );
	if ( !temp.second )
	{
		throw std::runtime_error( "やばい." );
	}
	auto handle = ( *temp.first );
	attachChunk( Light::LineLightHandle( handle ) );
	return handle;
}
void cLightManager::removeLineLight( Light::LineLightHandle handle )
{
	detachChunk( handle );
	mLineLights.erase( handle );
}
void cLightManager::attachChunk( Light::LineLightHandle handle )
{
	// 始点を判定に使った簡易なもの。
	for ( int id : cFieldManager::getInstance( )->getChunkId( handle->getBeginPosition( ), handle->getRadius( ) ) )
	{
		mLineLightsMap[id].insert( handle );
	}
}
void cLightManager::detachChunk( Light::LineLightHandle handle )
{
	for ( int id : cFieldManager::getInstance( )->getChunkId( handle->getBeginPosition( ), handle->getRadius( ) ) )
	{
		mLineLightsMap[id].erase( handle );
	}
}
}

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
		color = { 0.0, 0.0, 0.05 };
		break;
	case Game::Player::Team::Red:
		color = { 0.05, 0.0, 0.0 };
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
			mPointLightHandles[i]->color = ci::vec3(0, 0, 1 - (players[i]->getStatus().hp / Player::MAX_HP) / 2);
		}
		if (players[i]->getWhichTeam() == Player::Red) {
			mPointLightHandles[i]->color = ci::vec3(1 - (players[i]->getStatus().hp / Player::MAX_HP)/2, 0,0);
		}
		mPointLightHandles[i]->reAttachPositionWithRadius( players[i]->getPos( ),1 + 2 -  (players[i]->getStatus().hp / Player::MAX_HP) * 2);
	}

	static int c = 0; c++;
	if ( c % 10 == 0 )
	{
		mPlayerPositionBuffer.erase( mPlayerPositionBuffer.begin( ) );
		mPlayerPositionBuffer.emplace_back( Game::cPlayerManager::getInstance( )->getActivePlayer( )->getPos( ) + ci::vec3(0, -0.3, 0) );
	}

	for ( int i = 0; i < mPlayerPositionBuffer.size( ) - 1; ++i )
	{
		mLineLightHandles[i]->reAttachLine( mPlayerPositionBuffer[i], mPlayerPositionBuffer[i + 1] );
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

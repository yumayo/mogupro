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
			color = { 0.8, 0.8, 1 };
			break;
		case Game::Player::Team::Red:
			color = { 1, 0.8, 0.8 };
			break;
		default:
			break;
		}
		mPointLightHandles.emplace_back( Game::cLightManager::getInstance( )->addPointLight( players[i]->getPos( ), color, 1.0F ) );
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
		mPointLightHandles[i]->reAttachPosition( mPointLightHandles[i], players[i]->getPos( ) );
	}
}
boost::optional<std::set<Utility::softptr<Light::cPointLightParam>> const&> cLightManager::getPointLights( int chunkId ) const
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
Utility::softptr<Light::cPointLightParam> cLightManager::addPointLight( cinder::vec3 position, cinder::vec3 color, float radius )
{
	auto temp = mPointLights.insert( std::make_shared<Light::cPointLightParam>( position, color, radius ) );
	if ( !temp.second )
	{
		throw std::runtime_error( "‚â‚Î‚¢." );
	}
	auto handle = ( *temp.first );
	for ( int id : cFieldManager::getInstance( )->getChunkId( position, radius ) )
	{
		mPointLightsMap[id].insert( handle );
	}
	return handle;
}
void cLightManager::removePointLight( Utility::softptr<Light::cPointLightParam> handle )
{
	mPointLights.erase( handle );
}
void cLightManager::attachChunk( Utility::softptr<Light::cPointLightParam> handle )
{
	for ( int id : cFieldManager::getInstance( )->getChunkId( handle->getPosition( ), handle->getRadius( ) ) )
	{
		mPointLightsMap[id].insert( handle );
	}
}
void cLightManager::detachChunk( Utility::softptr<Light::cPointLightParam> handle )
{
	for ( int id : cFieldManager::getInstance( )->getChunkId( handle->getPosition( ), handle->getRadius( ) ) )
	{
		mPointLightsMap[id].erase( handle );
	}
}
}

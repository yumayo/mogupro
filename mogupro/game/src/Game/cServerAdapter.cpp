#include <Game/cServerAdapter.h>
#include <Network/cUDPServerManager.h>
#include <Game/Field/FieldData.h>
#include <Game/cPlayerManager.h>
#include <Game/Player/cPlayer.h>
using namespace Network;
using namespace Network::Packet;
using namespace Network::Packet::Event;
using namespace Network::Packet::Request;
using namespace Network::Packet::Response;
namespace Game
{
cServerAdapter::cServerAdapter( )
{
	mObjectId = 0;

	ubyte1 index = 0;
	for ( auto& respo : Game::Field::RESPAWN_POINT )
	{
		mPlayers[index] = { index, true, respo, cinder::vec2(0) };
		index += 1;
	}
}
cServerAdapter::~cServerAdapter( )
{

}
void cServerAdapter::update( )
{
	sendPlayers( );
	sendSetQuarry( );
	sendGetGemPlayer( );
	sendGetGemQuarry( );
	sendBreakBlocks( );
	sendLightBombs( );
	sendResult( );
	sendAddCannonPower( );
}
void cServerAdapter::sendPlayers( )
{
	for ( auto& m : Network::cUDPServerManager::getInstance( )->getUDPManager( ) )
	while ( auto packet = m->ReqPlayer.get( ) )
	{
		auto id = packet->mFormat.playerId;
		mPlayers[id].id = packet->mFormat.playerId;
		mPlayers[id].position = packet->mFormat.position;
		mPlayers[id].rotation = packet->mFormat.rotation;
	}
	{
		auto eventPack = new cEvePlayers( );
		for ( auto& player : mPlayers )
		{
			eventPack->mPlayerFormats.emplace_back( player.second.id, player.second.position, player.second.rotation );
		}
		Network::cUDPServerManager::getInstance( )->broadcast( eventPack );
	}

	for ( auto& m : Network::cUDPServerManager::getInstance( )->getUDPManager( ) )
	while ( auto p = m->ReqPlayerDeath.get( ) )
	{
		if ( mPlayers[p->enemyId].kill( ) )
		{
			auto evePack = new cEvePlayerDeath( );
			evePack->playerId = p->playerId;
			evePack->enemyId = p->enemyId;
			Network::cUDPServerManager::getInstance( )->broadcast( evePack );
		}
	}

	for ( auto& m : Network::cUDPServerManager::getInstance( )->getUDPManager( ) )
	while ( auto p = m->ReqDamage.get( ) )
	{
		auto eve = new cEveDamage( );
		eve->playerId = p->playerId;
		eve->enemyId = p->enemyId;
		eve->damage = p->damage;
		Network::cUDPServerManager::getInstance( )->broadcast( eve );
	}

	for ( auto& m : Network::cUDPServerManager::getInstance( )->getUDPManager( ) )
	while ( auto p = m->ReqRespawn.get( ) )
	{
		auto eve = new cEveRespawn( );
		eve->playerId = p->playerId;
		mPlayers[p->playerId].respawn( );
		cUDPServerManager::getInstance( )->broadcast( eve );
	}

	for ( auto& m : Network::cUDPServerManager::getInstance( )->getUDPManager( ) )
	while ( auto p = m->ReqPlayerAttack.get( ) )
	{
		auto eve = new cEvePlayerAttack( );
		eve->playerId = p->playerId;
		eve->call = p->call;
		cUDPServerManager::getInstance( )->broadcast( eve );
	}
}
void cServerAdapter::sendSetQuarry( )
{
	for ( auto& m : Network::cUDPServerManager::getInstance( )->getUDPManager( ) )
	while ( auto packet = m->ReqSetQuarry.get( ) )
	{
		if ( true )
		{
			auto eventPack = new cEveSetQuarry( );
			eventPack->mObjectId = mObjectId++;
			eventPack->mPosition = packet->mPosition;
			eventPack->mPlayerId = packet->mPlayerId;
			Network::cUDPServerManager::getInstance( )->broadcast( eventPack );
		}
	}
}
void cServerAdapter::sendGetGemPlayer( )
{
	for ( auto& m : Network::cUDPServerManager::getInstance( )->getUDPManager( ) )
	while ( auto packet = m->ReqGetJemPlayer.get( ) )
	{
		// 成功なら他の人に俺、宝石採ったぜアピールをします。
		if ( mGems.insert( packet->mGemId ).second )
		{
			auto eventPack = new cEveGetJemPlayer( );
			eventPack->mPlayerId = packet->mPlayerId;
			eventPack->mGemId = packet->mGemId;
			Network::cUDPServerManager::getInstance( )->broadcast( eventPack );
		}
	}
}
void cServerAdapter::sendGetGemQuarry( )
{
	for ( auto& m : Network::cUDPServerManager::getInstance( )->getUDPManager( ) )
	while ( auto packet = m->ReqGetJemQuarry.get( ) )
	{
		// 成功なら他の人に俺の掘削機、宝石採ったぜアピールをします。
		if ( mGems.insert( packet->mGemId ).second )
		{
			auto eventPack = new cEveGetJemQuarry( );
			eventPack->mObjectId = packet->mObjectId;
			eventPack->mGemId = packet->mGemId;
			Network::cUDPServerManager::getInstance( )->broadcast( eventPack );
		}
	}
}
void cServerAdapter::sendBreakBlocks( )
{
	auto breakBlocksPacket = new cEveBreakBlocks( );
	for ( auto& m : Network::cUDPServerManager::getInstance( )->getUDPManager( ) )
	while ( auto packet = m->ReqBreakBlocks.get( ) )
	{
		std::copy( packet->mBreakFormats.begin( ),
				   packet->mBreakFormats.end( ),
				   std::back_inserter( breakBlocksPacket->mBreakFormats ) );
	}
	if ( !breakBlocksPacket->mBreakFormats.empty( ) )
	{
		Network::cUDPServerManager::getInstance( )->broadcast( breakBlocksPacket );
	}
}
void cServerAdapter::sendLightBombs( )
{
	for ( auto& m : Network::cUDPServerManager::getInstance( )->getUDPManager( ) )
	while ( auto packet = m->ReqLightBomb.get( ) )
	{
		auto eventPack = new cEveLightBomb( );
		eventPack->playerId = packet->playerId;
		eventPack->position = packet->position;
		eventPack->objectId = mObjectId++;
		eventPack->speed = packet->speed;
		Network::cUDPServerManager::getInstance( )->broadcast( eventPack );
	}
}
void cServerAdapter::sendResult( )
{
	for ( auto& m : Network::cUDPServerManager::getInstance( )->getUDPManager( ) )
	while ( auto p = m->ReqResult.get( ) )
	{
		auto s = new cResResult( );
		s->redTeamPower = mRedTeamPower;
		s->blueTeamPower = mBlueTeamPower;
		cUDPServerManager::getInstance( )->send( p->networkHandle, s );
	}
}
void cServerAdapter::sendAddCannonPower( )
{
	for ( auto& m : Network::cUDPServerManager::getInstance( )->getUDPManager( ) )
	while ( auto p = m->ReqAddCannonPower.get( ) )
	{
		if ( p->teamId == Game::Player::Red )
		{
			mRedTeamPower += p->power;
		}
		else if ( p->teamId == Game::Player::Blue )
		{
			mBlueTeamPower += p->power;
		}
		else
		{
			continue;
		}

		auto e = new cEveAddCannonPower( );
		e->teamId = p->teamId;
		e->power = p->power;
		cUDPServerManager::getInstance( )->broadcast( e );
	}
}
void cServerAdapter::Player::respawn( )
{
	isLive = true;
}
bool cServerAdapter::Player::kill( )
{
	if ( isLive == false ) return false;
	isLive = false;
	return true;
}
}

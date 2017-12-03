#include <Game/cServerAdapter.h>
#include <Network/cUDPServerManager.h>
#include <Network/cUDPManager.h>
#include <Network/cDeliverManager.h>
#include <Network/cEventManager.h>
#include <Network/cRequestManager.h>
#include <Network/cResponseManager.h>
#include <Game/Field/FieldData.h>
#include <Game/cPlayerManager.h>
#include <Game/Player/cPlayer.h>
using namespace Network;
using namespace Network::Packet;
using namespace Network::Packet::Deliver;
using namespace Network::Packet::Event;
using namespace Network::Packet::Request;
using namespace Network::Packet::Response;
namespace Game
{
cServerAdapter::cServerAdapter( )
{
	ci::vec3 worldSize = ci::vec3( Game::Field::CHUNK_RANGE_X, Game::Field::CHUNK_RANGE_Y, Game::Field::CHUNK_RANGE_Z ) * Game::Field::BLOCK_SIZE * (float)Game::Field::CHUNK_SIZE;

	mQuarryId = 0;
	mPlayers.insert( std::make_pair( 0, Player{ 0, true, cinder::vec3( worldSize.x / 2 - 1.5F, worldSize.y + 1.0F, 7.0F ), cinder::quat( ) } ) );
	mPlayers.insert( std::make_pair( 1, Player{ 1, true, cinder::vec3( worldSize.x / 2 - 0.5F, worldSize.y + 1.0F, 7.0F ), cinder::quat( ) } ) );
	mPlayers.insert( std::make_pair( 2, Player{ 2, true, cinder::vec3( worldSize.x / 2 + 0.5F, worldSize.y + 1.0F, 7.0F ), cinder::quat( ) } ) );
	mPlayers.insert( std::make_pair( 3, Player{ 3, true, cinder::vec3( worldSize.x / 2 + 1.5F, worldSize.y + 1.0F, 7.0F ), cinder::quat( ) } ) );
	mPlayers.insert( std::make_pair( 4, Player{ 4, true, cinder::vec3( worldSize.x / 2 - 1.5F, worldSize.y + 1.0F, worldSize.z - 7.0F ), cinder::quat( ) } ) );
	mPlayers.insert( std::make_pair( 5, Player{ 5, true, cinder::vec3( worldSize.x / 2 - 0.5F, worldSize.y + 1.0F, worldSize.z - 7.0F ), cinder::quat( ) } ) );
	mPlayers.insert( std::make_pair( 6, Player{ 6, true, cinder::vec3( worldSize.x / 2 + 0.5F, worldSize.y + 1.0F, worldSize.z - 7.0F ), cinder::quat( ) } ) );
	mPlayers.insert( std::make_pair( 7, Player{ 7, true, cinder::vec3( worldSize.x / 2 + 1.5F, worldSize.y + 1.0F, worldSize.z - 7.0F ), cinder::quat( ) } ) );
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
	sendCannonPower( );
	sendAddCannonPower( );
}
void cServerAdapter::sendPlayers( )
{
	auto dli = Network::cDeliverManager::getInstance( );
	auto req = Network::cRequestManager::getInstance( );

	while ( auto packet = dli->getDliPlayer( ) )
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

	while ( auto p = req->getReqCheckPlayerDeath( ) )
	{
		auto resPack = new cResCheckPlayerDeath( );
		resPack->playerId = p->playerId;
		resPack->enemyId = p->enemyId;
		resPack->isSuccess = mPlayers[p->enemyId].kill( );

		if ( resPack->isSuccess )
		{
			auto evePack = new cEvePlayerDeath( );
			evePack->playerId = resPack->playerId;
			evePack->enemyId = resPack->enemyId;
			Network::cUDPServerManager::getInstance( )->broadcastOthers( p->networkHandle, evePack );
		}

		Network::cUDPServerManager::getInstance( )->send( p->networkHandle, resPack );
	}

	while ( auto p = req->getReqDamage( ) )
	{
		auto eve = new cEveDamage( );
		eve->playerId = p->playerId;
		eve->enemyId = p->enemyId;
		eve->damage = p->damage;
		Network::cUDPServerManager::getInstance( )->broadcast( eve );
	}

	while ( auto p = req->getReqRespawn( ) )
	{
		auto eve = new cEveRespawn( );
		eve->playerId = p->playerId;
		mPlayers[p->playerId].respawn( );
		cUDPServerManager::getInstance( )->broadcastOthers( p->networkHandle, eve );
	}
}
void cServerAdapter::sendSetQuarry( )
{
	auto req = Network::cRequestManager::getInstance( );
	while ( auto packet = req->getReqCheckSetQuarry( ) )
	{
		mQuarryId += 1;
		mQuarrys.insert( mQuarryId );
		auto quarryPack = new cResCheckSetQuarry( );
		quarryPack->mDrillId = mQuarryId;
		quarryPack->mIsSucceeded = true;
		quarryPack->mPosition = packet->mPosition;
		quarryPack->mType = packet->mType;
		quarryPack->mTeamId = packet->mTeamId;

		if ( quarryPack->mIsSucceeded )
		{
			auto eventPack = new cEveSetQuarry( );
			eventPack->mDrillId = quarryPack->mDrillId;
			eventPack->mPosition = quarryPack->mPosition;
			eventPack->mType = quarryPack->mType;
			eventPack->mTeamId = quarryPack->mTeamId;
			Network::cUDPServerManager::getInstance( )->broadcastOthers( packet->mNetworkHandle, eventPack );
		}

		Network::cUDPServerManager::getInstance( )->send( packet->mNetworkHandle, quarryPack );
	}
}
void cServerAdapter::sendGetGemPlayer( )
{
	auto req = Network::cRequestManager::getInstance( );
	while ( auto packet = req->getReqCheckGetJemPlayer( ) )
	{
		auto resPack = new cResCheckGetJemPlayer( );
		auto isSuccess = mGems.insert( packet->mGemId ).second;

		resPack->mPlayerId = packet->mPlayerId;
		resPack->mGemId = packet->mGemId;
		resPack->mIsSuccessed = isSuccess;

		// 成功なら他の人に俺、宝石採ったぜアピールをします。
		if ( isSuccess )
		{
			auto eventPack = new cEveGetJemPlayer( );
			eventPack->mPlayerId = packet->mPlayerId;
			eventPack->mGemId = packet->mGemId;
			Network::cUDPServerManager::getInstance( )->broadcastOthers( packet->mNetworkHandle, eventPack );
		}

		Network::cUDPServerManager::getInstance( )->send( packet->mNetworkHandle, resPack );
	}
}
void cServerAdapter::sendGetGemQuarry( )
{
	auto req = Network::cRequestManager::getInstance( );
	while ( auto packet = req->getReqCheckGetJemQuarry( ) )
	{
		auto resPack = new cResCheckGetJemQuarry( );
		auto isSuccess = mGems.insert( packet->mGemId ).second;

		resPack->mDrillId = packet->mDrillId;
		resPack->mGemId = packet->mGemId;
		resPack->mIsSuccessed = isSuccess;

		// 成功なら他の人に俺の掘削機、宝石採ったぜアピールをします。
		if ( isSuccess )
		{
			auto eventPack = new cEveGetJemQuarry( );
			eventPack->mDrillId = packet->mDrillId;
			eventPack->mGemId = packet->mGemId;
			Network::cUDPServerManager::getInstance( )->broadcastOthers( packet->mNetworkHandle, eventPack );
		}

		Network::cUDPServerManager::getInstance( )->send( packet->mNetworkHandle, resPack );
	}
}
void cServerAdapter::sendBreakBlocks( )
{
	auto dli = ::Network::cDeliverManager::getInstance( );
	auto breakBlocksPacket = new cEveBreakBlocks( );
	while ( auto packet = dli->getDliBreakBlocks( ) )
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
	auto req = Network::cRequestManager::getInstance( );
	while ( auto packet = req->getReqCheckLightBomb( ) )
	{
		auto eventPack = new cEveLightBomb( );
		eventPack->playerId = packet->playerId;
		eventPack->position = packet->position;
		eventPack->speed = packet->speed;
		Network::cUDPServerManager::getInstance( )->broadcast( eventPack );
	}
}
void cServerAdapter::sendResult( )
{
	auto q = cRequestManager::getInstance( );
	while ( auto p = q->getReqResult( ) )
	{
		auto s = new cResResult( );
		s->redTeamPower = mRedTeamPower;
		s->blueTeamPower = mBlueTeamPower;
		cUDPServerManager::getInstance( )->send( p->networkHandle, s );
	}
}
void cServerAdapter::sendCannonPower( )
{
	auto q = cRequestManager::getInstance( );
	while ( auto p = q->getReqCannonPower( ) )
	{
		auto s = new cResCannonPower( );
		s->redTeamPower = mRedTeamPower;
		s->blueTeamPower = mBlueTeamPower;
		cUDPServerManager::getInstance( )->send( p->networkHandle, s );
	}
}
void cServerAdapter::sendAddCannonPower( )
{
	auto q = cRequestManager::getInstance( );
	while ( auto p = q->getReqAddCannonPower( ) )
	{
		if ( p->teamId == Game::Player::Red )
		{
			mRedTeamPower += p->power;
		}
		else if ( p->teamId == Game::Player::Blue )
		{
			mBlueTeamPower += p->power;
		}
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

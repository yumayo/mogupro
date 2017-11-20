#include <Game/cServerAdapter.h>
#include <Network/cUDPServerManager.h>
#include <Network/cUDPManager.h>
#include <Network/cDeliverManager.h>
#include <Network/cEventManager.h>
#include <Network/cRequestManager.h>
#include <Network/cResponseManager.h>
#include <Game/Field/FieldData.h>
namespace Game
{
cServerAdapter::cServerAdapter( )
{
	ci::vec3 worldSize = ci::vec3( Game::Field::CHUNK_RANGE_X, Game::Field::CHUNK_RANGE_Y, Game::Field::CHUNK_RANGE_Z ) * Game::Field::BLOCK_SIZE * (float)Game::Field::CHUNK_SIZE;

	mQuarryId = 0;
	mPlayerFormats.insert( std::make_pair( 0, Network::Packet::PlayerFormat( 0, cinder::vec3( worldSize.x / 2 - 1.5F, worldSize.y + 1.0F, 7.0F ), cinder::quat( ) ) ) );
	mPlayerFormats.insert( std::make_pair( 1, Network::Packet::PlayerFormat( 1, cinder::vec3( worldSize.x / 2 - 0.5F, worldSize.y + 1.0F, 7.0F ), cinder::quat( ) ) ) );
	mPlayerFormats.insert( std::make_pair( 2, Network::Packet::PlayerFormat( 2, cinder::vec3( worldSize.x / 2 + 0.5F, worldSize.y + 1.0F, 7.0F ), cinder::quat( ) ) ) );
	mPlayerFormats.insert( std::make_pair( 3, Network::Packet::PlayerFormat( 3, cinder::vec3( worldSize.x / 2 + 1.5F, worldSize.y + 1.0F, 7.0F ), cinder::quat( ) ) ) );
	mPlayerFormats.insert( std::make_pair( 4, Network::Packet::PlayerFormat( 4, cinder::vec3( worldSize.x / 2 - 1.5F, worldSize.y + 1.0F, worldSize.z - 7.0F ), cinder::quat( ) ) ) );
	mPlayerFormats.insert( std::make_pair( 5, Network::Packet::PlayerFormat( 5, cinder::vec3( worldSize.x / 2 - 0.5F, worldSize.y + 1.0F, worldSize.z - 7.0F ), cinder::quat( ) ) ) );
	mPlayerFormats.insert( std::make_pair( 6, Network::Packet::PlayerFormat( 6, cinder::vec3( worldSize.x / 2 + 0.5F, worldSize.y + 1.0F, worldSize.z - 7.0F ), cinder::quat( ) ) ) );
	mPlayerFormats.insert( std::make_pair( 7, Network::Packet::PlayerFormat( 7, cinder::vec3( worldSize.x / 2 + 1.5F, worldSize.y + 1.0F, worldSize.z - 7.0F ), cinder::quat( ) ) ) );
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
}
void cServerAdapter::sendPlayers( )
{
	auto dli = Network::cDeliverManager::getInstance( );
	while ( auto packet = dli->getDliPlayer( ) )
	{
		try
		{
			auto id = packet->mFormat.playerId;
			mPlayerFormats[id].position = packet->mFormat.position;
			mPlayerFormats[id].rotation = packet->mFormat.rotation;
		}
		catch ( std::runtime_error e )
		{
			continue;
		}
	}
	auto eventPack = new Network::Packet::Event::cEvePlayers( );
	for ( auto& player : mPlayerFormats )
	{
		eventPack->mPlayerFormats.emplace_back( player.first, player.second.position, player.second.rotation );
	}
	Network::cUDPServerManager::getInstance( )->broadcast( eventPack );
}
void cServerAdapter::sendSetQuarry( )
{
	auto req = Network::cRequestManager::getInstance( );
	while ( auto packet = req->getReqCheckSetQuarry( ) )
	{
		mQuarryId += 1;
		mQuarrys.insert( mQuarryId );
		auto quarryPack = new Network::Packet::Response::cResCheckSetQuarry( );
		quarryPack->mDrillId = mQuarryId;
		quarryPack->mIsSucceeded = true;
		quarryPack->mPosition = packet->mPosition;
		quarryPack->mType = packet->mType;
		quarryPack->mTeamId = packet->mTeamId;

		if ( quarryPack->mIsSucceeded )
		{
			auto eventPack = new Network::Packet::Event::cEveSetQuarry( );
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
		try
		{
			auto resPack = new Network::Packet::Response::cResCheckGetJemPlayer( );
			auto isSuccess = mGems.insert( packet->mGemId ).second;

			resPack->mPlayerId = packet->mPlayerId;
			resPack->mGemId = packet->mGemId;
			resPack->mIsSuccessed = isSuccess;

			// 成功なら他の人に俺、宝石採ったぜアピールをします。
			if ( isSuccess )
			{
				auto eventPack = new Network::Packet::Event::cEveGetJemPlayer( );
				eventPack->mPlayerId = packet->mPlayerId;
				eventPack->mGemId = packet->mGemId;
				Network::cUDPServerManager::getInstance( )->broadcastOthers( packet->mNetworkHandle, eventPack );
			}

			Network::cUDPServerManager::getInstance( )->send( packet->mNetworkHandle, resPack );
		}
		catch ( std::runtime_error e )
		{
			continue;
		}
	}
}
void cServerAdapter::sendGetGemQuarry( )
{
	auto req = Network::cRequestManager::getInstance( );
	while ( auto packet = req->getReqCheckGetJemQuarry( ) )
	{
		auto resPack = new Network::Packet::Response::cResCheckGetJemQuarry( );
		auto isSuccess = mGems.insert( packet->mGemId ).second;

		resPack->mDrillId = packet->mDrillId;
		resPack->mGemId = packet->mGemId;
		resPack->mIsSuccessed = isSuccess;

		// 成功なら他の人に俺の掘削機、宝石採ったぜアピールをします。
		if ( isSuccess )
		{
			auto eventPack = new Network::Packet::Event::cEveGetJemQuarry( );
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
	auto breakBlocksPacket = new Network::Packet::Event::cEveBreakBlocks( );
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
}

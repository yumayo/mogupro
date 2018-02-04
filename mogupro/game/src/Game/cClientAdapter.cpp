#include <Game/cClientAdapter.h>
#include <Network/cUDPClientManager.h>
#include <Game/cGemManager.h>
#include <Game/cPlayerManager.h>
#include <Game/cStrategyManager.h>
#include <Game/cFieldManager.h>
#include <Game/cSubWeaponManager.h>
#include <Game/cGameManager.h>
#include <Game/Weapons/MainWeapon/cBase.h>
using namespace cinder;
using namespace Network;
using namespace Network::Packet;
using namespace Network::Packet::Event;
using namespace Network::Packet::Request;
using namespace Network::Packet::Response;
namespace Game
{
cClientAdapter::cClientAdapter( )
    : mBreakBlocksPecket( new cReqBreakBlocks( ) )
{
}
cClientAdapter::~cClientAdapter( )
{
    delete mBreakBlocksPecket;
}
void cClientAdapter::update( )
{
    // ブロックだけはまとめてで送信します。
    sendBreakBlocks( );

    // サーバーから受信したものがあった場合は取り出して、
    // 各マネージャーに伝えます。
    recvAllPlayers( );
    recvAllQuarrys( );
    recvAllGems( );
    recvAllBreakBlocks( );
	recvAllBombs( );
	recvAllCannons( );
	recvAllGameInfo( );
}
void cClientAdapter::recvAllPlayers( )
{
    auto m = Network::cUDPClientManager::getInstance( )->getUDPManager( );

    while ( auto packet = m->EvePlayers.get( ) )
    {
		if ( packet->getSequenceId( ) < mPlayerSeq )
		{
			continue;
		}
		mPlayerSeq = packet->getSequenceId( );
        for ( auto& o : packet->mPlayerFormats )
        {
			Game::cPlayerManager::getInstance( )->getPlayer( o.playerId )->setPos( o.position,o.rotation );
			//Game::cPlayerManager::getInstance( )->getPlayer( o.playerId )->setRotate(o.rotation.x, o.rotation.x);
        }
    }
	while ( auto packet = m->EveDamage.get( ) )
	{
		Game::cPlayerManager::getInstance( )->getPlayer( packet->enemyId )->receiveDamage( packet->damage, packet->playerId );
	}
	while ( auto packet = m->EvePlayerDeath.get( ) )
	{
		// TODO:プレイヤーをキルする。
		packet->enemyId;
		packet->playerId;
	}
	while ( auto packet = m->EveRespawn.get( ) )
	{
		// TODO:プレイヤーをリスポーンさせる。
		packet->playerId;
	}
	while ( auto packet = m->EvePlayerAttack.get( ) )
	{
		switch ( packet->call )
		{
		case 1:
			cPlayerManager::getInstance( )->getPlayer( packet->playerId )->getMainWeapon( )->pushCall( );
			break;
		case 2:
			cPlayerManager::getInstance( )->getPlayer( packet->playerId )->getMainWeapon( )->pullCall( );
			break;
		default:
			break;
		}
	}
}
void cClientAdapter::recvAllQuarrys( )
{
    auto m = Network::cUDPClientManager::getInstance( )->getUDPManager( );
    while ( auto packet = m->EveSetQuarry.get( ) )
    {
		cSubWeaponManager::getInstance( )->createQuarry(
			packet->mPosition
			, packet->mObjectId
			, packet->mPlayerId
		);
    }
}
void cClientAdapter::recvAllGems( )
{
    auto m = Network::cUDPClientManager::getInstance( )->getUDPManager( );
    while ( auto packet = m->EveGetJemQuarry.get( ) )
    {
        cSubWeaponManager::getInstance( )->HitDrillToGem(
            packet->mObjectId,
            packet->mGemId
        );
    }
    while ( auto packet = m->EveGetJemPlayer.get( ) )
    {
		for ( auto& gem : cGemManager::getInstance( )->breakGemStone( packet->mGemId ) )
		{
			cPlayerManager::getInstance( )->getPlayer(packet->mPlayerId)->getGems( gem->getId() );
		}
    }
}
void cClientAdapter::recvAllBreakBlocks( )
{
    auto m = Network::cUDPClientManager::getInstance( )->getUDPManager( );
    while ( auto packet = m->EveBreakBlocks.get( ) )
    {
        for ( auto& o : packet->mBreakFormats )
        {
            Game::cFieldManager::getInstance( )->blockBreakNetwork(
                o.position, o.radius, o.type
            );
        }
    }
}
void cClientAdapter::recvAllBombs( )
{
	auto m = Network::cUDPClientManager::getInstance( )->getUDPManager( );
	while ( auto packet = m->EveLightBomb.get( ) )
	{
		cSubWeaponManager::getInstance( )->createLightBomb( packet->position, packet->speed, cinder::vec3(0.5F), packet->objectId, packet->playerId );
	}
}
void cClientAdapter::recvAllCannons( )
{
	auto m = Network::cUDPClientManager::getInstance( )->getUDPManager( );
	while ( auto packet = m->EveAddCannonPower.get( ) )
	{
		if ( packet->teamId == Game::Player::Red )
		{
			cGameManager::getInstance( )->addRedCannonPower( packet->power );
			cGameManager::getInstance( )->appendGem( packet->playerId, packet->power );
			cStrategyManager::getInstance()->getCannons()[int(packet->teamId)]->setAddCanonPower(packet->power);
		}
		else if ( packet->teamId == Game::Player::Blue )
		{
			cGameManager::getInstance( )->addBlueCannonPower( packet->power );
			cGameManager::getInstance( )->appendGem( packet->playerId, packet->power );
			cStrategyManager::getInstance()->getCannons()[int(packet->teamId)]->setAddCanonPower(packet->power);
		}
		else
		{
			continue;
		}
	}
}
void cClientAdapter::recvAllGameInfo( )
{
	auto m = Network::cUDPClientManager::getInstance( )->getUDPManager( );

	while ( auto packet = m->ResSetGamestartTimer.get( ) )
	{
		auto startTime = packet->time;
		Game::cGameManager::getInstance( )->setTime( startTime );
	}
}
void cClientAdapter::sendBreakBlock( cinder::vec3 const & position, float radius, Network::ubyte1 type )
{
    // ブロック破壊は一旦バッファに詰めておきます。
    mBreakBlocksPecket->mBreakFormats.emplace_back( position, radius, type );
}
void cClientAdapter::sendSetQuarry( cinder::vec3 const & position )
{
    auto packet = new cReqSetQuarry( );
    packet->mPosition = position;
    packet->mPlayerId = cPlayerManager::getInstance( )->getActivePlayerId( );
    cUDPClientManager::getInstance( )->send( packet );
}
void cClientAdapter::sendPlayer( cinder::vec3 const & position, cinder::quat const & rotation )
{
    auto packet = new cReqPlayer( );
    packet->mFormat.playerId = cPlayerManager::getInstance( )->getActivePlayerId( );
    packet->mFormat.position = position;
    packet->mFormat.rotation = rotation;
    cUDPClientManager::getInstance( )->send( packet );
}
void cClientAdapter::sendGetGemPlayer( Network::ubyte2 gemId )
{
    auto packet = new cReqGetJemPlayer( );
    packet->mPlayerId = cPlayerManager::getInstance( )->getActivePlayerId( );
    packet->mGemId = gemId;
    cUDPClientManager::getInstance( )->send( packet );
}
void cClientAdapter::sendGetGemQuarry( Network::ubyte2 objectId, Network::ubyte2 gemId )
{
    auto packet = new cReqGetJemQuarry( );
    packet->mObjectId = objectId;
    packet->mGemId = gemId;
    cUDPClientManager::getInstance( )->send( packet );
}
void cClientAdapter::sendLightBomb( cinder::vec3 const & position, cinder::vec3 const & speed )
{
	auto packet = new cReqLightBomb( );
	packet->playerId = cPlayerManager::getInstance( )->getActivePlayerId( );
	packet->position = position;
	packet->speed = speed;
	cUDPClientManager::getInstance( )->send( packet );
}
void cClientAdapter::sendKill( Network::ubyte1 enemyId )
{
	auto p = new cReqPlayerDeath( );
	p->enemyId = enemyId;
	p->playerId = cPlayerManager::getInstance( )->getActivePlayerId( );
	cUDPClientManager::getInstance( )->send( p );
}
void cClientAdapter::sendDamage( Network::ubyte1 enemyId, float damage )
{
	auto p = new cReqDamage( );
	p->enemyId = enemyId;
	p->playerId = cPlayerManager::getInstance( )->getActivePlayerId( );
	p->damage = damage;
	cUDPClientManager::getInstance( )->send( p );
}
void cClientAdapter::sendRespawn( )
{
	auto p = new cReqRespawn( );
	p->playerId = cPlayerManager::getInstance( )->getActivePlayerId( );
	cUDPClientManager::getInstance( )->send( p );
}
void cClientAdapter::sendAddCannonPower( Network::ubyte1 teamId, Network::ubyte1 power )
{
	auto p = new cReqAddCannonPower( );
	p->teamId = teamId;
	p->playerId = cPlayerManager::getInstance( )->getActivePlayerId( );
	p->power = power;
	cUDPClientManager::getInstance( )->send( p );
}
void cClientAdapter::sendResult( )
{
	auto p = new cReqResult( );
	cUDPClientManager::getInstance( )->send( p );
}
void cClientAdapter::sendPlayerAttack( Network::ubyte1 playerId, Network::ubyte1 call )
{
	auto p = new cReqPlayerAttack( );
	p->playerId = playerId;
	p->call = call;
	cUDPClientManager::getInstance( )->send( p );
}
void cClientAdapter::sendBreakBlocks( )
{
    if ( !mBreakBlocksPecket->mBreakFormats.empty( ) )
    {
        cUDPClientManager::getInstance( )->send( mBreakBlocksPecket );
        mBreakBlocksPecket = new Packet::Request::cReqBreakBlocks( );
    }
}
}

#include <Game/cClientAdapter.h>
#include <Network/cUDPClientManager.h>
#include <Network/cDeliverManager.h>
#include <Network/cEventManager.h>
#include <Network/cRequestManager.h>
#include <Network/cResponseManager.h>
#include <Game/cGemManager.h>
#include <Game/cPlayerManager.h>
#include <Game/cStrategyManager.h>
#include <Game/cFieldManager.h>
namespace Game
{
cClientAdapter::cClientAdapter( )
    : mBreakBlocksPecket( new Network::Packet::Deliver::cDliBreakBlocks( ) )
{
}
cClientAdapter::~cClientAdapter( )
{
    delete mBreakBlocksPecket;
}
void cClientAdapter::update( )
{
    // �u���b�N�����͌X�̃^�C�~���O�ő��M���܂��B
    sendBreakBlocks( );

    // �T�[�o�[�����M�������̂��������ꍇ�͎��o���āA
    // �e�}�l�[�W���[�ɓ`���܂��B
    recvAllPlayersFormat( );
    recvAllQuarrys( );
    recvAllGems( );
    recvAllBreakBlocks( );
}
void cClientAdapter::recvAllPlayersFormat( )
{
    auto m = ::Network::cEventManager::getInstance( );
    while ( auto packet = m->getEvePlayers( ) )
    {
        for ( auto& o : packet->mPlayerFormats )
        {
            auto players = Game::cPlayerManager::getInstance( )->getPlayers( );
            for ( auto& player : players )
            {
                player->setPos( o.mPosition );
            }
        }
    }
}
void cClientAdapter::recvAllQuarrys( )
{
    auto m = ::Network::cEventManager::getInstance( );
    while ( auto packet = m->getEveSetQuarry( ) )
    {
        Game::cStrategyManager::getInstance( )->CreateDrill(
            packet->mPosition,
            packet->mDrillId,
            static_cast<Game::Strategy::cDrill::DrillType>( packet->mType ),
            packet->mTeamId == 0 // TODO: �`�[��ID�Ɣ�r����B
        );
    }
}
void cClientAdapter::recvAllGems( )
{
    auto eve = ::Network::cEventManager::getInstance( );
    while ( auto packet = eve->getEveGetJemQuarry( ) )
    {
        Game::cStrategyManager::getInstance( )->HitDrillToGem(
            packet->mDrillId,
            packet->mGemId
        );
    }
    while ( auto packet = eve->getEveGetJemPlayer( ) )
    {
        //Game::cPlayerManager::getInstance( )->mineGem(
        //    packet->mPlayerId,
        //    packet->mGemId
        //);
    }
    auto res = ::Network::cResponseManager::getInstance( );
    while ( auto packet = res->getResCheckGetJemQuarry( ) )
    {
        if ( !packet->mIsSuccessed ) continue;
        Game::cStrategyManager::getInstance( )->HitDrillToGem(
            packet->mDrillId,
            packet->mGemId
        );
    }
    while ( auto packet = res->getResCheckGetJemPlayer( ) )
    {
        if ( !packet->mIsSuccessed ) continue;
        //Game::cPlayerManager::getInstance( )->mineGem(
        //    packet->mPlayerId,
        //    packet->mGemId
        //);
    }
}
void cClientAdapter::recvAllBreakBlocks( )
{
    auto eve = ::Network::cEventManager::getInstance( );
    while ( auto packet = eve->getEveBreakBlocks( ) )
    {
        for ( auto& o : packet->mBreakPositions )
        {
            Game::cFieldManager::getInstance( )->blockBreak(
                o
            );
        }
    }
}
void cClientAdapter::sendBreakBlock( cinder::vec3 const & position )
{
    // �u���b�N�j��͈�U�o�b�t�@�ɋl�߂Ă����܂��B
    mBreakBlocksPecket->mBreakPositions.emplace_back( position );
}
void cClientAdapter::sendBreakBlocks( std::vector<cinder::vec3> const & positions )
{
    // �u���b�N�j��͈�U�o�b�t�@�ɋl�߂Ă����܂��B
    std::copy( positions.begin( ), positions.end( ), std::back_inserter( mBreakBlocksPecket->mBreakPositions ) );
}
void cClientAdapter::sendSetQuarry( cinder::vec3 const & position, Network::ubyte1 drillType )
{
    auto packet = new Network::Packet::Request::cReqCheckSetQuarry( );
    packet->mPosition = position;
    packet->mType = drillType;
    Network::cUDPClientManager::getInstance( )->send( packet );
}
void cClientAdapter::sendPlayerFormat( cinder::vec3 const & position, cinder::quat const & rotation )
{
    auto packet = new Network::Packet::Deliver::cDliPlayer( );
    packet->mPosition = position;
    packet->mRotation = rotation;
    Network::cUDPClientManager::getInstance( )->send( packet );
}
void cClientAdapter::sendGetGemPlayer( Network::ubyte2 gemId )
{
    auto packet = new Network::Packet::Request::cReqCheckGetJemPlayer( );
    packet->mGemId = gemId;
    Network::cUDPClientManager::getInstance( )->send( packet );
}
void cClientAdapter::sendGetGemQuarry( Network::ubyte1 drillId, Network::ubyte2 gemId )
{
    auto packet = new Network::Packet::Request::cReqCheckGetJemQuarry( );
    packet->mDrillId = drillId;
    packet->mGemId = gemId;
    Network::cUDPClientManager::getInstance( )->send( packet );
}
void cClientAdapter::sendBreakBlocks( )
{
    if ( !mBreakBlocksPecket->mBreakPositions.empty( ) )
    {
        Network::cUDPClientManager::getInstance( )->send( mBreakBlocksPecket );
        mBreakBlocksPecket = new Network::Packet::Deliver::cDliBreakBlocks( );
    }
}
}
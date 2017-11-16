#include <Network/cUDPManager.h>
#include <Network/Packet/PacketId.h>
#include <Network/cDeliverManager.h>
#include <Network/cEventManager.h>
#include <Network/cRequestManager.h>
#include <Network/cResponseManager.h>
namespace Network
{
bool cUDPManager::isConnectPacket( cPacketChunk const & packetChunk )
{
    cNetworkHandle const& networkHandle = packetChunk.networkHandle;
    cPacketBuffer const& packetBuffer = packetChunk.packetBuffer;
    ubyte2 const& transferredBytes = packetBuffer.transferredBytes;
    cBuffer const& buffer = packetBuffer.buffer;

	ubyte2 headerOffset = 0U;
    do
    {
        Packet::PacketHeader packetHeader;
        char const* bufferData = buffer.data( ) + headerOffset;
        memcpy( &packetHeader, bufferData, sizeof( Packet::PacketHeader ) );
        ubyte2 bufferSize = packetHeader.mPacketByte;
        switch ( packetHeader.mPacketId )
        {
        case Network::Packet::PacketId::REQ_CONNECT:
        {
            return true;
            break;
        }
        default:
            break;
        }
        headerOffset += packetHeader.mPacketByte;
        if ( headerOffset > transferredBytes )
        {
            throw std::exception( "Ç‚ÇŒÇ¢ÅB" );
        }
    
    } while ( headerOffset != transferredBytes );

    return false;
}
void cUDPManager::onReceive( cPacketChunk const & packetChunk )
{
    cNetworkHandle const& networkHandle = packetChunk.networkHandle;
    cPacketBuffer const& packetBuffer = packetChunk.packetBuffer;
    ubyte2 const& transferredBytes = packetBuffer.transferredBytes;
    cBuffer const& buffer = packetBuffer.buffer;

    ubyte2 headerOffset = 0U;
    do
    {
        Packet::PacketHeader packetHeader;
        char const* bufferData = buffer.data( ) + headerOffset;
        memcpy( &packetHeader, bufferData, sizeof( Packet::PacketHeader ) );
        ubyte2 bufferSize = packetHeader.mPacketByte;
        switch ( packetHeader.mPacketId )
        {
            // P=====BEGIN=====P
        case Network::Packet::PacketId::EVE_STRING:
        {
            if ( cEventManager::getInstance( )->isNewEveString( packetHeader ) )
			{
                Packet::Event::cEveString data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cEventManager::getInstance( )->ungetEveString( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::EVE_PING:
        {
            if ( cEventManager::getInstance( )->isNewEvePing( packetHeader ) )
			{
                Packet::Event::cEvePing data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cEventManager::getInstance( )->ungetEvePing( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::EVE_PLAYERS:
        {
            if ( cEventManager::getInstance( )->isNewEvePlayers( packetHeader ) )
			{
                Packet::Event::cEvePlayers data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cEventManager::getInstance( )->ungetEvePlayers( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::EVE_GET_JEM_PLAYER:
        {
            if ( cEventManager::getInstance( )->isNewEveGetJemPlayer( packetHeader ) )
			{
                Packet::Event::cEveGetJemPlayer data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cEventManager::getInstance( )->ungetEveGetJemPlayer( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::EVE_GET_JEM_QUARRY:
        {
            if ( cEventManager::getInstance( )->isNewEveGetJemQuarry( packetHeader ) )
			{
                Packet::Event::cEveGetJemQuarry data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cEventManager::getInstance( )->ungetEveGetJemQuarry( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::EVE_BREAK_BLOCKS:
        {
            if ( cEventManager::getInstance( )->isNewEveBreakBlocks( packetHeader ) )
			{
                Packet::Event::cEveBreakBlocks data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cEventManager::getInstance( )->ungetEveBreakBlocks( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::EVE_SET_QUARRY:
        {
            if ( cEventManager::getInstance( )->isNewEveSetQuarry( packetHeader ) )
			{
                Packet::Event::cEveSetQuarry data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cEventManager::getInstance( )->ungetEveSetQuarry( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::EVE_PLAYER_ROB_JEM:
        {
            if ( cEventManager::getInstance( )->isNewEvePlayerRobJem( packetHeader ) )
			{
                Packet::Event::cEvePlayerRobJem data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cEventManager::getInstance( )->ungetEvePlayerRobJem( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::EVE_PLAYER_DEATH:
        {
            if ( cEventManager::getInstance( )->isNewEvePlayerDeath( packetHeader ) )
			{
                Packet::Event::cEvePlayerDeath data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cEventManager::getInstance( )->ungetEvePlayerDeath( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::EVE_TEAM_MEMBER:
        {
            if ( cEventManager::getInstance( )->isNewEveTeamMember( packetHeader ) )
			{
                Packet::Event::cEveTeamMember data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cEventManager::getInstance( )->ungetEveTeamMember( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::EVE_PLAYERS_RESPAWN:
        {
            if ( cEventManager::getInstance( )->isNewEvePlayersRespawn( packetHeader ) )
			{
                Packet::Event::cEvePlayersRespawn data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cEventManager::getInstance( )->ungetEvePlayersRespawn( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::DLI_STRING:
        {
            if ( cDeliverManager::getInstance( )->isNewDliString( packetHeader ) )
			{
                Packet::Deliver::cDliString data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cDeliverManager::getInstance( )->ungetDliString( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::DLI_PING:
        {
            if ( cDeliverManager::getInstance( )->isNewDliPing( packetHeader ) )
			{
                Packet::Deliver::cDliPing data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cDeliverManager::getInstance( )->ungetDliPing( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::DLI_PLAYER:
        {
            if ( cDeliverManager::getInstance( )->isNewDliPlayer( packetHeader ) )
			{
                Packet::Deliver::cDliPlayer data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cDeliverManager::getInstance( )->ungetDliPlayer( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::DLI_BREAK_BLOCKS:
        {
            if ( cDeliverManager::getInstance( )->isNewDliBreakBlocks( packetHeader ) )
			{
                Packet::Deliver::cDliBreakBlocks data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cDeliverManager::getInstance( )->ungetDliBreakBlocks( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::REQ_STRING:
        {
            if ( cRequestManager::getInstance( )->isNewReqString( packetHeader ) )
			{
                Packet::Request::cReqString data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cRequestManager::getInstance( )->ungetReqString( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::REQ_CONNECT:
        {
            if ( cRequestManager::getInstance( )->isNewReqConnect( packetHeader ) )
			{
                Packet::Request::cReqConnect data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cRequestManager::getInstance( )->ungetReqConnect( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::REQ_GET_JEM_SEED:
        {
            if ( cRequestManager::getInstance( )->isNewReqGetJemSeed( packetHeader ) )
			{
                Packet::Request::cReqGetJemSeed data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cRequestManager::getInstance( )->ungetReqGetJemSeed( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::REQ_GET_JEM_POINT:
        {
            if ( cRequestManager::getInstance( )->isNewReqGetJemPoint( packetHeader ) )
			{
                Packet::Request::cReqGetJemPoint data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cRequestManager::getInstance( )->ungetReqGetJemPoint( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::REQ_CHECK_GET_JEM_PLAYER:
        {
            if ( cRequestManager::getInstance( )->isNewReqCheckGetJemPlayer( packetHeader ) )
			{
                Packet::Request::cReqCheckGetJemPlayer data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cRequestManager::getInstance( )->ungetReqCheckGetJemPlayer( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::REQ_CHECK_GET_JEM_QUARRY:
        {
            if ( cRequestManager::getInstance( )->isNewReqCheckGetJemQuarry( packetHeader ) )
			{
                Packet::Request::cReqCheckGetJemQuarry data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cRequestManager::getInstance( )->ungetReqCheckGetJemQuarry( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::REQ_CHECK_PLAYER_ROB_JEM:
        {
            if ( cRequestManager::getInstance( )->isNewReqCheckPlayerRobJem( packetHeader ) )
			{
                Packet::Request::cReqCheckPlayerRobJem data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cRequestManager::getInstance( )->ungetReqCheckPlayerRobJem( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::REQ_CHECK_SET_QUARRY:
        {
            if ( cRequestManager::getInstance( )->isNewReqCheckSetQuarry( packetHeader ) )
			{
                Packet::Request::cReqCheckSetQuarry data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cRequestManager::getInstance( )->ungetReqCheckSetQuarry( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::REQ_CHECK_PLAYER_DEATH:
        {
            if ( cRequestManager::getInstance( )->isNewReqCheckPlayerDeath( packetHeader ) )
			{
                Packet::Request::cReqCheckPlayerDeath data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cRequestManager::getInstance( )->ungetReqCheckPlayerDeath( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::REQ_MAKE_ROOM:
        {
            if ( cRequestManager::getInstance( )->isNewReqMakeRoom( packetHeader ) )
			{
                Packet::Request::cReqMakeRoom data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cRequestManager::getInstance( )->ungetReqMakeRoom( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::REQ_IN_ROOM:
        {
            if ( cRequestManager::getInstance( )->isNewReqInRoom( packetHeader ) )
			{
                Packet::Request::cReqInRoom data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cRequestManager::getInstance( )->ungetReqInRoom( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::REQ_WANT_TEAM_IN:
        {
            if ( cRequestManager::getInstance( )->isNewReqWantTeamIn( packetHeader ) )
			{
                Packet::Request::cReqWantTeamIn data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cRequestManager::getInstance( )->ungetReqWantTeamIn( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::REQ_CHECK_BEGIN_GAME:
        {
            if ( cRequestManager::getInstance( )->isNewReqCheckBeginGame( packetHeader ) )
			{
                Packet::Request::cReqCheckBeginGame data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cRequestManager::getInstance( )->ungetReqCheckBeginGame( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::REQ_CHECK_MEMBER:
        {
            if ( cRequestManager::getInstance( )->isNewReqCheckMember( packetHeader ) )
			{
                Packet::Request::cReqCheckMember data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cRequestManager::getInstance( )->ungetReqCheckMember( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::REQ_END_GAMEMAIN_SETUP:
        {
            if ( cRequestManager::getInstance( )->isNewReqEndGamemainSetup( packetHeader ) )
			{
                Packet::Request::cReqEndGamemainSetup data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cRequestManager::getInstance( )->ungetReqEndGamemainSetup( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::RES_STRING:
        {
            if ( cResponseManager::getInstance( )->isNewResString( packetHeader ) )
			{
                Packet::Response::cResString data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cResponseManager::getInstance( )->ungetResString( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::RES_CONNECT:
        {
            if ( cResponseManager::getInstance( )->isNewResConnect( packetHeader ) )
			{
                Packet::Response::cResConnect data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cResponseManager::getInstance( )->ungetResConnect( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::RES_GET_JEM_SEED:
        {
            if ( cResponseManager::getInstance( )->isNewResGetJemSeed( packetHeader ) )
			{
                Packet::Response::cResGetJemSeed data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cResponseManager::getInstance( )->ungetResGetJemSeed( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::RES_GET_JEM_POINT:
        {
            if ( cResponseManager::getInstance( )->isNewResGetJemPoint( packetHeader ) )
			{
                Packet::Response::cResGetJemPoint data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cResponseManager::getInstance( )->ungetResGetJemPoint( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::RES_CHECK_GET_JEM_PLAYER:
        {
            if ( cResponseManager::getInstance( )->isNewResCheckGetJemPlayer( packetHeader ) )
			{
                Packet::Response::cResCheckGetJemPlayer data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cResponseManager::getInstance( )->ungetResCheckGetJemPlayer( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::RES_CHECK_GET_JEM_QUARRY:
        {
            if ( cResponseManager::getInstance( )->isNewResCheckGetJemQuarry( packetHeader ) )
			{
                Packet::Response::cResCheckGetJemQuarry data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cResponseManager::getInstance( )->ungetResCheckGetJemQuarry( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::RES_CHECK_PLAYER_ROB_JEM:
        {
            if ( cResponseManager::getInstance( )->isNewResCheckPlayerRobJem( packetHeader ) )
			{
                Packet::Response::cResCheckPlayerRobJem data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cResponseManager::getInstance( )->ungetResCheckPlayerRobJem( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::RES_CHECK_SET_QUARRY:
        {
            if ( cResponseManager::getInstance( )->isNewResCheckSetQuarry( packetHeader ) )
			{
                Packet::Response::cResCheckSetQuarry data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cResponseManager::getInstance( )->ungetResCheckSetQuarry( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::RES_CHECK_PLAYER_DEATH:
        {
            if ( cResponseManager::getInstance( )->isNewResCheckPlayerDeath( packetHeader ) )
			{
                Packet::Response::cResCheckPlayerDeath data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cResponseManager::getInstance( )->ungetResCheckPlayerDeath( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::RES_MAKE_ROOM:
        {
            if ( cResponseManager::getInstance( )->isNewResMakeRoom( packetHeader ) )
			{
                Packet::Response::cResMakeRoom data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cResponseManager::getInstance( )->ungetResMakeRoom( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::RES_IN_ROOM:
        {
            if ( cResponseManager::getInstance( )->isNewResInRoom( packetHeader ) )
			{
                Packet::Response::cResInRoom data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cResponseManager::getInstance( )->ungetResInRoom( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::RES_WANT_TEAM_IN:
        {
            if ( cResponseManager::getInstance( )->isNewResWantTeamIn( packetHeader ) )
			{
                Packet::Response::cResWantTeamIn data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cResponseManager::getInstance( )->ungetResWantTeamIn( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::RES_CHECK_BEGIN_GAME:
        {
            if ( cResponseManager::getInstance( )->isNewResCheckBeginGame( packetHeader ) )
			{
                Packet::Response::cResCheckBeginGame data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cResponseManager::getInstance( )->ungetResCheckBeginGame( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::RES_CHECK_MEMBER:
        {
            if ( cResponseManager::getInstance( )->isNewResCheckMember( packetHeader ) )
			{
                Packet::Response::cResCheckMember data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cResponseManager::getInstance( )->ungetResCheckMember( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::RES_END_GAMEMAIN_SETUP:
        {
            if ( cResponseManager::getInstance( )->isNewResEndGamemainSetup( packetHeader ) )
			{
                Packet::Response::cResEndGamemainSetup data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cResponseManager::getInstance( )->ungetResEndGamemainSetup( std::move( data ) );
            }
            break;
        }
        case Network::Packet::PacketId::RES_SET_GAMESTART_TIMER:
        {
            if ( cResponseManager::getInstance( )->isNewResSetGamestartTimer( packetHeader ) )
			{
                Packet::Response::cResSetGamestartTimer data;
                data.onReceive( networkHandle, bufferSize, bufferData );
                cResponseManager::getInstance( )->ungetResSetGamestartTimer( std::move( data ) );
            }
            break;
        }
        // P=====END=====P
        default:
            break;
        }
        headerOffset += packetHeader.mPacketByte;
        if ( headerOffset > transferredBytes )
        {
            throw std::exception( "Ç‚ÇŒÇ¢ÅB" );
        }
    } while ( headerOffset != transferredBytes );
}
}

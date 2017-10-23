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

    if ( transferredBytes != sizeof( Packet::PacketHeader ) ) return false;

    Packet::PacketHeader packetHeader;
    char const* bufferData = buffer.data( );
    memcpy( &packetHeader, bufferData, sizeof( Packet::PacketHeader ) );
    ubyte2 bufferSize = packetHeader.mPacketByte;
    return packetHeader.mPacketId == Network::Packet::PacketId::REQ_CONNECT;
}
void cUDPManager::onReceive( cPacketChunk const & packetChunk )
{
    cNetworkHandle const& networkHandle = packetChunk.networkHandle;
    cPacketBuffer const& packetBuffer = packetChunk.packetBuffer;
    ubyte2 const& transferredBytes = packetBuffer.transferredBytes;
    cBuffer const& buffer = packetBuffer.buffer;

    ubyte2 headerOffset = 0;
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
            Packet::Event::cEveString data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cEventManager::getInstance( )->ungetEveString( std::move( data ) );
            break;
        }
        case Network::Packet::PacketId::EVE_PING:
        {
            Packet::Event::cEvePing data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cEventManager::getInstance( )->ungetEvePing( std::move( data ) );
            break;
        }
        case Network::Packet::PacketId::EVE_PLAYERS:
        {
            Packet::Event::cEvePlayers data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cEventManager::getInstance( )->ungetEvePlayers( std::move( data ) );
            break;
        }
        case Network::Packet::PacketId::EVE_GET_JEM_PLAYER:
        {
            Packet::Event::cEveGetJemPlayer data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cEventManager::getInstance( )->ungetEveGetJemPlayer( std::move( data ) );
            break;
        }
        case Network::Packet::PacketId::EVE_GET_JEM_QUARRY:
        {
            Packet::Event::cEveGetJemQuarry data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cEventManager::getInstance( )->ungetEveGetJemQuarry( std::move( data ) );
            break;
        }
        case Network::Packet::PacketId::EVE_BREAK_BLOCKS:
        {
            Packet::Event::cEveBreakBlocks data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cEventManager::getInstance( )->ungetEveBreakBlocks( std::move( data ) );
            break;
        }
        case Network::Packet::PacketId::EVE_SET_QUARRY:
        {
            Packet::Event::cEveSetQuarry data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cEventManager::getInstance( )->ungetEveSetQuarry( std::move( data ) );
            break;
        }
        case Network::Packet::PacketId::EVE_PLAYER_ROB_JEM:
        {
            Packet::Event::cEvePlayerRobJem data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cEventManager::getInstance( )->ungetEvePlayerRobJem( std::move( data ) );
            break;
        }
        case Network::Packet::PacketId::EVE_PLAYER_DEATH:
        {
            Packet::Event::cEvePlayerDeath data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cEventManager::getInstance( )->ungetEvePlayerDeath( std::move( data ) );
            break;
        }
        case Network::Packet::PacketId::EVE_TEAM_MEMBER:
        {
            Packet::Event::cEveTeamMember data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cEventManager::getInstance( )->ungetEveTeamMember( std::move( data ) );
            break;
        }
        case Network::Packet::PacketId::DLI_STRING:
        {
            Packet::Deliver::cDliString data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cDeliverManager::getInstance( )->ungetDliString( std::move( data ) );
            break;
        }
        case Network::Packet::PacketId::DLI_PING:
        {
            Packet::Deliver::cDliPing data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cDeliverManager::getInstance( )->ungetDliPing( std::move( data ) );
            break;
        }
        case Network::Packet::PacketId::DLI_PLAYER:
        {
            Packet::Deliver::cDliPlayer data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cDeliverManager::getInstance( )->ungetDliPlayer( std::move( data ) );
            break;
        }
        case Network::Packet::PacketId::DLI_BREAK_BLOCKS:
        {
            Packet::Deliver::cDliBreakBlocks data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cDeliverManager::getInstance( )->ungetDliBreakBlocks( std::move( data ) );
            break;
        }
        case Network::Packet::PacketId::REQ_STRING:
        {
            Packet::Request::cReqString data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cRequestManager::getInstance( )->ungetReqString( std::move( data ) );
            break;
        }
        case Network::Packet::PacketId::REQ_CONNECT:
        {
            Packet::Request::cReqConnect data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cRequestManager::getInstance( )->ungetReqConnect( std::move( data ) );
            break;
        }
        case Network::Packet::PacketId::REQ_GET_JEM_SEED:
        {
            Packet::Request::cReqGetJemSeed data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cRequestManager::getInstance( )->ungetReqGetJemSeed( std::move( data ) );
            break;
        }
        case Network::Packet::PacketId::REQ_GET_JEM_POINT:
        {
            Packet::Request::cReqGetJemPoint data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cRequestManager::getInstance( )->ungetReqGetJemPoint( std::move( data ) );
            break;
        }
        case Network::Packet::PacketId::REQ_CHECK_GET_JEM_PLAYER:
        {
            Packet::Request::cReqCheckGetJemPlayer data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cRequestManager::getInstance( )->ungetReqCheckGetJemPlayer( std::move( data ) );
            break;
        }
        case Network::Packet::PacketId::REQ_CHECK_GET_JEM_QUARRY:
        {
            Packet::Request::cReqCheckGetJemQuarry data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cRequestManager::getInstance( )->ungetReqCheckGetJemQuarry( std::move( data ) );
            break;
        }
        case Network::Packet::PacketId::REQ_CHECK_PLAYER_ROB_JEM:
        {
            Packet::Request::cReqCheckPlayerRobJem data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cRequestManager::getInstance( )->ungetReqCheckPlayerRobJem( std::move( data ) );
            break;
        }
        case Network::Packet::PacketId::REQ_CHECK_SET_QUARRY:
        {
            Packet::Request::cReqCheckSetQuarry data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cRequestManager::getInstance( )->ungetReqCheckSetQuarry( std::move( data ) );
            break;
        }
        case Network::Packet::PacketId::REQ_CHECK_PLAYER_DEATH:
        {
            Packet::Request::cReqCheckPlayerDeath data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cRequestManager::getInstance( )->ungetReqCheckPlayerDeath( std::move( data ) );
            break;
        }
        case Network::Packet::PacketId::REQ_MAKE_ROOM:
        {
            Packet::Request::cReqMakeRoom data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cRequestManager::getInstance( )->ungetReqMakeRoom( std::move( data ) );
            break;
        }
        case Network::Packet::PacketId::REQ_IN_ROOM:
        {
            Packet::Request::cReqInRoom data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cRequestManager::getInstance( )->ungetReqInRoom( std::move( data ) );
            break;
        }
        case Network::Packet::PacketId::REQ_WANT_TEAM_IN:
        {
            Packet::Request::cReqWantTeamIn data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cRequestManager::getInstance( )->ungetReqWantTeamIn( std::move( data ) );
            break;
        }
        case Network::Packet::PacketId::REQ_CHECK_BEGIN_GAME:
        {
            Packet::Request::cReqCheckBeginGame data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cRequestManager::getInstance( )->ungetReqCheckBeginGame( std::move( data ) );
            break;
        }
        case Network::Packet::PacketId::REQ_CHECK_MEMBER:
        {
            Packet::Request::cReqCheckMember data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cRequestManager::getInstance( )->ungetReqCheckMember( std::move( data ) );
            break;
        }
        case Network::Packet::PacketId::RES_STRING:
        {
            Packet::Response::cResString data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cResponseManager::getInstance( )->ungetResString( std::move( data ) );
            break;
        }
        case Network::Packet::PacketId::RES_CONNECT:
        {
            Packet::Response::cResConnect data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cResponseManager::getInstance( )->ungetResConnect( std::move( data ) );
            break;
        }
        case Network::Packet::PacketId::RES_GET_JEM_SEED:
        {
            Packet::Response::cResGetJemSeed data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cResponseManager::getInstance( )->ungetResGetJemSeed( std::move( data ) );
            break;
        }
        case Network::Packet::PacketId::RES_GET_JEM_POINT:
        {
            Packet::Response::cResGetJemPoint data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cResponseManager::getInstance( )->ungetResGetJemPoint( std::move( data ) );
            break;
        }
        case Network::Packet::PacketId::RES_CHECK_GET_JEM_PLAYER:
        {
            Packet::Response::cResCheckGetJemPlayer data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cResponseManager::getInstance( )->ungetResCheckGetJemPlayer( std::move( data ) );
            break;
        }
        case Network::Packet::PacketId::RES_CHECK_GET_JEM_QUARRY:
        {
            Packet::Response::cResCheckGetJemQuarry data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cResponseManager::getInstance( )->ungetResCheckGetJemQuarry( std::move( data ) );
            break;
        }
        case Network::Packet::PacketId::RES_CHECK_PLAYER_ROB_JEM:
        {
            Packet::Response::cResCheckPlayerRobJem data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cResponseManager::getInstance( )->ungetResCheckPlayerRobJem( std::move( data ) );
            break;
        }
        case Network::Packet::PacketId::RES_CHECK_SET_QUARRY:
        {
            Packet::Response::cResCheckSetQuarry data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cResponseManager::getInstance( )->ungetResCheckSetQuarry( std::move( data ) );
            break;
        }
        case Network::Packet::PacketId::RES_CHECK_PLAYER_DEATH:
        {
            Packet::Response::cResCheckPlayerDeath data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cResponseManager::getInstance( )->ungetResCheckPlayerDeath( std::move( data ) );
            break;
        }
        case Network::Packet::PacketId::RES_MAKE_ROOM:
        {
            Packet::Response::cResMakeRoom data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cResponseManager::getInstance( )->ungetResMakeRoom( std::move( data ) );
            break;
        }
        case Network::Packet::PacketId::RES_IN_ROOM:
        {
            Packet::Response::cResInRoom data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cResponseManager::getInstance( )->ungetResInRoom( std::move( data ) );
            break;
        }
        case Network::Packet::PacketId::RES_WANT_TEAM_IN:
        {
            Packet::Response::cResWantTeamIn data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cResponseManager::getInstance( )->ungetResWantTeamIn( std::move( data ) );
            break;
        }
        case Network::Packet::PacketId::RES_CHECK_BEGIN_GAME:
        {
            Packet::Response::cResCheckBeginGame data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cResponseManager::getInstance( )->ungetResCheckBeginGame( std::move( data ) );
            break;
        }
        case Network::Packet::PacketId::RES_CHECK_MEMBER:
        {
            Packet::Response::cResCheckMember data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cResponseManager::getInstance( )->ungetResCheckMember( std::move( data ) );
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

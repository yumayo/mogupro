#include <Network/cUDPManager.h>
#include <Network/Packet/PacketId.h>
#include <Network/cEventManager.h>
#include <Network/cRequestManager.h>
#include <Network/cResponseManager.h>
namespace Network
{
void cUDPManager::close( )
{
    socket.close( );
}
void cUDPManager::open( )
{
    socket.open( );
}
void cUDPManager::open( int port )
{
    socket.open( port );
}
void cUDPManager::update( )
{
    while ( !socket.emptyRemoteBuffer( ) )
    {
        auto&& buffer = socket.popRemoteBuffer( );
        onReceive( buffer );
    }
}
void cUDPManager::onReceive( cPacketRaw const & raw )
{
    ubyte2 const& packetByte = std::get<0>( raw );
    cBuffer const& buffer = std::get<1>( raw );

    Packet::PacketId packetId;
    memcpy( &packetId, buffer.data( ), sizeof( Packet::PacketId ) );

    switch ( packetId )
    {
    // P=====BEGIN=====P
    case Network::Packet::PacketId::EVE_STRING:
    {
        Packet::Event::cEveString data;
        data.onReceive( raw );
        cEventManager::getInstance( )->ungetEveString( data );
        break;
    }
    case Network::Packet::PacketId::EVE_GET_JEM:
    {
        Packet::Event::cEveGetJem data;
        data.onReceive( raw );
        cEventManager::getInstance( )->ungetEveGetJem( data );
        break;
    }
    case Network::Packet::PacketId::EVE_PLAYER_ROB_JEM:
    {
        Packet::Event::cEvePlayerRobJem data;
        data.onReceive( raw );
        cEventManager::getInstance( )->ungetEvePlayerRobJem( data );
        break;
    }
    case Network::Packet::PacketId::EVE_PLAYER_DEATH:
    {
        Packet::Event::cEvePlayerDeath data;
        data.onReceive( raw );
        cEventManager::getInstance( )->ungetEvePlayerDeath( data );
        break;
    }
    case Network::Packet::PacketId::REQ_STRING:
    {
        Packet::Request::cReqString data;
        data.onReceive( raw );
        cRequestManager::getInstance( )->ungetReqString( data );
        break;
    }
    case Network::Packet::PacketId::REQ_PLAYER:
    {
        Packet::Request::cReqPlayer data;
        data.onReceive( raw );
        cRequestManager::getInstance( )->ungetReqPlayer( data );
        break;
    }
    case Network::Packet::PacketId::REQ_GET_JEM_SEED:
    {
        Packet::Request::cReqGetJemSeed data;
        data.onReceive( raw );
        cRequestManager::getInstance( )->ungetReqGetJemSeed( data );
        break;
    }
    case Network::Packet::PacketId::REQ_GET_JEM_POINT:
    {
        Packet::Request::cReqGetJemPoint data;
        data.onReceive( raw );
        cRequestManager::getInstance( )->ungetReqGetJemPoint( data );
        break;
    }
    case Network::Packet::PacketId::REQ_CHECK_GET_JEM:
    {
        Packet::Request::cReqCheckGetJem data;
        data.onReceive( raw );
        cRequestManager::getInstance( )->ungetReqCheckGetJem( data );
        break;
    }
    case Network::Packet::PacketId::REQ_CHECK_PLAYER_ROB_JEM:
    {
        Packet::Request::cReqCheckPlayerRobJem data;
        data.onReceive( raw );
        cRequestManager::getInstance( )->ungetReqCheckPlayerRobJem( data );
        break;
    }
    case Network::Packet::PacketId::REQ_CHECK_BRAKE_BLOCKS:
    {
        Packet::Request::cReqCheckBrakeBlocks data;
        data.onReceive( raw );
        cRequestManager::getInstance( )->ungetReqCheckBrakeBlocks( data );
        break;
    }
    case Network::Packet::PacketId::REQ_CHECK_SET_QUARRY:
    {
        Packet::Request::cReqCheckSetQuarry data;
        data.onReceive( raw );
        cRequestManager::getInstance( )->ungetReqCheckSetQuarry( data );
        break;
    }
    case Network::Packet::PacketId::REQ_CHECK_PLAYER_DEATH:
    {
        Packet::Request::cReqCheckPlayerDeath data;
        data.onReceive( raw );
        cRequestManager::getInstance( )->ungetReqCheckPlayerDeath( data );
        break;
    }
    case Network::Packet::PacketId::RES_STRING:
    {
        Packet::Response::cResString data;
        data.onReceive( raw );
        cResponseManager::getInstance( )->ungetResString( data );
        break;
    }
    case Network::Packet::PacketId::RES_PLAYER:
    {
        Packet::Response::cResPlayer data;
        data.onReceive( raw );
        cResponseManager::getInstance( )->ungetResPlayer( data );
        break;
    }
    case Network::Packet::PacketId::RES_GET_JEM_SEED:
    {
        Packet::Response::cResGetJemSeed data;
        data.onReceive( raw );
        cResponseManager::getInstance( )->ungetResGetJemSeed( data );
        break;
    }
    case Network::Packet::PacketId::RES_GET_JEM_POINT:
    {
        Packet::Response::cResGetJemPoint data;
        data.onReceive( raw );
        cResponseManager::getInstance( )->ungetResGetJemPoint( data );
        break;
    }
    case Network::Packet::PacketId::RES_CHECK_GET_JEM:
    {
        Packet::Response::cResCheckGetJem data;
        data.onReceive( raw );
        cResponseManager::getInstance( )->ungetResCheckGetJem( data );
        break;
    }
    case Network::Packet::PacketId::RES_CHECK_PLAYER_ROB_JEM:
    {
        Packet::Response::cResCheckPlayerRobJem data;
        data.onReceive( raw );
        cResponseManager::getInstance( )->ungetResCheckPlayerRobJem( data );
        break;
    }
    case Network::Packet::PacketId::RES_CHECK_BRAKE_BLOCKS:
    {
        Packet::Response::cResCheckBrakeBlocks data;
        data.onReceive( raw );
        cResponseManager::getInstance( )->ungetResCheckBrakeBlocks( data );
        break;
    }
    case Network::Packet::PacketId::RES_CHECK_SET_QUARRY:
    {
        Packet::Response::cResCheckSetQuarry data;
        data.onReceive( raw );
        cResponseManager::getInstance( )->ungetResCheckSetQuarry( data );
        break;
    }
    case Network::Packet::PacketId::RES_CHECK_PLAYER_DEATH:
    {
        Packet::Response::cResCheckPlayerDeath data;
        data.onReceive( raw );
        cResponseManager::getInstance( )->ungetResCheckPlayerDeath( data );
        break;
    }
    // P=====END=====P
    default:
        break;
    }
}
}

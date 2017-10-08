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
    case Network::Packet::PacketId::EVE_STRING:
    {
        Packet::Event::cEveString data;
        data.onReceive( raw );
        cEventManager::getInstance( )->ungetEveString( data );
        break;
    }
    case Network::Packet::PacketId::EVE_GET_JEM:
        break;
    case Network::Packet::PacketId::EVE_PLAYER_ROB_JEM:
        break;
    case Network::Packet::PacketId::REQ_STRING:
    {
        Packet::Request::cReqString data;
        data.onReceive( raw );
        cRequestManager::getInstance( )->ungetReqString( data );
        break;
    }
    case Network::Packet::PacketId::REQ_PLAYER:
        break;
    case Network::Packet::PacketId::REQ_GET_JEM_SEED:
        break;
    case Network::Packet::PacketId::REQ_GET_JEM_POINT:
        break;
    case Network::Packet::PacketId::REQ_GET_JEM:
        break;
    case Network::Packet::PacketId::REQ_PLAYER_ROB_JEM:
        break;
    case Network::Packet::PacketId::REQ_BREKE_BLOCKS:
        break;
    case Network::Packet::PacketId::REQ_SET_QUARRY:
        break;
    case Network::Packet::PacketId::RES_STRING:
    {
        Packet::Response::cResString data;
        data.onReceive( raw );
        cResponseManager::getInstance( )->ungetResString( data );
        break;
    }
    case Network::Packet::PacketId::RES_PLAYER:
        break;
    case Network::Packet::PacketId::RES_GET_JEM_SEED:
        break;
    case Network::Packet::PacketId::RES_GET_JEM_POINT:
        break;
    case Network::Packet::PacketId::RES_GET_JEM:
        break;
    case Network::Packet::PacketId::RES_PLAYER_ROB_JEM:
        break;
    case Network::Packet::PacketId::RES_BREKE_BLOCKS:
        break;
    case Network::Packet::PacketId::RES_SET_QUARRY:
        break;
    default:
        break;
    }
}
}
#include <Network/cUDPManager.h>
#include <Network/Packet/PacketId.h>
#include <Network/cEventManager.h>
#include <Network/cRequestManager.h>
#include <Network/cResponseManager.h>
namespace Network
{
void cUDPManager::close( )
{
    mSocket.close( );
}
void cUDPManager::open( )
{
    mSocket.open( );
}
void cUDPManager::open( int port )
{
    mSocket.open( port );
}
void cUDPManager::update( )
{
    while ( !mSocket.emptyChunk( ) )
    {
        auto chunk = mSocket.popChunk( );
        onReceive( chunk );
    }

    for ( auto& buffer : mSendDataBuffer )
    {
        // 余ってたらパケットを送ります。
        if ( !buffer.second.empty( ) )
        {
            mSocket.write( buffer.first, buffer.second.size( ), buffer.second.data( ) );
            buffer.second.clear( );
            buffer.second.shrink_to_fit( );
        }
    }
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
            cEventManager::getInstance( )->ungetEveString( data );
            break;
        }
        case Network::Packet::PacketId::EVE_GET_JEM:
        {
            Packet::Event::cEveGetJem data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cEventManager::getInstance( )->ungetEveGetJem( data );
            break;
        }
        case Network::Packet::PacketId::EVE_PLAYER_ROB_JEM:
        {
            Packet::Event::cEvePlayerRobJem data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cEventManager::getInstance( )->ungetEvePlayerRobJem( data );
            break;
        }
        case Network::Packet::PacketId::EVE_PLAYER_DEATH:
        {
            Packet::Event::cEvePlayerDeath data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cEventManager::getInstance( )->ungetEvePlayerDeath( data );
            break;
        }
        case Network::Packet::PacketId::REQ_STRING:
        {
            Packet::Request::cReqString data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cRequestManager::getInstance( )->ungetReqString( data );
            break;
        }
        case Network::Packet::PacketId::REQ_PLAYER:
        {
            Packet::Request::cReqPlayer data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cRequestManager::getInstance( )->ungetReqPlayer( data );
            break;
        }
        case Network::Packet::PacketId::REQ_GET_JEM_SEED:
        {
            Packet::Request::cReqGetJemSeed data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cRequestManager::getInstance( )->ungetReqGetJemSeed( data );
            break;
        }
        case Network::Packet::PacketId::REQ_GET_JEM_POINT:
        {
            Packet::Request::cReqGetJemPoint data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cRequestManager::getInstance( )->ungetReqGetJemPoint( data );
            break;
        }
        case Network::Packet::PacketId::REQ_CHECK_GET_JEM:
        {
            Packet::Request::cReqCheckGetJem data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cRequestManager::getInstance( )->ungetReqCheckGetJem( data );
            break;
        }
        case Network::Packet::PacketId::REQ_CHECK_PLAYER_ROB_JEM:
        {
            Packet::Request::cReqCheckPlayerRobJem data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cRequestManager::getInstance( )->ungetReqCheckPlayerRobJem( data );
            break;
        }
        case Network::Packet::PacketId::REQ_CHECK_BRAKE_BLOCKS:
        {
            Packet::Request::cReqCheckBreakBlocks data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cRequestManager::getInstance( )->ungetReqCheckBrakeBlocks( data );
            break;
        }
        case Network::Packet::PacketId::REQ_CHECK_SET_QUARRY:
        {
            Packet::Request::cReqCheckSetQuarry data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cRequestManager::getInstance( )->ungetReqCheckSetQuarry( data );
            break;
        }
        case Network::Packet::PacketId::REQ_CHECK_PLAYER_DEATH:
        {
            Packet::Request::cReqCheckPlayerDeath data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cRequestManager::getInstance( )->ungetReqCheckPlayerDeath( data );
            break;
        }
        case Network::Packet::PacketId::RES_STRING:
        {
            Packet::Response::cResString data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cResponseManager::getInstance( )->ungetResString( data );
            break;
        }
        case Network::Packet::PacketId::RES_PLAYER:
        {
            Packet::Response::cResPlayer data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cResponseManager::getInstance( )->ungetResPlayer( data );
            break;
        }
        case Network::Packet::PacketId::RES_GET_JEM_SEED:
        {
            Packet::Response::cResGetJemSeed data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cResponseManager::getInstance( )->ungetResGetJemSeed( data );
            break;
        }
        case Network::Packet::PacketId::RES_GET_JEM_POINT:
        {
            Packet::Response::cResGetJemPoint data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cResponseManager::getInstance( )->ungetResGetJemPoint( data );
            break;
        }
        case Network::Packet::PacketId::RES_CHECK_GET_JEM:
        {
            Packet::Response::cResCheckGetJem data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cResponseManager::getInstance( )->ungetResCheckGetJem( data );
            break;
        }
        case Network::Packet::PacketId::RES_CHECK_PLAYER_ROB_JEM:
        {
            Packet::Response::cResCheckPlayerRobJem data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cResponseManager::getInstance( )->ungetResCheckPlayerRobJem( data );
            break;
        }
        case Network::Packet::PacketId::RES_CHECK_BREAK_BLOCKS:
        {
            Packet::Response::cResCheckBrakeBlocks data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cResponseManager::getInstance( )->ungetResCheckBrakeBlocks( data );
            break;
        }
        case Network::Packet::PacketId::RES_CHECK_SET_QUARRY:
        {
            Packet::Response::cResCheckSetQuarry data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cResponseManager::getInstance( )->ungetResCheckSetQuarry( data );
            break;
        }
        case Network::Packet::PacketId::RES_CHECK_PLAYER_DEATH:
        {
            Packet::Response::cResCheckPlayerDeath data;
            data.onReceive( networkHandle, bufferSize, bufferData );
            cResponseManager::getInstance( )->ungetResCheckPlayerDeath( data );
            break;
        }
        // P=====END=====P
        default:
            break;
        }
        headerOffset += packetHeader.mPacketByte;
        if ( headerOffset > transferredBytes )
        {
            throw std::exception( "やばい。" );
        }
    } while ( headerOffset != transferredBytes );
}
void cUDPManager::sendDataBufferAdd( cNetworkHandle const& networkHandle, cPacketBuffer const & packetBuffer )
{
    auto& buf = mSendDataBuffer[networkHandle];

    // パケットが大きくなりそうなら先に送ってしまいます。
    if ( 1024 < buf.size( ) )
    {
        mSocket.write( networkHandle, buf.size( ), buf.data( ) );
        buf.clear( );
        buf.shrink_to_fit( );
    }

    ubyte2 const& byte = packetBuffer.transferredBytes;
    cBuffer const& buffer = packetBuffer.buffer;

    buf.resize( buf.size( ) + byte );
    memcpy( buf.data( ) + buf.size( ) - byte, &buffer, byte );
}
}

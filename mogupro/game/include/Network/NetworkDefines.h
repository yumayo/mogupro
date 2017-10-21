#pragma once
#include <array>
#include <tuple>
namespace Network
{
static_assert( sizeof( unsigned char ) == 1, "<unsinged char>が\"1バイト\"じゃありません。" );
static_assert( sizeof( unsigned short ) == 2, "<unsinged short>が\"2バイト\"じゃありません。" );
static_assert( sizeof( unsigned int ) == 4, "<unsinged int>が\"4バイト\"じゃありません。" );
using ubyte1 = unsigned char;
using ubyte2 = unsigned short;
using ubyte4 = unsigned int;
constexpr ubyte2 PACKET_MAX_BYTE = 4096U;
using cBuffer = std::array<char, PACKET_MAX_BYTE>;
class cPacketBuffer
{
public:
    cPacketBuffer( ) = default;
    cPacketBuffer( ubyte2 transferredBytes, cBuffer const& buffer )
        : transferredBytes( transferredBytes )
        , buffer( buffer )
    {

    }
    ubyte2 transferredBytes;
    cBuffer buffer;
};
class cNetworkHandle
{
public:
    cNetworkHandle( std::string ipAddress, int port )
        : ipAddress( ipAddress )
        , port( port )
    {

    }
    cNetworkHandle& operator=( cNetworkHandle const& handle ) noexcept
    {
        const_cast<std::string&>( this->ipAddress ) = handle.ipAddress;
        const_cast<int&>( this->port ) = handle.port;
        return *this;
    }
    operator bool( ) const noexcept
    {
        return ( !this->ipAddress.empty( ) ) && ( this->port != 0 );
    }
    bool operator==( cNetworkHandle const& other ) const noexcept
    {
        return other.ipAddress == this->ipAddress && other.port == this->port;
    }
    bool operator!=( cNetworkHandle const& other ) const noexcept
    {
        return !( other.ipAddress == this->ipAddress && other.port == this->port );
    }
    bool operator<( cNetworkHandle const& other ) const noexcept
    {
        return other.ipAddress < this->ipAddress && other.port < this->port;
    }
    std::string const ipAddress;
    int const port;
};
class cPacketChunk
{
public:
    cPacketChunk( ) = default;
    cPacketChunk( cNetworkHandle networkHandle, cPacketBuffer const& packetBuffer )
        : networkHandle( networkHandle )
        , packetBuffer( packetBuffer )
    {

    }
    cPacketChunk( std::string ipAddress, int port, ubyte2 transferredBytes, cBuffer const& buffer )
        : networkHandle( ipAddress, port )
        , packetBuffer( transferredBytes, buffer )
    {

    }
    cNetworkHandle networkHandle;
    cPacketBuffer packetBuffer;
};
#pragma pack()
}
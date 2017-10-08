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
constexpr ubyte2 PACKET_MAX_BYTE = 65535U;
using cBuffer = std::array<char, PACKET_MAX_BYTE>;
using cPacketRaw = std::tuple<ubyte2, cBuffer>;
#pragma pack()
}
#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
#include <Network/Packet/StructureDefines.h>
#include <vector>
namespace Network
{
namespace Packet
{
namespace Event
{
class cEvePlayers : public cPacketBase<cEvePlayers, PacketId::EVE_PLAYERS>
{
public:
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
public:
    std::vector<PlayerFormat> mPlayerFormats;
};
}
}
}
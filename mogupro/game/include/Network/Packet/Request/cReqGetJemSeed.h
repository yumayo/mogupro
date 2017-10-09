#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Request
{
class cReqGetJemSeed : public cPacketBase<cReqGetJemSeed, PacketId::REQ_GET_JEM_SEED>
{
public:
    cReqGetJemSeed( );
    void packetImport( ubyte2 size, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
};
}
}
}
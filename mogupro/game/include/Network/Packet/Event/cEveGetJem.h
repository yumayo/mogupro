#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Event
{
class cEveGetJem : public cPacketBase<cEveGetJem, PacketId::EVE_GET_JEM>
{
public:
    cEveGetJem( );
    cEveGetJem( ubyte2 gemId );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
    // 採った相手の名前。
    // ※掘削機にも名前を付けておくのか、あとで話し合わないと！
    std::string mTargetName;
    // フィールドにあるどのジェムなのか。
    ubyte2 mGemId;
};
}
}
}
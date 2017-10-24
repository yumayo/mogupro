#pragma once
#include <Utility/cSingletonAble.h>
#include <Network/NetworkDefines.h>
namespace Network
{
class cUDPManager : public Utility::cSingletonAble<cUDPManager>
{
public:
    bool isConnectPacket( cPacketChunk const & packetChunk );
    void onReceive( cPacketChunk const & packetChunk );
};
}
#pragma once
#include <Utility/cSingletonAble.h>
#include <Network/NetworkDefines.h>
namespace Network
{
class cUDPManager : public Utility::cSingletonAble<cUDPManager>
{
public:
    void onReceive( cPacketChunk const & packetChunk );
};
}
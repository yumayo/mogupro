#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
	namespace Packet
	{
		namespace Response
		{
			class cResMakeRoom : public cPacketBase<cResMakeRoom, PacketId::RES_MAKE_ROOM>
			{
			public:
				cResMakeRoom();
				void packetImport(cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data) override;
				ubyte2 packetExport(char* const data) override;
				char mFlag;
				cNetworkHandle mNetworkHandle;
			};
		}
	}
}
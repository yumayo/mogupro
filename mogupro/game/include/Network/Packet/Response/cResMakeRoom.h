#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
#include <Network/PackFunction.hpp>
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
				cResMakeRoom(char flag);
				void packetImport(cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data) override;

				ubyte2 packetExport(char* const data) override;

				char mFlag;
				cNetworkHandle mNetworkHandle;
			};
		}
	}
}
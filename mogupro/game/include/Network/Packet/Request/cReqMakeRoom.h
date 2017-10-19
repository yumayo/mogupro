#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
#include <Network/PackFunction.hpp>
namespace Network
{
	namespace Packet
	{
		namespace Request
		{
			class cReqMakeRoom : public cPacketBase<cReqMakeRoom, PacketId::REQ_MAKE_ROOM>
			{
			public:
				cReqMakeRoom();
				cReqMakeRoom(int roomID);
				void packetImport(cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data) override;

				ubyte2 packetExport(char* const data) override;

				int mRoomID;
				cNetworkHandle mNetworkHandle;
			};
		}
	}
}
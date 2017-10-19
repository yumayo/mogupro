#pragma once

#include <Network/Packet/cPacketBase.h>

#include <Network/Packet/PacketId.h>

namespace Network
{
	namespace Packet
	{
		namespace Response
		{
			class cResCheckBreakBlocks : public cPacketBase<cResCheckBreakBlocks, PacketId::RES_CHECK_BREAK_BLOCKS>
			{
			public:

				cResCheckBreakBlocks();

				void packetImport(cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data) override;

				ubyte2 packetExport(char* const data) override;
			};
		}
	}
}
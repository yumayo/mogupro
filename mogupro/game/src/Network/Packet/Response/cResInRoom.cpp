#include <Network/Packet/Response/cResInRoom.h>
#include <Network/PackFunction.hpp>
namespace Network
{
	namespace Packet
	{
		namespace Response
		{
			cResInRoom::cResInRoom()
			{
			}

			cResInRoom::cResInRoom(char flag) : mFlag(flag)
			{ 
			}

			void cResInRoom::packetImport(cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data)
			{
				int offset = 0;
				importChar(mFlag, data, offset, 1);
			}

			ubyte2 cResInRoom::packetExport(char* const data)
			{
				int offset = 0;
				exportChar(mFlag, data, offset, 1);
				return offset;
			}
		}
	}
}
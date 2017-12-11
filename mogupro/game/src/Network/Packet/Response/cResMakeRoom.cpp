#include <Network/Packet/Response/cResMakeRoom.h>
#include <Network/PackFunction.hpp>
namespace Network
{
	namespace Packet
	{
		namespace Response
		{
			cResMakeRoom::cResMakeRoom() : mNetworkHandle("", 0)
			{

			}

			cResMakeRoom::cResMakeRoom(char flag) :mFlag(flag), mNetworkHandle("", 0)
			{

			}

			void cResMakeRoom::packetImport(cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data)
			{
				mNetworkHandle = networkHandle;
				int offset = 0;
				importChar(mFlag, data, offset, 1);
			}

			ubyte2 cResMakeRoom::packetExport(char* const data)
			{
				int offset = 0;
				exportChar(mFlag, data, offset, 1);
				return offset;
			}

		}

	}

}
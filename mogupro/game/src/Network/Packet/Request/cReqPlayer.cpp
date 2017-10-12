#include <Network/Packet/Request/cReqPlayer.h>
#include <Network/PackFunction.hpp>
namespace Network
{
namespace Packet
{
namespace Request
{
cReqPlayer::cReqPlayer( )
{
	xPos = yPos = zPos = -10.0f;
	thetaX = 1.5f;
	thetaY = 2.4f;
}
void cReqPlayer::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
	bool end = true;
	int offset = 0;
	end = importFloat(xPos,data,offset,20);
	end = importFloat(yPos, data, offset, 20);
	end = importFloat(zPos, data, offset, 20);
	end = importFloat(thetaX, data, offset, 20);
	end = importFloat(thetaY, data, offset, 20);

	//if (end != true)
}
ubyte2 cReqPlayer::packetExport( char* const data )
{
	bool end = true;
	int offset = 0;
	end = exportFloat(xPos,data,offset,20);
	end = exportFloat(yPos, data, offset, 20);
	end = exportFloat(zPos, data, offset, 20);
	end = exportFloat(thetaX, data, offset, 20);
	end = exportFloat(thetaY, data, offset, 20);

	//if(end != true)
		

    return offset;
}
}
}
}
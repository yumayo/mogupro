#include <Network/Packet/Request/cReqPlayer.h>
#include <Network/PackFunction.hpp>
namespace Network
{
namespace Packet
{
namespace Request
{
cReqPlayer::cReqPlayer( ) 
: xPos(0.0f)
, yPos(0.0f)
, zPos(0.0f)
, xTheta(0.0f)
, yTheta(0.0f)
{

}

cReqPlayer::cReqPlayer(float x, float y, float z, float xTheta, float yTheta)
: xPos(x)
, yPos(y)
, zPos(z)
, xTheta(xTheta)
, yTheta(yTheta)
{
}
void cReqPlayer::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
	bool end = true;
	int offset = 0;
	end = importFloat(xPos,data,offset,20);
	end = importFloat(yPos, data, offset, 20);
	end = importFloat(zPos, data, offset, 20);
	end = importFloat(xTheta, data, offset, 20);
	end = importFloat(yTheta, data, offset, 20);

	//if (end != true)
}
ubyte2 cReqPlayer::packetExport( char* const data )
{
	bool end = true;
	int offset = 0;
	end = exportFloat(xPos,data,offset,20);
	end = exportFloat(yPos, data, offset, 20);
	end = exportFloat(zPos, data, offset, 20);
	end = exportFloat(xTheta, data, offset, 20);
	end = exportFloat(yTheta, data, offset, 20);

	//if(end != true)
		

    return offset;
}
}
}
}
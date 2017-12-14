#include <Network/Packet/Response/cResCheckBeginGame.h>
#include <Network/PackFunction.hpp>

namespace Network
{
namespace Packet
{
namespace Response
{
cResCheckBeginGame::cResCheckBeginGame( )
{


}
cResCheckBeginGame::cResCheckBeginGame(ubyte1 playerID) : mPlayerID(playerID)
{


}

void cResCheckBeginGame::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
	cImporter(data) >> mPlayerID;
}

ubyte2 cResCheckBeginGame::packetExport( char* const data )
{
	cExporter exp( data );
	exp << mPlayerID;
	return exp.getSize( );
}
}
}
}
#include <Network/Packet/Event/cEveTeamMember.h>
#include <Network/PackFunction.hpp>
namespace Network
{
namespace Packet
{
namespace Event
{
cEveTeamMember::cEveTeamMember( )
{

}

cEveTeamMember::cEveTeamMember(ubyte1 teamNum, std::string nameStr,ubyte1 playerID) :mTeamNum(teamNum),mNameStr(nameStr), mPlayerID(playerID)
{

}
void cEveTeamMember::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
    cImporter imp( data );
    imp >> mTeamNum >> mNameStr >> mPlayerID;
}

ubyte2 cEveTeamMember::packetExport( char* const data )
{
    cExporter exp( data );
     exp << mTeamNum << mNameStr << mPlayerID;
    return exp.getSize( );
}
}
}
}
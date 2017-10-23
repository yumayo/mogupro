#pragma once
#include <Utility/cSingletonAble.h>
#include <Network/NetworkDefines.h>
#include <map>
#include <set>
namespace Game
{
namespace Trans
{
class cServerAdapter
{
public:
    cServerAdapter( );
    ~cServerAdapter( );
private:
    void sendPlayersPosition( );
    void sendSetQuarry( );
    void sendGetGemPlayer( );
    void sendGetGemQuarry( );
    void sendBreakBlocks( );
private:
    std::map<Network::cNetworkHandle, Network::ubyte1> mHandlePlayers;
    std::set<Network::ubyte2> mQuarrys;
    std::set<Network::ubyte2> mGems;
};
}
}

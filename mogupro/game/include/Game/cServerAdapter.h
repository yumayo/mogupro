#pragma once
#include <Utility/cSingletonAble.h>
#include <Network/NetworkDefines.h>
#include <map>
#include <set>
#include <Network/Packet/StructureDefines.h>
namespace Game
{
class cServerAdapter : public Utility::cSingletonAble<cServerAdapter>
{
public:
    cServerAdapter( );
    ~cServerAdapter( );
public:
    void update( );
private:
    void sendPlayers( );
    void sendSetQuarry( );
    void sendGetGemPlayer( );
    void sendGetGemQuarry( );
	void sendBreakBlocks( );
	void sendLightBombs( );
private:
    std::map<Network::ubyte1, Network::Packet::PlayerFormat> mPlayerFormats;
    std::set<Network::ubyte2> mQuarrys;
    Network::ubyte2 mQuarryId;
    std::set<Network::ubyte2> mGems;
};
}

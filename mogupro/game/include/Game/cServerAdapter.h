#pragma once
#include <Utility/cSingletonAble.h>
#include <Network/NetworkDefines.h>
#include <map>
#include <set>
#include <cinder/Vector.h>
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
    void sendPlayersPosition( );
    void sendSetQuarry( );
    void sendGetGemPlayer( );
    void sendGetGemQuarry( );
    void sendBreakBlocks( );
private:
    std::map<Network::ubyte1, cinder::vec3> mPlayersPosition;
    std::set<Network::ubyte2> mQuarrys;
    Network::ubyte2 mQuarryId;
    std::set<Network::ubyte2> mGems;
};
}

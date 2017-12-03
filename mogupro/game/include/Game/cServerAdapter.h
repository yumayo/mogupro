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
	class Player
	{
	public:
		void respawn( );
		bool kill( );
		Network::ubyte1 id;
		Network::ubyte1 isLive;
		cinder::vec3 position;
		cinder::quat rotation;
	};
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
	void sendResult( );
	void sendCannonPower( );
	void sendAddCannonPower( );
private:
    std::map<Network::ubyte1, Player> mPlayers;
    std::set<Network::ubyte2> mQuarrys;
    Network::ubyte2 mQuarryId;
	std::set<Network::ubyte2> mGems;
	Network::ubyte1 mRedTeamPower;
	Network::ubyte1 mBlueTeamPower;
};
}

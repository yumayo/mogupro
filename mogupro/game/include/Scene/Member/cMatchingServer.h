#pragma once
#include <Scene/cSceneBase.h>
#include <Network.hpp>
namespace Scene
{
namespace Member
{
class cMatchingServer : public cSceneBase
{
public:
	void setup();
	void shutDown();
	void update(float deltaTime);
	void checkReqMakeRoom();
	void checkReqInRoom();
	void draw();
	void draw2D();
	void resize();
private:
	enum class PhaseState
	{
		NOT_IN_ROOM,
		IN_ROOM,
	};

	bool addRoomMembers(Network::cNetworkHandle addMember);

	PhaseState mPhaseState;
	Network::cNetworkHandle mMasterData;

	std::vector<Network::cNetworkHandle> mRoomMembers;
	int mRoomID;
	bool mOpenRoom;
};
}
}
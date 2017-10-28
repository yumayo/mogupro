#pragma once
#include <Scene/cSceneBase.h>
#include <Network.hpp>
#include <Node/node.h>
#include <Node/renderer.hpp>
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
	void checkTeamIn();
	void checkBeginGame();
	void draw();
	void draw2D();
	void resize();
private:
	enum class PhaseState
	{
		NOT_IN_ROOM,
		IN_ROOM,
		//Ç∆ÇËÇ†Ç¶Ç∏ÉQÅ[ÉÄäJénÇ∑ÇÈ
		BEGIN_GAME
	};
	PhaseState mPhaseState;
	int mRoomID;
	bool mOpenRoom;
    bool mIsGameUpdate;
	Utility::hardptr<Node::node> n;
	Utility::hardptr<Node::Renderer::label> font;
};
}
}
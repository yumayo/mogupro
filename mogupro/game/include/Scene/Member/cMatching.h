#pragma once
#include <Scene/cSceneBase.h>
#include <Node/node.h>

namespace Scene
{
namespace Member
{
class cMatching : public cSceneBase
{
public:
	void setup();
	void shutDown();
	void update(float deltaTime);
	void makeRoom();
	void inRoom();
	void addInRoomUI();
	void draw();
	void draw2D();
	void resize();
private:
	enum class ClassState
	{
		MASTER,
		CLIENT,
		NOT
	};

	ClassState mClassState;
	ClassState mWaitClassState;
	enum class PhaseState
	{
		NOT_IN_ROOM,
		IN_ROOM,
		BEGIN_GAME
	};

	PhaseState mPhaseState;

	bool mCanSend;
	hardptr<Node::node> mRoot;
	hardptr<Node::node> mMemberRoot;
	bool mAddMember;
	int mTeamNum;
	ci::vec3 mCamPos;
	int mSelectTag;
	std::vector<std::function<void()>> outRoomFunc;
	std::vector<std::function<void()>> inRoomFunc;
};
}
}
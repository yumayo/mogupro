#pragma once
#include <Scene/cSceneBase.h>
#include <Node/node.h>
#include <vector>
#include <Node/action.hpp>
#include <Node/renderer.hpp>
#include <Utility/TriMeshAnimation.h>
#include <cinder/gl/Fbo.h>

class DrillUI
{
public:

	DrillUI();
	DrillUI(ci::vec2 pos, ci::vec2 moveVec, std::string name);

	void update(float deltaTime);
	void draw();
	hardptr<Node::node> mRoot;
	int animationTime;
	float time;
	enum AnimationType
	{
		NONE,
		BEGIN,
		END
	};
	AnimationType type;
	ci::vec2 firePosition;
};

class Star
{
public:
	Star();
	void update(float deltaTime);
	void draw();
	hardptr<Node::node> mRoot;
	float time;
	float value;
};

namespace Scene
{
namespace Member
{
class cMatching : public cSceneBase
{
public:
	void setup();
	void registerFunc();
	void setAnimation();
	void shutDown();
	void update(float deltaTime);
	void updateBoxFunc();
	void makeRoom();
	void inRoom();
	void addInRoomUI();
	void draw();
	void draw2D();
	void drawInRoom2D();
	void resize();
	inline std::string const getName( ) { return typeid( *this ).name( ); }
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
	hardptr<Node::node> mBackRoot;
	hardptr<Node::node> mRoot;
	hardptr<Node::node> mMemberRoot;
	bool mAddMember;
	int mTeamNum;
	ci::vec3 mCamPos;
	int mSelectTag;
	int mPrevSelectTag;
	std::vector<std::function<void()>> outRoomFunc;
	std::vector<std::function<void()>> inRoomFunc;
	bool canSendInRoom;
	bool canStartUpdateServer;

	bool mBeginAnimation;
	bool mIsEndAnimation;

	int teamCount[2];
	std::vector<DrillUI> drillUI1Ps;
	std::vector<DrillUI> drillUI2Ps;
	Utility::TriMeshAnimation mTrimeshAnimation;
	ci::gl::FboRef mTrimeshAnimationFbo;
	bool sceneChange;

	std::vector<Star> stars;
	bool wantWatching;

	enum class SceneType
	{
		TITLE,
		GAME_MAIN
	};
	SceneType sceneType;

};

}
}
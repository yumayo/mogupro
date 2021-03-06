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
	void checkReqInRoomWatching( ); // 2017/12/14 yumayo 観戦用
	void checkTeamIn();
	void checkBeginGame();
	void resetMember();
	void updateServer(float deltaTime);
	void checkStartGameMember();
	void draw();
	void draw2D();
	void resize();
	inline std::string const getName( ) { return typeid( *this ).name( ); }
private:
	enum class PhaseState
	{
		NOT_IN_ROOM,
		IN_ROOM,
		//とりあえずゲーム開始する
		BEGIN_GAME
	};

	PhaseState mPhaseState;
	int mRoomID;
	bool mOpenRoom;
    bool mIsGameUpdate;
	Utility::hardptr<Node::node> n;
	Utility::hardptr<Node::Renderer::label> font;
	bool mStartGame;
	int teamCount;
};
}
}
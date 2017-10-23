#pragma once
#include <Scene/cSceneBase.h>
#include "cinder/Font.h"
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
	//ci::Font mFont;
};
}
}

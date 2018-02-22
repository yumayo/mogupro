#pragma once
#include <Scene/cSceneBase.h>
#include <cinder/gl/gl.h>
#include <ModeSelect/cDrawFunc.h>
#include <ModeSelect/cSelectCard.h>
#include <Sound/cIntroLoopableBGM.h>
#include<Node\node.h>
#include<Node\action.hpp>
namespace Scene
{
namespace Member
{
class cModeSelect final : public cSceneBase
{
public:
    cModeSelect( );
    ~cModeSelect( );
public:
	void setup() override;
	void shutDown() override;
	void update(float t) override;
	void draw() override;
	void draw2D() override;
	void resize() override;
	inline std::string const getName() { return typeid(*this).name(); }
private:
	ModeSelect::cDrawFunc mDrawFunc;
	std::vector<std::shared_ptr<ModeSelect::cSelectCard>>mSelectCards;
	float mBackGroundRotateZ = M_PI;
	void createTextureNames();
	void changeScene();
	void selectIcon();
	void shiftOnlineBattle();
	void shiftTitle();
	void shiftLocalBattle();
	void shiftTutorial();
	void desideScene();
	void updateBackGround(float t);
	int MSelectScene = 0;
	void drawButton();
	float buttonalfa = 0.0f;
	float buttonscale = 0.0f;
	void updateButtonAlfa(float t);
	float buttonAlfaT = 0.0f;
	float arrowT = 0.0f;
	void updateArrow(float t);
	float arrowAlfa = 0.0f;
	float aroowtranceY = 0.0f;
	float EaseReturn(const float t,const float s,const float e);
	std::vector<std::string> iconnames;
	Utility::hardptr<Node::node>root;
	bool isfading = true;
	Sound::cIntroLoopableBGM introloopBGM;
};
}
}
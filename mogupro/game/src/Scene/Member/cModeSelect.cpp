#include <Scene/Member/cModeSelect.h>
#include <CameraManager/cCameraManager.h>
#include "Resource\TextureManager.h"
#include"cinder\gl\gl.h"
#include <Utility/cInput.h>
#include "cinder\Easing.h"
#include "Particle\cParticleManager.h"
#include <Sound/Wav.h>
#include"Resource\cSoundManager.h"
#include <Scene/Member/Tutorial.h>
#include <Network.hpp>
#include <Scene/Member/cMatching.h>
#include <Scene/Member/cGameMain.h>
#include <Scene/cSceneManager.h>
#include <Game/cGameManager.h>
#include <Scene/Member/cTitle.h>
#include"Resource\cImageManager.h"
using namespace ci;
using namespace ci::app;

using namespace Node;

using namespace Node::Action;
namespace Scene
{
namespace Member
{
cModeSelect::cModeSelect( )
{

	
}
cModeSelect::~cModeSelect( )
{
	
	
}
void cModeSelect::setup()
{
	auto bgm = Sound::Wav(cinder::app::getAssetDirectories().front().string() + "/BGM/modeselect.wav");
	introloopBGM.create(bgm.data(), bgm.size(), 22.130F, 78.594F);
	introloopBGM.gain(0.15F);
	introloopBGM.play();
	CAMERA->refPosition = ci::vec3(0,0,-0.001);
	ENV->setMouseControl(false);
	CAMERA->setCameraAngle(ci::vec2(M_PI/18.f, M_PI / 18.f));
	CAMERA->setup();
	createTextureNames();

	for (float i = 0; i < 4; i++) {
		mSelectCards.push_back(std::make_shared<ModeSelect::cSelectCard>(i*2.f*M_PI / 4.f, iconnames[i], ci::vec3(0, 0.5, 3), ci::vec3(2.5, 0.5, 4.75),i));
	}
	root = Node::node::create();
	root->set_content_size(app::getWindowSize());
	root->set_schedule_update();
	root->set_scale(vec2(1, -1));
	root->set_position(root->get_content_size() * vec2(-0.5F, 0.5F));

	auto fader = root->add_child(Node::Renderer::rect::create(app::getWindowSize()));
	fader->set_color(ColorA(0, 0, 0, 1));
	fader->set_anchor_point(vec2(0, 0));
	fader->run_action(sequence::create( fade_out::create(1.0F), call_func::create([this]
	{
		this->isfading = false;
		//ci::app::console() << "‚Ó‚¨‚¨‚¨‚¨‚¨‚¨‚¨‚¨‚¨‚¨‚¨‚¨‚¨‚¨‚¨‚¨‚¨‚¨‚¨‚¨‚¨‚¨‚¨‚¨‚¨‚¨‚¨‚¨‚¨‚¨‚¨‚¨‚¨‚¨‚¨‚¨‚¨‚¨	" << std::endl;
	})));
	//this->isfading = false;
}
void cModeSelect::shutDown()
{
	mSelectCards.clear();
	iconnames.clear();
	introloopBGM.stop();
}
void cModeSelect::update(float t)
{
	for (auto& it : mSelectCards) {
		it->update(t);
	}
	updateBackGround(t);
	updateButtonAlfa(t);
	updateArrow(t);
	introloopBGM.update(t);
	selectIcon();
	if (ENV->pushKey(ci::app::KeyEvent::KEY_RETURN)|| ENV->isPadPush(ENV->BUTTON_2)) {
		desideScene();
	}
	root->entry_update(t);
}
void cModeSelect::draw()
{
	mDrawFunc.drawTextureCube(ci::vec3(0), ci::vec3(16, 9, 20), ci::vec3(M_PI/12.f, 0, mBackGroundRotateZ), "modeselect/selectsky.png", ci::ColorA(1, 1, 1));

	for (auto& it : mSelectCards) {
		it->draw();
	}
	drawButton();

	mDrawFunc.drawTextureRect3D(ci::vec3(-0.85-aroowtranceY/2.f, -0.7+aroowtranceY,-0.3), ci::vec3(0.8, 0.16,0),ci::vec3(0,0,M_PI- M_PI / 12.f) , "modeselect/redright.png", ci::ColorA(1, 1, 1, arrowAlfa));
	mDrawFunc.drawTextureRect3D(ci::vec3(1.55+aroowtranceY/2.f, -0.7 + aroowtranceY, -0.3), ci::vec3(-0.8, 0.12, 0), ci::vec3(0, 0, M_PI + M_PI / 4.25f),  "modeselect/redright.png", ci::ColorA(1, 1, 1, arrowAlfa));
	//mDrawFunc.drawTextureRect3D(ci::vec3(-400, -175 + aroowtranceY), ci::vec3(250, 50), M_PI - M_PI / 9.f, "redright.png", ci::ColorA(1, 1, 1, arrowAlfa));
}
void cModeSelect::draw2D()
{
	/*mDrawFunc.drawTextureRect2D(ci::vec2(400,-175+aroowtranceY), ci::vec2(-250, 50), M_PI+M_PI/9.f, "redright.png", ci::ColorA(1, 1, 1,arrowAlfa));
	mDrawFunc.drawTextureRect2D(ci::vec2(-400, -175+aroowtranceY), ci::vec2(250, 50), M_PI - M_PI / 9.f, "redright.png", ci::ColorA(1, 1,1, arrowAlfa));*/
	mDrawFunc.drawTextureRect2D(ci::vec2(550,-320), ci::vec2(-590,449)*0.52f, M_PI, "title/logo.png", ci::ColorA(1, 1, 1, 1));
	root->entry_render(mat4());
}
void cModeSelect::resize()
{

}

void cModeSelect::createTextureNames()
{
	iconnames.push_back("modeselect/online.png");
	iconnames.push_back("modeselect/tutorial.png");
	iconnames.push_back("modeselect/gotitle.png");
	iconnames.push_back("modeselect/training.png");
	
}

void cModeSelect::changeScene()
{
	
	switch (MSelectScene)
	{
	case 0:
		shiftOnlineBattle();
		return;
	case 1:
		shiftTutorial();
		return;
	case 2:
		shiftTitle();
		return;
	case 3:
		shiftLocalBattle();
		return;
	default:
		break;
	}
}

void cModeSelect::selectIcon()
{
	if (isfading)return;

	if (ENV->pushKey(ci::app::KeyEvent::KEY_LEFT)||ENV->getPadAxisPushMinus(0)) {
		Resource::cSoundManager::getInstance()->findSe("ModeSelect/cursor.wav").setGain(0.4f);
		Resource::cSoundManager::getInstance()->findSe("ModeSelect/cursor.wav").play();
		for (auto& it : mSelectCards) {
			it->setEasing(true);
		}
		MSelectScene++;
		if (MSelectScene == 4)MSelectScene = 0;
		return;
	}
	if (ENV->pushKey(ci::app::KeyEvent::KEY_RIGHT) || ENV->getPadAxisPushPlus(0)) {
		Resource::cSoundManager::getInstance()->findSe("ModeSelect/cursor.wav").setGain(0.4f);
		Resource::cSoundManager::getInstance()->findSe("ModeSelect/cursor.wav").play();
		for (auto& it : mSelectCards) {
			it->setEasing(false);
		}
		MSelectScene--;
		if (MSelectScene < 0)MSelectScene = 3;
		return;
	}
}

void cModeSelect::shiftOnlineBattle()
{
	//introloopBGM.stop();
	cSceneManager::getInstance()->shift<Scene::Member::cMatching>();
}

void cModeSelect::shiftTitle()
{
	//introloopBGM.stop();
	cSceneManager::getInstance()->shift<Scene::Member::cTitle>();
}

void cModeSelect::shiftLocalBattle()
{
	//introloopBGM.stop();
	Network::cUDPClientManager::getInstance()->open();
	Network::cUDPServerManager::getInstance()->open();
	Network::cUDPClientManager::getInstance()->connectOfflineServer();
	Game::cGameManager::getInstance()->setTime(0.0F);
	cSceneManager::getInstance()->shift<Scene::Member::cGameMain>();
}

void cModeSelect::shiftTutorial()
{
	//introloopBGM.stop();
	Network::cUDPClientManager::getInstance()->open();
	Network::cUDPServerManager::getInstance()->open();
	Network::cUDPClientManager::getInstance()->connectOfflineServer();
	Game::cGameManager::getInstance()->setTime(0.0F);
	cSceneManager::getInstance()->shift<Scene::Member::cTutorial>();
}

void cModeSelect::desideScene()
{
	if (isfading)return;
	for (auto& it : mSelectCards) {
		if (it->getIsEasing())return;//ƒC[ƒWƒ“ƒO’†‚Í–³Œø
	}
	Resource::cSoundManager::getInstance()->findSe("ModeSelect/return.wav").setGain(0.8f);
	Resource::cSoundManager::getInstance()->findSe("ModeSelect/return.wav").play();
	introloopBGM.fadeout(0.9f,0.0f);
	isfading = true;
	auto fader = root->add_child(Node::Renderer::rect::create(app::getWindowSize()));
	fader->set_color(ColorA(0, 0, 0, 0));
	fader->set_anchor_point(vec2(0, 0));
	fader->run_action(sequence::create( fade_in::create(1.0F), call_func::create([this]
	{
		this->changeScene();
	})));
}

void cModeSelect::updateBackGround(float t)
{
	mBackGroundRotateZ += 0.02f*t;
}

void cModeSelect::drawButton()
{
	ci::vec3 pos = ci::vec3(-0.5, -0.4, -1.78);
	mDrawFunc.drawTextureRect3D(pos, ci::vec3(0.2, 0.2, 0), ci::vec3(0, M_PI, 0), "modeselect/buttonb.png", ci::ColorA(1, 1, 1, 1));
	mDrawFunc.drawTextureRect3D(pos+ci::vec3(0,0,-0.01), ci::vec3(0.2, 0.2, 0)*buttonscale, ci::vec3(0, M_PI, 0), "modeselect/ring.png", ci::ColorA(1, 1, 0, buttonalfa));
	
}

void cModeSelect::updateButtonAlfa(float t)
{
	float speed = 2.18f;
	buttonAlfaT += speed*t;
	bool isend = false;
	if (buttonAlfaT >= 1.0f) {
		buttonAlfaT = 1.0f;
		isend = true;
	}
	buttonalfa = EaseCubicIn(buttonAlfaT, 0.4f, 0.8f);
	buttonscale = EaseCubicIn(buttonAlfaT, 0.8f, 1.2f);
	if (isend) {
		buttonAlfaT = 0.0f;
	}

}

void cModeSelect::updateArrow(float t)
{
	float speed = 1.09f;
	arrowT += speed*t;
	bool isend = false;
	if (arrowT >= 1.0f) {
		arrowT = 1.0f;
		isend = true;
	}
	aroowtranceY = EaseReturn(arrowT, 0, 0.05f);//‚P‚T
	arrowAlfa = EaseReturn(arrowT, 0.5f, 0.5f);
	if (isend) {
		arrowT = 0.0f;
	}
}

float cModeSelect::EaseReturn(const float t, const float s, const float e)
{
	return s + -4.f*e*(t - 0.5f)*(t - 0.5f) + e;
}

}
}

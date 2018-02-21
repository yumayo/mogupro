#pragma once
#include"Utility\cSingletonAble.h"
#include"Game\cObjectBase.h"
#include"Game/Strategy/cStrategyObjectBase.h"
#include"cinder\AxisAlignedBox.h"
#include"Strategy\cCannon.h"
#include<map>
#include "cinder\Json.h"

namespace Game
{
class cStrategyManager : public Utility::cSingletonAble<cStrategyManager>
{
public:
    cStrategyManager();
    ~cStrategyManager();
    void setup();
	void lightSetup();
    void draw();
    void update(const float& deltatime);
	void updateCollisionAfterUpdate(const float& deltaTime);
    bool isAABB(const ci::AxisAlignedBox & a, const ci::AxisAlignedBox & b );
	std::vector<std::shared_ptr<Game::Strategy::cCannon>> getCannons();
	//�e�X�g�p�ł�
	void drawCube(const ci::vec3 pos, const ci::vec3 size, const ci::vec3 rotate, const ci::ColorA color);
	void drawShere(const ci::vec3 pos, const ci::vec3 size, const ci::vec3 rotate, const ci::ColorA color,int segment);
private:
	std::vector<std::shared_ptr<Game::Strategy::cCannon>>cannons;
	ci::vec3 jsonToVec3(Json::Value json);
    void deleteObject();
	int testcount = 0;

	// ///// ���C�g��ID���܂Ƃ߂��f�[�^��p�ӂ���
	std::vector<int> pointLightIds;
	std::vector<int> lineLightIds;
	std::vector<int> spotLightIds;
};

}
#define STRM Game::cStrategyManager::getInstance()
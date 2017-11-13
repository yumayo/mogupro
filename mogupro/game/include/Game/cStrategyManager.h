#pragma once
#include"Utility\cSingletonAble.h"
#include"Game\cObjectBase.h"
#include"Game/Strategy/cStrategyObjectBase.h"
#include"cinder\AxisAlignedBox.h"
#include"Strategy/cDrill.h"
#include"Strategy\cCannon.h"
#include"Strategy\cBomb.h"
#include<map>

namespace Game
{
class cStrategyManager : public Utility::cSingletonAble<cStrategyManager>
{
public:
    cStrategyManager();
    ~cStrategyManager();
    void setup();
    void draw();
    void update(const float& deltatime);
	void updateCollisionAfterUpdate(const float& deltaTime);
    bool isAABB( const ci::AxisAlignedBox & a, const ci::AxisAlignedBox & b );
	void HitDrillToGem(const int _objectid,const int _gemid);
	void CreateDrill(const ci::vec3 _pos, const int _id,const Strategy::cDrill::DrillType _type,const bool _ismyobject);
	void CreateCannon(const ci::vec3 _pos, const float _direction, const int _id,const bool _ismyobject);
	void CreateBomb(const ci::vec3 _pos, const ci::vec3 _speed, const ci::vec3 _scale, const int _id);
	//テスト用です
	void drawCube(const ci::vec3 pos, const ci::vec3 size, const ci::vec3 rotate, const ci::ColorA color);
private:
	std::map<int, std::shared_ptr<Game::Strategy::cDrill>>drills;
	std::map<int, std::shared_ptr<Game::Strategy::cBomb>>bombs;
	std::map<int, std::shared_ptr<Game::Strategy::cCannon>>cannons;
    void deleteObject();
	int testcount = 0;
};

}
#define STRM Game::cStrategyManager::getInstance()
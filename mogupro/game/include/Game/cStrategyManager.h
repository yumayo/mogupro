#pragma once
#include"Utility\cSingletonAble.h"
#include"Game\cObjectBase.h"
#include"Game/Strategy/cStrategyObjectBase.h"
#include"cinder\AxisAlignedBox.h"
#include"Strategy/cDrill.h"
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
    void update(const float& deltatime = 0);
    bool isAABB( const ci::AxisAlignedBox & a, const ci::AxisAlignedBox & b );
	void HitDrillToGem(const int _objectid,const int _gemid);
	void CreateDrill(const ci::vec3 _pos, const int _id,const Strategy::cDrill::DrillType _type,const bool _ismyobject);
private:
	std::map<int, std::shared_ptr<Game::Strategy::cDrill>>drills;
    void deleteObject();
	int testcount = 0;
};

}
#define STRM Game::cStrategyManager::getInstance()
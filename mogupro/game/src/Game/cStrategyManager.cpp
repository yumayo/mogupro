#include<Game/cStrategyManager.h>
#include <Utility/cInput.h>
#include"Game/Strategy/cDrill.h"
#include"Game/Strategy/cStrategyObjectBase.h"
#include"cinder\Rand.h"
using namespace ci;
using namespace ci::app;

namespace Game
{
cStrategyManager::cStrategyManager( )
{

};
cStrategyManager::~cStrategyManager( )
{

}
void cStrategyManager::setup()
{

}
void cStrategyManager::draw()
{
	for (auto it : strategyobjects) {
		it->draw();
	}
}
void cStrategyManager::update()
{
	for (auto& it : strategyobjects) {
		it->update(0.f);//////////////////////////仮
	}
	deleteObject();
	if (ENV->pushKey(KeyEvent::KEY_p)) {
		strategyobjects.push_back(std::make_shared<Game::Strategy::cDrill>(vec3(randInt(0,10), 10, randInt(0, 10)), vec3(1, 1, 1)));
	}
}

void cStrategyManager::deleteObject()
{
	for (auto itr = strategyobjects.begin();
		itr != strategyobjects.end();) {
		if ((*itr)->DeleteThis()) {
			itr = strategyobjects.erase(itr);
		}
		else {
			itr++;
		}
	}
}

bool cStrategyManager::isAABB(const ci::AxisAlignedBox & a, const ci::AxisAlignedBox & b)
{
		const ci::vec3& a_min = a.getMin();
		const ci::vec3& a_max = a.getMax();
		const ci::vec3& b_min = b.getMin();
		const ci::vec3& b_max = b.getMax();

		// XYZの各軸ごとに領域が重なっているかを調べる
		if (a_max.x < b_min.x || a_min.x > b_max.x) return false;
		if (a_max.y < b_min.y || a_min.y > b_max.y) return false;
		if (a_max.z < b_min.z || a_min.z > b_max.z) return false;

		// 全てが重なっている→２つのAABBは交差
		return true;
}

;
}

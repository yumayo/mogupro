#include<Game/cStrategyManager.h>
#include <Utility/cInput.h>
#include"Game/Strategy/cDrill.h"
#include"Game/Strategy/cStrategyObjectBase.h"
#include"cinder\Rand.h"
#include<algorithm>
#include"Game\cClientAdapter.h"
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
	for (auto it : drills) {
		it.second->draw();
	}
}
void cStrategyManager::update(const float & deltatime)
{
	//using namespace Network;
	//using namespace Network::Packet::Response;
	//auto& p = cResponseManager::getInstance()->mResCheckGetJem;
	//while (!p.empty())
	//{
	//	auto t = p.top();
	//	p.pop();

	//	if (t.mFlag)
	//	{
	//		HitDrillToGem(t.mDrillID, t.mGemID);
	//	}
	//}

	for (auto& it : drills) {
		it.second->update(deltatime);
	}


	deleteObject();
	if (ENV->pushKey(KeyEvent::KEY_p)) {

		//cClientAdapter::getInstance()->sendSetQuarry(vec3(randInt(0, 10), 15, randInt(0, 10)),Strategy::cDrill::DrillType::Level1);

		//drills.insert(std::make_pair(testcount, std::make_shared<Game::Strategy::cDrill>(vec3(randInt(0, 10), 15, randInt(0, 10)), testcount, Strategy::cDrill::DrillType::Level1, true)));

		testcount++;
	}



}

void cStrategyManager::deleteObject()
{
	for (auto itr = drills.begin();
		itr != drills.end();) {
		if (itr->second->DeleteThis()) {
			itr = drills.erase(itr);
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

void cStrategyManager::HitDrillToGem(const int _objectid, const int _gemid)
{
	auto drill = drills[_objectid];

	drill->HitGem(_gemid);


}

void cStrategyManager::CreateDrill(const ci::vec3 _pos, const int _id, const Strategy::cDrill::DrillType _type, const bool _ismyobject)
{
	drills.insert(std::make_pair(_id, std::make_shared<Game::Strategy::cDrill>(_pos, _id, _type, _ismyobject)));
}
}


;


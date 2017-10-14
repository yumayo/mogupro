#include<Strategy/cStrategyManager.h>
#include <Utility/Input.h>
#include"Strategy\StrategyObject\cDrill.h"
#include"Strategy\StrategyObject\cStrategyObjectBase.h"
#include"cinder\Rand.h"
using namespace ci;
using namespace ci::app;

namespace Strategy
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
		it->update(0.f);//////////////////////////‰¼
	}
	deleteObject();
	if (ENV->pushKey(KeyEvent::KEY_p)) {
		strategyobjects.push_back(std::make_shared<Strategy::StrategyObject::cDrill>(vec3(randInt(0,10), 10, randInt(0, 10)), vec3(1, 1, 1)));
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

;
}

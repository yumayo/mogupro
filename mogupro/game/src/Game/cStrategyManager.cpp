#include<Game/cStrategyManager.h>
#include <Utility/cInput.h>
#include"Game/Strategy/cDrill.h"
#include"Game/Strategy/cStrategyObjectBase.h"
#include"cinder\Rand.h"
#include<algorithm>
#include"Game\cClientAdapter.h"
#include"Game\cFieldManager.h"
#include"Game/Strategy/cBomb.h"
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
	///////キャノン作ります
	float x_max = Field::CHUNK_RANGE_X * Field::CHUNK_SIZE*Field::BLOCK_SIZE;
	float y_max = Field::CHUNK_RANGE_Y * Field::CHUNK_SIZE*Field::BLOCK_SIZE;
	float z_max = Field::CHUNK_RANGE_Z * Field::CHUNK_SIZE*Field::BLOCK_SIZE;

	CreateCannon(vec3(x_max / 2.f, y_max + Field::CHUNK_RANGE_X/2.f - 0.5f, Field::CHUNK_RANGE_X / 2.f -0.5),1.f,0, true);
	CreateCannon(vec3(x_max / 2.f, y_max + Field::CHUNK_RANGE_X / 2.f -0.5f, z_max- Field::CHUNK_RANGE_X / 2.f -0.5),-1.f,1, false);
	///////キャノン作ります
}
void cStrategyManager::draw()
{
	for (auto it : drills) {
		it.second->draw();
	}
	for (auto it :cannons) {
		it.second->draw();
	}
	for (auto it : bombs) {
		it.second->draw();
	}
}
void cStrategyManager::update(const float & deltatime)
{

	for (auto& it : drills) {
		it.second->update(deltatime);
	}
	for (auto& it : cannons) {
		it.second->update(deltatime);
	}
	for (auto& it : bombs) {
		it.second->update(deltatime);
	}
	deleteObject();

	if (ENV->pushKey(KeyEvent::KEY_p)) {
		cClientAdapter::getInstance()->sendSetQuarry(vec3(randInt(0, 10), 15, randInt(0, 10)),Strategy::cDrill::DrillType::Level1);

	}
}

void cStrategyManager::updateCollisionAfterUpdate(const float & deltaTime)
{
	for (auto& it : cannons) {
		it.second->updateCollisionAfterUpdate(deltaTime);
	}
	for (auto& it : bombs) {
		it.second->updateCollisionAfterUpdate(deltaTime);
	}
	for (auto& it : drills) {
		it.second->updateCollisionAfterUpdate(deltaTime);
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
	for (auto itr = cannons.begin();
		itr != cannons.end();) {
		if (itr->second->DeleteThis()) {
			itr = cannons.erase(itr);
		}
		else {
			itr++;
		}
	}
	for (auto itr = bombs.begin();
		itr != bombs.end();) {
		if (itr->second->DeleteThis()) {
			itr = bombs.erase(itr);
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
	drills[_id]->setup();
}
void cStrategyManager::CreateCannon(const ci::vec3 _pos, const float _direction, const int _id, const bool _ismyobject)
{
	cannons.insert(std::make_pair(_id, std::make_shared<Game::Strategy::cCannon>(_pos, _direction, _ismyobject)));
	cannons[_id]->setup();
}

void cStrategyManager::CreateBomb(const ci::vec3 _pos, const ci::vec3 _speed, const ci::vec3 _scale, const int _id)
{
	bombs.insert(std::make_pair(_id, std::make_shared<Game::Strategy::cBomb>(_pos, _speed, _scale, true)));
	bombs[_id]->setup();
}
void cStrategyManager::drawCube(const ci::vec3 pos, const ci::vec3 size, const ci::vec3 rotate, const ci::ColorA color)
{
		gl::pushModelView();
		gl::translate(pos);
		gl::rotate(rotate.x, vec3(1, 0, 0));
		gl::rotate(rotate.y, vec3(0, 1, 0));
		gl::rotate(rotate.z, vec3(0, 0, 1));
		gl::scale(size);
		gl::color(color);
		gl::drawCube(vec3(0, 0, 0), vec3(1, 1, 1));
		gl::popModelView();
}
void cStrategyManager::drawShere(const ci::vec3 pos, const ci::vec3 size, const ci::vec3 rotate, const ci::ColorA color, int segment)
{
	gl::pushModelView();
	gl::translate(pos);
	gl::rotate(rotate.x, vec3(1, 0, 0));
	gl::rotate(rotate.y, vec3(0, 1, 0));
	gl::rotate(rotate.z, vec3(0, 0, 1));
	gl::scale(size);
	gl::color(color);
	gl::drawSphere(vec3(0, 0, 0), 1, segment);
	gl::popModelView();
}
}


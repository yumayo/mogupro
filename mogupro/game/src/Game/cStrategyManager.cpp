#include<Game/cStrategyManager.h>
#include <Utility/cInput.h>
#include"Game/Strategy/cDrill.h"
#include"Game/Strategy/cStrategyObjectBase.h"
#include"cinder\Rand.h"
#include<algorithm>
#include"Game\cClientAdapter.h"
#include"Game\cFieldManager.h"
#include"Resource\cJsonManager.h"
#include <Game/cPlayerManager.h>
#include <Game/cShaderManager.h>
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

	Json::Value cannon=Resource::JSON["cannon.json"];
	ci::vec3 reddrawpos = jsonToVec3(cannon["reddrawpos"]) + ci::vec3(Field::WORLD_SIZE.x/2.f, Field::WORLD_SIZE.y,0);
	ci::vec3 bluedrawpos = jsonToVec3(cannon["bluedrawpos"]) + ci::vec3(Field::WORLD_SIZE.x / 2.f, Field::WORLD_SIZE.y, Field::WORLD_SIZE.z);
	ci::vec3 scale = ci::vec3((x_max / cannon["scalerate"].asFloat()));
	ci::vec3 hitscale = jsonToVec3(cannon["hitscale"]) * scale;
	ci::vec3 redhitpos = jsonToVec3(cannon["redhitpos"])+reddrawpos;
	ci::vec3 bluehitpos = jsonToVec3(cannon["bluehitpos"])+bluedrawpos;
	ci::vec3 foundationscale = jsonToVec3(cannon["foundationscale"])*scale;
	ci::vec3 foundationscale2 = jsonToVec3(cannon["foundationacale2"])*scale;

	ci::vec3 redfoundationtrancepos = jsonToVec3(cannon["redfoundationpos1"]);
	ci::vec3 redfoundationtrancepos2 = jsonToVec3(cannon["redfoundationpos2"]);
	ci::vec3 bluefoundationtrancepos = jsonToVec3(cannon["bluefoundationpos1"]);
	ci::vec3 bluefoundationtrancepos2 = jsonToVec3(cannon["bluefoundationpos2"]);
	{
		ci::vec3 pos = reddrawpos;
		ci::vec3 foundationpos = reddrawpos + redfoundationtrancepos;
		ci::vec3 foundationpos2 = reddrawpos + redfoundationtrancepos2;

		cannons.push_back(std::make_shared<Game::Strategy::cCannon>(pos,scale, foundationpos, foundationscale, foundationpos2, foundationscale2,pos+ jsonToVec3(cannon["redstorepos"]), redhitpos,hitscale, Game::Player::Team::Red));
	}
	{
		ci::vec3 pos = bluedrawpos;
		ci::vec3 foundationpos = bluedrawpos + bluefoundationtrancepos;
		ci::vec3 foundationpos2 = bluedrawpos + bluefoundationtrancepos2;
		cannons.push_back(std::make_shared<Game::Strategy::cCannon>(pos, scale, foundationpos, foundationscale, foundationpos2, foundationscale2,pos+ jsonToVec3(cannon["bluestorepos"]),bluehitpos, hitscale, Game::Player::Team::Blue));
	}



	for (int i = 0; i < cannons.size(); i++) {
		cannons[i]->setup();
	}

	///////キャノン作ります
}
void cStrategyManager::lightSetup()
{
	for (auto& p : cPlayerManager::getInstance()->getPlayers())
	{
		auto lights = p->getLightIds();
		pointLightIds.emplace_back(lights.p_id);
		spotLightIds.emplace_back(lights.s_id);
	}
}
void cStrategyManager::draw()
{
	// ////////////まとめておいたIDを使ってライトを反映させる。
	cShaderManager::getInstance()->uniformUpdate(pointLightIds, lineLightIds, spotLightIds);

	for (auto& it :cannons) {
		it->draw();
	}

	// ////////////ライトの反映解除
	cShaderManager::getInstance()->uniformUpdate();
}
void cStrategyManager::update(const float & deltatime)
{


	for (auto& it : cannons) {
		it->update(deltatime);
	}

	
	deleteObject();
}

void cStrategyManager::updateCollisionAfterUpdate(const float & deltaTime)
{

}

ci::vec3 cStrategyManager::jsonToVec3(Json::Value json)
{
	ci::vec3 Vec3;
	Vec3.x = json[0].asFloat();
	Vec3.y = json[1].asFloat();
	Vec3.z = json[2].asFloat();

	return Vec3;
}

void cStrategyManager::deleteObject()
{

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



std::vector<std::shared_ptr<Game::Strategy::cCannon>> cStrategyManager::getCannons()
{
	return cannons;
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
	gl::drawSphere(vec3(0, 0, 0), 1.f, segment);
	gl::popModelView();
}

}


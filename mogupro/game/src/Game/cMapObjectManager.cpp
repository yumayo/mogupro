#include <Game/cMapObjectManager.h>
#include "Utility\cInput.h"
#include "Resource\cJsonManager.h"
#include "Game/Field/FieldData.h"
using namespace ci;
using namespace ci::app;
namespace Game
{
cMapObjectManager::cMapObjectManager( )
{

}
cMapObjectManager::~cMapObjectManager( )
{

}
void cMapObjectManager::setup()
{
	mMapObjects.clear();
	createObjects();
}
void cMapObjectManager::draw()
{
	for (auto it : mMapObjects) {
		it->draw();
	}
	if (mIsDrawAabb) {
		for (auto it : mMapObjects) {
			it->drawAABB();
		}
	}
	
}
void cMapObjectManager::update(const float & deltatime)
{
	for (auto& it : mMapObjects) {
		it->update(deltatime);
	}
	if (ENV->pushKey(ci::app::KeyEvent::KEY_m)) {
		mMapObjects.clear();
		createObjects();
	}
	if (ENV->pushKey(ci::app::KeyEvent::KEY_n)) {
		mIsDrawAabb = (!mIsDrawAabb);
	}
}
void cMapObjectManager::beginCreateObjects()
{
	/*MapObjects.push_back(std::make_shared<Game::MapObjet::cMapObject>());
	mMapObjects[mMapObjects.size() - 1]->setup();*/
}
void cMapObjectManager::createObjects()
{
	ci::vec3 worldsize = Game::Field::WORLD_SIZE;
	ci::app::console() << worldsize << std::endl;
	Json::Value objects = Resource::JSON["mapdata.json"];

	for (int i = 0; i < objects.size(); i++) {
		Json::Value child = objects[i];
		mMapObjects.push_back(std::make_shared<Game::MapObjet::cMapObject>());
		mMapObjects[mMapObjects.size() - 1]->setup(child);
	}
}
void cMapObjectManager::createTemplate()
{

}
ci::vec3 cMapObjectManager::jsonToVec3(Json::Value json)
{
	ci::vec3 Vec3;
	Vec3.x = json[0].asFloat();
	Vec3.y = json[1].asFloat();
	Vec3.z = json[2].asFloat();

	return Vec3;
}
}

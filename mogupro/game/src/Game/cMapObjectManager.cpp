#include <Game/cMapObjectManager.h>
#include "Utility\cInput.h"
#include "Resource\cJsonManager.h"
#include "Game/Field/FieldData.h"
#include <Game/cShaderManager.h>
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
	// ////////////ƒ‰ƒCƒg‚Ì”½‰f‰ðœ
	cShaderManager::getInstance()->uniformUpdate();
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
	if (ENV->pushKey(ci::app::KeyEvent::KEY_j)) {
		SavePointSymmetry();
	}
}
void cMapObjectManager::beginCreateObjects()
{
	/*MapObjects.push_back(std::make_shared<Game::MapObjet::cMapObject>());
	mMapObjects[mMapObjects.size() - 1]->setup();*/
}
void cMapObjectManager::createObjects()
{
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
void cMapObjectManager::SavePointSymmetry()
{
	Json::Value objects = Resource::JSON["make.json"];
	std::vector<JsonTree>jsons;
	JsonTree kansei;

	for (int i = 0; i < objects.size(); i++)
	{
		JsonTree buf;
		buf.pushBack(JsonTree("name", objects[i]["name"].asString()));
		buf.pushBack(Vec3fToJson(getPointSymmetry(jsonToVec3(objects[i]["pos"])), "pos"));
		buf.pushBack(Vec3fToJson(jsonToVec3(objects[i]["scale"]), "scale"));
		buf.pushBack(JsonTree("rotatey",getSymmetryIndex(objects[i]["rotatey"].asInt())));
		jsons.push_back(buf);
	}
	for (int i = 0; i < jsons.size(); i++) {
		kansei.pushBack(jsons[i]);
	}
	console() << kansei << std::endl;
	kansei.write("copy.json");
}
ci::vec3 cMapObjectManager::jsonToVec3(Json::Value json)
{
	ci::vec3 Vec3;
	Vec3.x = json[0].asFloat();
	Vec3.y = json[1].asFloat();
	Vec3.z = json[2].asFloat();

	return Vec3;
}
ci::vec3 cMapObjectManager::getPointSymmetry(const ci::vec3 pos)
{
	vec3 symmetry;
	symmetry.x = std::abs(pos.x - Field::WORLD_SIZE.x);
	symmetry.y = pos.y;
	symmetry.z = std::abs(pos.z - Field::WORLD_SIZE.z);
	return symmetry;
}
int cMapObjectManager::getSymmetryIndex(const int index)
{
	return (index + 2) % 4;
}
ci::JsonTree cMapObjectManager::Vec3fToJson(const ci::vec3 vec, const std::string key)
{
	JsonTree jsonx = JsonTree("", vec.x);
	JsonTree jsony = JsonTree("", vec.y);
	JsonTree jsonz = JsonTree("", vec.z);
	JsonTree json = JsonTree::makeObject(key);
	json.pushBack(jsonx); json.pushBack(jsony); json.pushBack(jsonz);
	return json;
}
}

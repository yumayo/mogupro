#pragma once
#include "Utility\cSingletonAble.h"
#include "Game\MapObjet\cMapObject.h"
#include "cinder\Json.h"
namespace Game
{
class cMapObjectManager : public Utility::cSingletonAble<cMapObjectManager>
{
public:
    cMapObjectManager( );
    ~cMapObjectManager( );
	void setup();
	void draw();
	void update(const float& deltatime);
private:
	bool mIsDrawAabb = false;
	void beginCreateObjects();
	void createObjects();
	void createTemplate();
	void SavePointSymmetry();
	std::vector<std::shared_ptr<Game::MapObjet::cMapObject>>mMapObjects;
	std::map<std::string, ci::JsonTree> mJsonTemplate;
	ci::vec3 jsonToVec3(Json::Value json);
	ci::vec3 getPointSymmetry(const ci::vec3 pos);
	int getSymmetryIndex(const int index);
	ci::JsonTree Vec3fToJson(const ci::vec3 vec, const std::string key);
};
}

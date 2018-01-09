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
	std::vector<std::shared_ptr<Game::MapObjet::cMapObject>>mMapObjects;
	std::map<std::string, ci::JsonTree> mJsonTemplate;
	ci::vec3 jsonToVec3(Json::Value json);
};
}

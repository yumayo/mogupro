#include <Game/cCapsuleManager.h>
#include<Game/Capsule/cLightBombCapsule.h>
#include<Game/Capsule/cQuarryCapsule.h>
#include<Game/Field/FieldData.h>
#include<Game/cPlayerManager.h>
#include<Game/Field/FieldData.h>
#include<Game/Capsule/cQuarryCapsule.h>
#include<cinder\Rand.h>
//#include<Game/Weapons/SubWeapon/SubWeaponType.h>
namespace Game
{
cCapsuleManager::cCapsuleManager( )
{

}
cCapsuleManager::~cCapsuleManager( )
{

}
void cCapsuleManager::setup()
{
	BeginSetCapsels();
	/*ci::vec3 pos = ci::vec3(1, Game::Field::CHUNK_RANGE_Y*Game::Field::CHUNK_SIZE*Game::Field::BLOCK_SIZE,1);
	createCapsule(pos, Game::Weapons::SubWeapon::LIGHT_BOMB,debugcount);*/
}
void cCapsuleManager::draw()
{
	for (auto& itr : mCapsules) {
		if (glm::distance2(Game::cPlayerManager::getInstance()->getActivePlayer()->getPos(), itr.second->getPos()) < 30.f) {
			itr.second->draw();
		}	
	}
}
void cCapsuleManager::update(const float & deltatime)
{
	for (auto& itr : mCapsules) {
		itr.second->update(deltatime);
	}
	deleteCapsule();
	CollisionToPlayer();
}
void cCapsuleManager::deleteCapsule()
{
	for (auto itr = mCapsules.begin();
		itr != mCapsules.end();) {
		if (itr->second->deleteThis()) {
			itr = mCapsules.erase(itr);
		}
		else {
			itr++;
		}
	}
}
void cCapsuleManager::createCapsule(const ci::vec3 pos, const Game::Weapons::SubWeapon::SubWeaponType type, const int capsuleid)
{
	switch (type)
	{
	case Game::Weapons::SubWeapon::LIGHT_BOMB:
		mCapsules.insert(std::make_pair(capsuleid, std::make_shared<Game::Capsule::cLightBombCapsule>()));
		break;
	case  Game::Weapons::SubWeapon::QUARRY:
		mCapsules.insert(std::make_pair(capsuleid, std::make_shared<Game::Capsule::cQuarryCapsule>()));
		break;
	default:
		ci::app::console() << "カプセルが予想外です" << std::endl;
		break;
	}
	mCapsules[capsuleid]->setup(pos, type);
	debugcount++;
}

void cCapsuleManager::CollisionToPlayer()
{
	if (!Game::cPlayerManager::getInstance()->getActivePlayer()->useSubWeapon.canGetItem())return;

	for (auto& itr : mCapsules) {
		if (itr.second->getIsget())continue;
		if (Game::cPlayerManager::getInstance()->getActivePlayer()->getAABB().intersects(itr.second->getAABB())) {
			////////////////////////通信
			HitPlayer(Game::cPlayerManager::getInstance()->getActivePlayer()->getPlayerId(), itr.first);
			////////////////////////通信
		}
	}
}
void cCapsuleManager::HitPlayer(const int playerid, const int capsuleid)
{
	mCapsules[capsuleid]->setIsget(true);

	if (Game::cPlayerManager::getInstance()->getActivePlayerId() == playerid) {
		Game::cPlayerManager::getInstance()->getPlayers()[playerid]->useSubWeapon.addSubWeapon(mCapsules[capsuleid]->getType());
	}
}
void cCapsuleManager::BeginSetCapsels()
{
	float createnum = 15.f;

	ci::vec3 worldcenter(Field::BLOCK_SIZE*Field::CHUNK_RANGE_X*Field::CHUNK_SIZE / 2.f
		, Field::BLOCK_SIZE*Field::CHUNK_RANGE_Y*Field::CHUNK_SIZE / 2.f,
		Field::BLOCK_SIZE*Field::CHUNK_RANGE_Z*Field::CHUNK_SIZE / 2.f);

	int seed = 20171031;
	ci::randSeed(seed);

	for (float i = 0.0f; i < createnum; ++i) {
		float tranceX = ci::randFloat(-(worldcenter.x - 1.f), (worldcenter.x - 1.f));
		float tranceY = ci::randFloat(-(worldcenter.y - 1.f), (worldcenter.y - 1.f));
		float tranceZ = ci::randFloat(-(worldcenter.z - 1.f), (worldcenter.z - 1.f));
		ci::vec3 redpos = worldcenter + ci::vec3(-tranceX, tranceY, -tranceZ);
		ci::vec3 bluepos= worldcenter + ci::vec3(tranceX, tranceY, tranceZ);
		int type = ci::randInt(Game::Weapons::SubWeapon::SubWeaponType::MAX);

		createCapsule(redpos,Game::Weapons::SubWeapon::SubWeaponType(type),debugcount);
		createCapsule(bluepos, Game::Weapons::SubWeapon::SubWeaponType(type), debugcount);

	}
	
	


}
}

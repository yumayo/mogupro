#include <Game/cSubWeaponManager.h>
#include <Game/Weapons/SubWeapon/cLightBomb.h>
#include <Game/Weapons/UseSubWeapon/cUseLightBomb.h>
namespace Game
{
cSubWeaponManager::cSubWeaponManager( )
{

}
cSubWeaponManager::~cSubWeaponManager( )
{

}
void cSubWeaponManager::setup()
{

}
void cSubWeaponManager::draw()
{
	for (auto itr : usesubweapons) {
		itr->draw();
	}
	for (auto itr : subweapons) {
		itr.second->draw();
	}
}
void cSubWeaponManager::update(const float & deltatime)
{
	for (auto& itr : usesubweapons) {
		itr->update(deltatime);
	}
	for (auto& itr : subweapons) {
		itr.second->update(deltatime);
	}
	deleteWeapons();
}
void cSubWeaponManager::updateCollisionAfterUpdate(const float & deltaTime)
{
	for (auto& itr : subweapons) {
		itr.second->updateCollisionAfterUpdate(deltaTime);
	}
}

void cSubWeaponManager::HitDrillToGem(const int _objectid, const int _gemid)
{

}

void cSubWeaponManager::drawCube(const ci::vec3 pos, const ci::vec3 size, const ci::vec3 rotate, const ci::ColorA color)
{

}
void cSubWeaponManager::createLightBomb(const ci::vec3 _pos, const ci::vec3 _speed, const ci::vec3 _scale, const int _playerid)
{
	subweapons.insert(std::make_pair(debugidcount, std::make_shared<Game::Weapons::SubWeapon::cLightBomb>(_pos, _scale, _speed, _playerid)));
	subweapons[debugidcount]->setup();
	debugidcount++;
}

void cSubWeaponManager::createUseSubWeapon(const Game::Weapons::SubWeapon::SubWeaponType type, const int playerid)
{
	switch (type)
	{
	case Game::Weapons::SubWeapon::SubWeaponType::LIGHT_BOMB:
		usesubweapons.push_back(std::make_shared<Game::Weapons::UseSubWeapon::cUseLightBomb>());
		usesubweapons.back()->setup(playerid);
		return;
	default:
		return;
	}
}
void cSubWeaponManager::deleteWeapons()
{
	for (auto itr = usesubweapons.begin();
		itr != usesubweapons.end();) {
		if ((*itr)->deleteThis()) {
			itr = usesubweapons.erase(itr);
		}
		else {
			itr++;
		}
	}

	for (auto itr = subweapons.begin();
		itr != subweapons.end();) {
		if (itr->second->deleteThis()) {
			itr = subweapons.erase(itr);
		}
		else {
			itr++;
		}
	}
}
}

#include <Game/cSubWeaponManager.h>
#include <Game/Weapons/SubWeapon/cLightBomb.h>
#include <Game/Weapons/UseSubWeapon/cUseLightBomb.h>
#include <Game/Weapons/UseSubWeapon/cUseQuarry.h>
#include<Game/cPlayerManager.h>
#include<Resource\cSoundManager.h>
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
	cPlayerManager::getInstance()->getActivePlayer()->useSubWeapon.addSubWeapon(Game::Weapons::SubWeapon::SubWeaponType::QUARRY);
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
	for (auto& itr : usesubweapons) {
		itr->updateCollisionAfterUpdate(deltaTime);
	}
}

void cSubWeaponManager::HitDrillToGem(const int _objectid, const int _gemid)
{
	//ci::app::console() << "��Ԃ�" << std::endl;
	auto quarry = std::static_pointer_cast<Game::Weapons::SubWeapon::cQuarry>(subweapons[_objectid]);
	quarry->HitGem(_gemid);
}

void cSubWeaponManager::drawCube(const ci::vec3 pos, const ci::vec3 size, const ci::vec3 rotate, const ci::ColorA color)
{

}
void cSubWeaponManager::createLightBomb(const ci::vec3 _pos, const ci::vec3 _speed, const ci::vec3 _scale, const int objectid, const int _playerid)
{

	
	ci::app::console() << _playerid << _pos << objectid <<"���C�g�{���N���G�C�g"<< std::endl;
	subweapons.insert(std::make_pair(objectid, std::make_shared<Game::Weapons::SubWeapon::cLightBomb>(_pos, _scale, _speed, _playerid,objectid)));
	subweapons[objectid]->setup();
	debugidcount++;
	float gain = getGain(_playerid);
	Resource::cSoundManager::getInstance()->findSe("SubWeapon/lightbombthrow.wav").setGain(gain);
	Resource::cSoundManager::getInstance()->findSe("SubWeapon/lightbombthrow.wav").play();
}

void cSubWeaponManager::createQuarry(const ci::vec3 _pos, const int _objectid, const int playerid)
{
	ci::app::console() << playerid << _pos << _objectid << "�����[��[" << std::endl;
	subweapons.insert(std::make_pair(_objectid, std::make_shared<Game::Weapons::SubWeapon::cQuarry>(_pos, _objectid, playerid)));
	subweapons[_objectid]->setup();
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
	case Game::Weapons::SubWeapon::SubWeaponType::QUARRY:
		usesubweapons.push_back(std::make_shared<Game::Weapons::UseSubWeapon::cUseQuarry>());
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
float cSubWeaponManager::getGain(const int playerid)
{
	float gain = 0.4f - glm::distance2(cPlayerManager::getInstance()->getActivePlayer()->getPos(),
		cPlayerManager::getInstance()->getPlayers()[playerid]->getPos()) / 10.f;
	if (gain <= 0.0f) {
		gain = 0.0f;
	}
	return gain;
}
}

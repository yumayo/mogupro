#pragma once
#include"Utility\cSingletonAble.h"
#include"Game\cObjectBase.h"
#include"cinder\AxisAlignedBox.h"
#include"Strategy/cDrill.h"
#include"Strategy\cCannon.h"
#include"Strategy\cBomb.h"
#include<map>
#include<Game/Weapons/SubWeapon/cSubWeaponBase.h>
#include<Game/Weapons/UseSubWeapon/cUseSubBase.h>
#include<Game/Weapons/SubWeapon/SubWeaponType.h>
#include<Game/Weapons/SubWeapon/cQuarry.h>
namespace Game
{
class cSubWeaponManager : public Utility::cSingletonAble<cSubWeaponManager>
{
public:
    cSubWeaponManager( );
    ~cSubWeaponManager( );
	void setup();
	void draw();
	void update(const float& deltatime);
	void updateCollisionAfterUpdate(const float& deltaTime);
	void HitDrillToGem(const int _objectid, const int _gemid);
	//void CreateDrill(const ci::vec3 _pos, const int _id, const Strategy::cDrill::DrillType _type, const bool _ismyobject);
	//void CreateCannon(const ci::vec3 _pos, const float _direction, const int _id, const bool _ismyobject);
	//void CreateBomb(const ci::vec3 _pos, const ci::vec3 _speed, const ci::vec3 _scale, const int _id);
	//テスト用です
	void drawCube(const ci::vec3 pos, const ci::vec3 size, const ci::vec3 rotate, const ci::ColorA color);
	void createLightBomb(const ci::vec3 _pos, const ci::vec3 _speed, const ci::vec3 _scale,const int objectid, const int _playerid);
	void createQuarry(const ci::vec3 _pos, const int _objectid, const int playerid);
	void createUseSubWeapon(const Game::Weapons::SubWeapon::SubWeaponType type, const int playerid);
	void deleteWeapons();
	int debugidcount = 0;
private:
	std::map<int, std::shared_ptr<Game::Weapons::SubWeapon::cSubWeaponBase>>subweapons;
	std::list<std::shared_ptr<Game::Weapons::UseSubWeapon::cUseSubBase>>usesubweapons;
	float getGain(const int playerid);
	
};
}
#define SUBWM Game::cSubWeaponManager::getInstance()
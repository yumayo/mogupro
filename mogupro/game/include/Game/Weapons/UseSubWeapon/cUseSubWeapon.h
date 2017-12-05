#pragma once
#include<Game/Weapons/SubWeapon/SubWeaponType.h>
#include<vector>
namespace Game
{
namespace Weapons
{
namespace UseSubWeapon
{
class cUseSubWeapon
{
public:
    cUseSubWeapon( );
    ~cUseSubWeapon( );
	void setup();
	///サブウェポンを使う(先から使う)
	void useWeapon(const int playerid);
	///サブウェポンを追加
	void addSubWeapon(const Game::Weapons::SubWeapon::SubWeaponType type);
	void setCanUseWeapon(const bool iscanuse);
	int getItemNum();
private:
	//持っているサブの種類
	std::vector<Game::Weapons::SubWeapon::SubWeaponType> mSubWeaponTypes;
	//現在サブを使えるか（例 硬直中など）
	bool mIsCanUse = true;
};
}
}
}

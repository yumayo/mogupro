#pragma once
#include<Game/Weapons/UseSubWeapon/cUseSubBase.h>
namespace Game
{
	namespace Weapons
	{
		namespace UseSubWeapon
		{
			class cUseQuarry : public Game::Weapons::UseSubWeapon::cUseSubBase
			{
			public:
				cUseQuarry();
				~cUseQuarry();
				void setup(const int playerid) override;
				void update(const float& delta_time) override;
				void draw() override;
				bool deleteThis() override;
				void createSubWeapon() override;
			private:
				void calcPos(const float length,const int playerid);
				bool mIsdelete = false;
				ci::vec3 mSetPos;
				ci::vec3 mScale;
				float mLength;
			};
		}
	}
}

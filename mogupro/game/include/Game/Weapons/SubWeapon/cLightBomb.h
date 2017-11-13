#pragma once
#include <Game/Weapons/SubWeapon/cSubWeaponBase.h>
namespace Game
{
	namespace Weapons
	{
		namespace SubWeapon {

			class cLightBomb : public Game::Weapons::SubWeapon::cSubWeaponBase
			{
			public:
				cLightBomb(const ci::vec3 pos, const ci::vec3 scale, const ci::vec3 speed, const int playerid);
				~cLightBomb();
			private:

			public:
				void setup() override;
				void update(const float& delta_time) override;
				void updateCollisionAfterUpdate(const float& delta_time) override;
				void draw() override;
				bool deleteThis() override;;
			};
		};
	};
}
#pragma once
#include <Game/Weapons/WeaponBase.h>

namespace Game {
	namespace Weapon {
		class LightSaber : public Game::Weapon::WeaponBase {
		private:
		
			bool is_attack;
			void Attack();
		public:
			LightSaber();
			
			void setup() override;
			void update(const float& delta_time) override;
			void draw() override;
		};
	}
}
#pragma once
#include <Game/Weapons/WeaponBase.h>
#include <Node/action/action.h>
#include <vector>
namespace Game {
	namespace Weapon {
		class LightSaber : public Game::Weapon::WeaponBase {
		private:
			
			std::vector<bool> hits;
			void Attack();
			void Operation();
		public:
			LightSaber();
			
			void setup() override;
			void update(const float& delta_time) override;
			void draw() override;
		};
	}
}
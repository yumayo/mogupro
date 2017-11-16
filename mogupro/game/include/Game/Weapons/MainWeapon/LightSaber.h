#pragma once
#include <Game/Weapons/WeaponBase.h>
#include <Node/action/action.h>
#include <vector>
namespace Game {
	namespace Weapon {
		class LightSaber : public Game::Weapon::WeaponBase {
		private:
			ci::vec3 player_pos;
			ci::vec3 player_vec;
			std::vector<bool> hits;
			Utility::hardptr<Node::node> root;
			void Attack(const float & delta_time);
			void Operation();
		public:
			LightSaber();
			
			void setup() override;
			void update(const float& delta_time) override;
			void draw() override;
		};
	}
}
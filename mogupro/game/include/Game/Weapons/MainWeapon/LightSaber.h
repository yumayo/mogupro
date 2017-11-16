#pragma once
#include <Game/Weapons/WeaponBase.h>
#include <Node/action/action.h>
#include <cinder/gl/gl.h>
#include <vector>
#include <Game/cLightManager.h>
namespace Game {
	namespace Weapon {
		class LightSaber : public Game::Weapon::WeaponBase {
		private:
			//ÉÅÉbÉVÉÖ
			ci::gl::VboMeshRef mesh;
			ci::vec3 player_pos;
			ci::vec3 weapon_pos;
			ci::vec3 weapon_vec;
			ci::vec3 debug_pos;
			float player_rotate_x;
			float player_rotate_y;
			std::vector<bool> hits;
			Utility::hardptr<Node::node> root_x;
			Utility::hardptr<Node::node> root_y;
			Utility::cSoftPointer<Game::Light::cPointLightParam> light;

			void Attack(const float & delta_time);
			void Operation();
		public:
			LightSaber();
			ci::vec2 rotate;
			void setup() override;
			void update(const float& delta_time) override;
			void draw() override;
		};
	}
}
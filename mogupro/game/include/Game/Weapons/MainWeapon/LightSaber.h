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
			float player_rotate_x;
			float player_rotate_y;
			
			const float motion1 = 0.5f;
			
			ci::vec2 rotate_before_frame;
			std::vector<bool> hits;
			Utility::hardptr<Node::node> root_x;
			Utility::hardptr<Node::node> root_y;
			Utility::hardptr<Node::node> root_time;
			Utility::cSoftPointer<Game::Light::cPointLightParam> light;

			void Attack(const float & delta_time);
			void Rotation1(ci::vec2 rotate_buf, float rotation);
			void Operation();
		public:
			LightSaber();
			ci::vec2 rotate;
			ci::vec3 weapon_draw_pos;
			float timer;
			void setup() override;
			void update(const float& delta_time) override;
			void draw() override;
		};
	}
}
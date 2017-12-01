#pragma once
#include <Game/Weapons/WeaponBase.h>
#include <Node/action/action.h>
#include <cinder/gl/gl.h>
#include <vector>
#include <Game/cLightManager.h>
#include <Game/cPlayerManager.h>
namespace Game {
	namespace Weapon {

		enum Motion {
			attack_1,
			attack_2,
			attack_3,
		};
		class LightSaber : public Game::Weapon::WeaponBase {
		private:
			//ÉÅÉbÉVÉÖ
			ci::gl::VboMeshRef mesh;
			ci::vec3 player_pos;
			ci::vec3 weapon_pos;
			ci::vec3 weapon_vec;
			float player_rotate_x;
			float player_rotate_y;
			Motion motion;
			float timer;

			Player::Team team;

			ci::vec2 rotate_before_frame;
			std::vector<bool> hits;

			Utility::hardptr<Node::node> root_x;
			Utility::hardptr<Node::node> root_y;
			Utility::cSoftPointer<Game::Light::cPointLightParam> light;

			void Attack1();
			void Attack2();
			void CollisionPlayers();
			void CollisionDrills();
			void CollisionGems();
			void Attack(const float & delta_time);
			void Rotation1(ci::vec2 rotate_buf, float rotation);
			void Operation(const float & delta_time);
			void DrawRotate1();
			void DrawRotate2();
			void DrawRotate3();
		public:
			LightSaber();
			ci::vec2 rotate;
			ci::vec3 weapon_draw_pos;
			const float motion1 = 0.3f;
			const float motion2 = 0.3f;
			const float motion3 = 0.6f;
			void setup() override;
			void update(const float& delta_time) override;
			void draw() override;
		};
	}
}
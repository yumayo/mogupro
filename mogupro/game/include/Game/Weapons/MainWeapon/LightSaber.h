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
		enum ChargeMotion {
			charge_attack_1,
			charge_attack_2,
		};
		class LightSaber : public Game::Weapon::WeaponBase {
		private:
			//ÉÅÉbÉVÉÖ
			ci::gl::VboMeshRef mesh;
			ci::vec3 player_pos;
			ci::vec3 player_buf_pos;
			ci::vec3 player_vec;
			ci::vec3 weapon_pos;
			ci::vec3 weapon_vec;
			int player_id;
			float player_rotate_x;
			float player_rotate_y;
			Motion motion;
			ChargeMotion charge_motion;
			float timer;
			//pushÇpressÇÃÇÊÇ§Ç…àµÇ§
			bool is_push;

			//è’åÇÇÕ
			float shock_wave_time;
			//ÇΩÇﬂÇÈéûä‘
			const float shock_wave_first = 1.0f;
			const float shock_wave_second = 2.0f;
			ci::AxisAlignedBox shock_aabb;
			bool charge_flag1 = false;
			bool charge_flag2 = false;
			bool charge_is_attack = false;
			ci::vec3 shock_pos_buf;
			Game::Light::PointLightHandle charge_light;

			Player::Team team;

			ci::vec2 rotate_before_frame;
			std::unordered_map<int,bool> hits;

			Utility::hardptr<Node::node> root_x;
			Utility::hardptr<Node::node> root_y;
			Game::Light::PointLightHandle light;
			Utility::hardptr<Node::node> root_shock;
			void Attack1();
			void Attack2();
			void ShockCollisionPlayers();
			void ShockCollisionDrills();
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
			LightSaber(int player_id);
			ci::vec2 rotate;
			ci::vec3 weapon_draw_pos;
			ci::vec3 shock_pos;
			bool charge_is_attack_now = false;
			const float motion1 = 0.3f;
			const float motion2 = 0.3f;
			const float motion3 = 0.6f;
			void setup() override;
			void update(const float& delta_time) override;
			void draw() override;
		};
	}
}
#pragma once
#include <Game/cObjectBase.h>
#include <cinder/gl/gl.h>
#include <Collision/cAABBCollider.h>
#include <Collision/cRigidBody.h>
namespace Game {
	namespace Player {

		struct PlayerStatus {
			float attack;
			float drill_range;
			float jump_force;
			float speed;
		};

		//基準スピード
		static const float DEFAULT_SPEED = 5.0f;

		class cPlayer : public Game::cObjectBase {
		private:
			ci::vec3 size;
			ci::ColorA8u color;
			//メッシュ
			ci::gl::VboMeshRef mesh;
			//ステータス
			PlayerStatus status;
			//設置位置
			ci::vec3 installation_position;

			//何Pか
			int player_id;
			//操作するプレイヤーかどうか
			bool active_user;
			//掘削中
			bool drilling;
			//ジャンプ中はtrue
			bool jump_flag;

			//クォータニオンの例外用の角度
			float save_rotate;

			//プレイヤーの移動ベクトル保存
			ci::vec3 velocity;

			//当たり判定登録
			Collision::cAABBCollider mCollider;
			Collision::cRigidBody mRigidbody;

			//画像
			ci::gl::TextureRef texture;
			//Sphereのメッシュデータ
			ci::gl::BatchRef mSphere;
			//画像を貼るためのシェーダー
			ci::gl::GlslProgRef mGlsl;

			void playerRotation();
		public:
			cPlayer(const ci::vec3& pos, const ci::vec3& center_angle, const int& id, const bool& is_active_user);
			
			ci::vec3 getSize() {
				return size;
			}
			ci::vec3 getInstallationPosition() {
				return installation_position;
			}

			float getSpeed() {
				return status.speed;
			}
			void setSpeed(const float& speed) {
				status.speed = speed;
			}
			
			void setDefaultSpeed() {
				status.speed = DEFAULT_SPEED;
			}

			int getPlayerId() {
				return player_id;
			}
			
			//掘削中ならtrueを入れる
			void Drilling(const bool& flag) {
				drilling = flag;
			}
			//掘削中かどうかを返す
			bool isDrilling() {
				return drilling;
			}

			void move(const ci::vec3& velocity);
			void jump(bool flag);

			void setup() override;
			void update(const float& delta_time) override;
			void draw() override;
		};
	}
}
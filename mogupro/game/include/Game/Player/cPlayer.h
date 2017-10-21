#pragma once
#include <Game/cObjectBase.h>
#include <cinder/gl/gl.h>
#include <Collision/cAABBCollider.h>
#include <Collision/cRigidBody.h>
namespace Game {
	namespace Player {
		class cPlayer : public Game::cObjectBase {
		private:
			ci::vec3 size;
			ci::ColorA8u color;
			//メッシュ
			ci::gl::VboMeshRef mesh;
			//速度
			float speed;
			const float DEFAULT_SPEED = 5.0f;
			//何Pか
			int player_id;
			//操作するプレイヤーかどうか
			bool active_user;
			//掘削中
			bool drilling;

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

			float getSpeed() {
				return speed;
			}
			void setSpeed(const float& speed) {
				this->speed = speed;
			}
			
			void setDefaultSpeed() {
				this->speed = DEFAULT_SPEED;
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

			void setup() override;
			void update(const float& delta_time) override;
			void draw() override;
		};
	}
}
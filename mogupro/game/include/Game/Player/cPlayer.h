#pragma once
#include <Game/cObjectBase.h>
#include <cinder/gl/gl.h>
#include <Collision/cAABBCollider.h>
#include <Collision/cRigidBody.h>
#include <Node/action.hpp>
#include <Node/node.h>

namespace Game {
	namespace Gem {
		class cGem;
	}
	namespace Field
	{
		class cBreakBlockType;
	}
	namespace Player {

		struct PlayerStatus {
			float attack;
			float drill_range;
			float jump_force;
			float speed;
			float drill_speed;
		};

		enum Team {
			Red = 0,
			Blue = 1
		};

		//基準スピード
		static const float DEFAULT_SPEED = 5.0f;
		static const float DEFAULT_SIZE = 0.8f;

		class cPlayer : public Game::cObjectBase {
		private:
			ci::vec3 size;
			ci::ColorA8u color;
			//メッシュ
			ci::gl::VboMeshRef mesh;
			//ステータス
			PlayerStatus status;
			//掘ることが可能なブロックタイプ
			std::shared_ptr<Game::Field::cBreakBlockType> block_type;
			//設置位置
			ci::vec3 installation_position;
			//プレイヤーの方向ベクトル
			ci::vec3 player_vec;
			//チーム
			Team team;

			//プレイヤーのAABB
			ci::AxisAlignedBox aabb;

			//何Pか
			int player_id;
			//操作するプレイヤーかどうか
			bool active_user;
			//掘削中
			bool drilling;
			//ジャンプ中はtrue
			bool jump_flag;

			//クォータニオンの例外用の角度
			float save_rotate_y;
			float save_rotate_x;

			//プレイヤーからのカメラの位置
			float player_far;

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


			//イージング用
			ci::vec3 begin_pos;
			Utility::hardptr<Node::node> root;
			std::vector<std::shared_ptr<Game::Gem::cGem>>getgems;
			std::unordered_map<int,bool> gem_production_end;

			//Y軸回転
			void playerRotationY();
			//X軸回転
			void playerRotationX();

			//掘削時のカメラの遠さ調整
			void drillingCamera(const float& delta_time);
			void drill(const float& delta_time);

			//ジェム関連
			void getGems(const int& _gemid);
			void collisionGems();
			
			
		public:
			cPlayer(
				const ci::vec3& pos,
				const int& id,
				const bool& is_active_user,
				const Game::Player::Team& team);
			
			ci::vec3 getSize() {
				return size;
			}
			void setPos(const ci::vec3 pos) {
				if(active_user) return;
				velocity = pos - mPos;
				mCollider.setPosition(pos);
			}

			void resetPos() {
				mCollider.setPosition(ci::vec3(10,70,10));
			}

			ci::vec3 getInstallationPosition() {
				return installation_position;
			}
			//プレイヤーが向いている方向のベクトル
			ci::vec3 getPlayerVec() {
				return player_vec;
			}
			Team getWhichTeam() {
				return team;
			}

			ci::AxisAlignedBox getAABB() {
				return aabb;
			}
			
			float getSpeed() {
				return status.speed;
			}
			float getDrillSpeed() {
				return status.drill_speed;
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

			bool getActiveUser() {
				return active_user;
			}
			
			//コリジョンの後に呼び出す
			//プレイヤーの移動
			void setColliderSpeed() {
				mPos = mCollider.getPosition();
			}
			void gemsUpdate(const float& delta_time);



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
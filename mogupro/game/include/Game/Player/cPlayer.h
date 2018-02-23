#pragma once
#include <Game/cObjectBase.h>
#include <cinder/gl/gl.h>
#include <Collision/cAABBCollider.h>
#include <Collision/cRigidBody.h>
#include <Node/action.hpp>
#include <Node/node.h>
#include <Game/Weapons/UseSubWeapon/cUseSubWeapon.h>
#include <Game/Light/cPointLightParam.h>
#include <Game/Animation/cAnimation.h>
#include <Game/Light/cSpotLightParam.h>
namespace Game {
namespace Weapons
{
namespace MainWeapon
{
class cBase;
}
}

	namespace Gem {
		class cFragmentGem;
	}
	namespace Field
	{
		class cBreakBlockType;
	}

	namespace Player {

		struct PlayerStatus {
			float hp;
			float attack;
			float drill_range;
			float jump_force;
			float speed;
			float drill_speed;
			float respawn_time;
		};

		enum Team {
			Red = 0,
			Blue = 1
		};

		//基準スピード
		static const float DEFAULT_SPEED = 5.0f;
		static const cinder::vec3 DEFAULT_SIZE = cinder::vec3( 0.4F, 0.7F, 0.4F );

		static const float MAX_HP = 100.0f;

		static const float DEFAULT_RESPAWN_TIME = 7.0f;

		static const float DEFAULT_NO_DAMAGE_TIME = 3.0f;

		class cPlayer : public Game::cObjectBase {
		public:
			const std::string playerName;
		private:
			ci::vec3 size;
			ci::ColorA8u color;
			//メッシュ
			ci::gl::VboMeshRef mesh;
			//ステータス
			PlayerStatus status;
			//リスポーン位置
			ci::vec3 start_position;
			//掘ることが可能なブロックタイプ
			std::shared_ptr<Game::Field::cBreakBlockType> block_type;
			//設置位置
			ci::vec3 installation_position;
			//プレイヤーの方向ベクトル
			ci::vec3 normalized_player_vec;
			//チーム
			Team team;
			//死んでいるかどうか(true時、死亡中)
			bool is_dead;
			//再出撃までの待機時間のカウント
			float respawn_count;
			//再出撃してからの無敵時間用カウント
			float no_damage_count = 99;
			int no_damage_blind = 0;
			//攻撃された時の敵のid
			int damaged_id;
			//プレイヤーのAABB
			ci::AxisAlignedBox aabb;
			//アニメーション
			Animation::cAnimation animation;

			//メイン武器
			std::shared_ptr<Weapons::MainWeapon::cBase> main_weapon;

			//何Pか
			int player_id;
			//操作するプレイヤーかどうか
			bool active_user;
			//掘削中
			bool drilling;
			//ジャンプ中はtrue
			bool jump_flag;
			//スタン時（動けない状態）
			bool stan;
			//クォータニオン
			ci::quat rotation;

			//プレイヤーからのカメラの位置
			float player_far;

			//掘削時に６０フレームで音がならないように
			float drill_sound;

			//当たり判定登録
			Collision::cAABBCollider mCollider;
			Collision::cRigidBody mRigidbody;

			//画像
			ci::gl::TextureRef texture;
			//Sphereのメッシュデータ
			ci::gl::BatchRef mSphere;
			//画像を貼るためのシェーダー
			ci::gl::GlslProgRef mGlsl;

			//取得する一つ前のジェム
			int gem_id_buf = 0;

			//イージング用
			ci::vec3 begin_pos;
			Utility::hardptr<Node::node> root;

			// プレイヤーに常に付きまとうライト
			Light::PointLightHandle light;

			// プレイヤーのヘルメットから放たれるライト
			Light::SpotLightHandle spotlight;

			// ///// ライトのIDをまとめたデータを用意する
			std::vector<int> pointLightIds;
			std::vector<int> lineLightIds;
			std::vector<int> spotLightIds;
		public:
			struct { int p_id, s_id; } getLightIds()
			{
				return { light->getId(), spotlight->getId() };
			};
		private:

			//掘削時のカメラの遠さ調整
			void drillingCamera(const float& delta_time);
			void drill(const float& delta_time);

			//ジェム関連
			void collisionGems();
			
			//死んだときのジェムの処理
			void deadGems();
			void dead();
			void respawn(const float& delta_time);
			void resetPlayerStatus();
		public:
			cPlayer(
				const ci::vec3& pos,
				const int& id,
				const bool& is_active_user,
				const int& main_weapon_id,
				const int& sub_weapon_id,
				const Game::Player::Team& team,
				const std::string& name);
			~cPlayer() {
				mCollider.removeWorld();
				mRigidbody.removeWorld();
			}
			//ジェムの演出が終わっているかどうか
			std::unordered_map<int, bool> gem_production_end;
			/////アイテムを使用するのに使います
			Game::Weapons::UseSubWeapon::cUseSubWeapon useSubWeapon;
			std::vector<std::shared_ptr<Game::Gem::cFragmentGem>>getgems;
			ci::vec3 getSize() {
				return size;
			}

			PlayerStatus getStatus() {
				return status;
			}
			ci::vec3 getPos() override {
				return mCollider.getPosition();
			}
			void setRotate(const ci::quat quatanion) {
				rotation = quatanion;
			}

			void setPos(const ci::vec3 pos, const ci::quat quatanion) {
				if (active_user) {
					return;
				}
				rotation = quatanion;
				auto vec = pos - mCollider.getPosition();
				if (vec.x >= 0.01f ||
					vec.x <= -0.01f ||
					vec.z >= 0.01f ||
					vec.z <= -0.01f) {
					normalized_player_vec = glm::normalize(vec);
				}
				mCollider.setPosition(pos);
			}
			void setPos(const ci::vec3 pos) {
				if (active_user) {
					return;
				}
				auto vec = pos - mCollider.getPosition();
				if (vec.x >= 0.01f ||
					vec.x <= -0.01f ||
					vec.z >= 0.01f ||
					vec.z <= -0.01f) {
					normalized_player_vec = glm::normalize(vec);
				}
				mCollider.setPosition(pos);
			}

			float& getPlayerFar() {
				return player_far;
			}
			void setActivePlayer(const bool& flag) {
				active_user = flag;
			}

			//強制的にその座標にしたいときのセット
			void settingPosition(const ci::vec3 pos) {
				mCollider.setPosition(pos);
			}
			bool isWatching( )
			{
				return (player_id == 3U) || (player_id == 7U);
			}

			//プレイヤーが動けない状態かどうか
			bool getStan() {
				return stan;
			}
			//プレイヤーが死んでいるかどうか（死亡中はtrue）
			bool isDead() {
				return is_dead;
			}

			bool getJumpFlag() {
				return jump_flag;
			}

			//攻撃してきた敵のid
			int getDamagedId() {
				return damaged_id;
			}
			int getRespawnCount() {
				return respawn_count;
			}

			void resetPos() {
				mCollider.setPosition(ci::vec3(0,70,0));
			}

			ci::vec3 getInstallationPosition() {
				return installation_position;
			}
			//プレイヤーが向いている方向のベクトル
			ci::vec3 getPlayerVec() {
				return normalized_player_vec;
			}

			//プレイヤーが向いている方向のベクトルの参照
			ci::vec3& getReferencePlayerVec() {
				return normalized_player_vec;
			}

			Animation::cAnimation& getPlayerAnimation() {
				return animation;
			}

			Team getWhichTeam() {
				return team;
			}

			ci::AxisAlignedBox getAABB() {
				return aabb;
			}
			Game::Weapons::MainWeapon::cBase* getMainWeapon( );
			
			float getSpeed() {
				return status.speed;
			}
			float getDrillSpeed() {
				return status.drill_speed;
			}
			void setStan(const bool& stan) {
				this->stan = stan;
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

			ci::quat getRotate() {
				return rotation;
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

			void setPlayerVec(ci::vec3 vec) {
				normalized_player_vec = glm::normalize(vec);
			}
			//ジェム関連
			void getGems(const int& _gemid);

			//attack = 攻撃力
			//player_id = 攻撃を仕掛けたプレイヤーのid
			void receiveDamage(const float& attack,float player_id);
			void weaponUpdae(const float& delta_time);
			void move(const ci::vec3& velocity);
			void jump(bool flag);

			//回転
			void updatePlayerRotation();
			cinder::mat4 getWorldMatrixWeapon( ) const;

			void setup() override;
			void update(const float& delta_time) override;
			void cameraAfterUpdate( const float& delta_time );
			void draw() override;
		};
	}
}
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

		//��X�s�[�h
		static const float DEFAULT_SPEED = 5.0f;
		static const cinder::vec3 DEFAULT_SIZE = cinder::vec3( 0.4F, 0.7F, 0.4F );

		static const float MAX_HP = 100.0f;

		static const float DEFAULT_RESPAWN_TIME = 7.0f;

		static const float DEFAULT_NO_DAMAGE_TIME = 3.0f;

		class cPlayer : public Game::cObjectBase {
		private:
			ci::vec3 size;
			ci::ColorA8u color;
			//���b�V��
			ci::gl::VboMeshRef mesh;
			//�X�e�[�^�X
			PlayerStatus status;
			//���X�|�[���ʒu
			ci::vec3 start_position;
			//�@�邱�Ƃ��\�ȃu���b�N�^�C�v
			std::shared_ptr<Game::Field::cBreakBlockType> block_type;
			//�ݒu�ʒu
			ci::vec3 installation_position;
			//�v���C���[�̕����x�N�g��
			ci::vec3 normalized_player_vec;
			//�`�[��
			Team team;
			//����ł��邩�ǂ���(true���A���S��)
			bool is_dead;
			//�ďo���܂ł̑ҋ@���Ԃ̃J�E���g
			float respawn_count;
			//�ďo�����Ă���̖��G���ԗp�J�E���g
			float no_damage_count = 99;
			int no_damage_blind = 0;
			//�U�����ꂽ���̓G��id
			int damaged_id;
			//�v���C���[��AABB
			ci::AxisAlignedBox aabb;
			//�A�j���[�V����
			Animation::cAnimation animation;

			//���C������
			std::shared_ptr<Weapons::MainWeapon::cBase> main_weapon;

			//��P��
			int player_id;
			//���삷��v���C���[���ǂ���
			bool active_user;
			//�@�풆
			bool drilling;
			//�W�����v����true
			bool jump_flag;
			//�X�^�����i�����Ȃ���ԁj
			bool stan;
			//�N�H�[�^�j�I��
			ci::quat rotation;

			//�v���C���[����̃J�����̈ʒu
			float player_far;

			//�@�펞�ɂU�O�t���[���ŉ����Ȃ�Ȃ��悤��
			float drill_sound;

			//�����蔻��o�^
			Collision::cAABBCollider mCollider;
			Collision::cRigidBody mRigidbody;

			//�摜
			ci::gl::TextureRef texture;
			//Sphere�̃��b�V���f�[�^
			ci::gl::BatchRef mSphere;
			//�摜��\�邽�߂̃V�F�[�_�[
			ci::gl::GlslProgRef mGlsl;

			//�擾�����O�̃W�F��
			int gem_id_buf = 0;

			//�C�[�W���O�p
			ci::vec3 begin_pos;
			Utility::hardptr<Node::node> root;

			// �v���C���[�ɏ�ɕt���܂Ƃ����C�g
			Light::PointLightHandle light;

			// �v���C���[�̃w�����b�g���������郉�C�g
			Light::SpotLightHandle spotlight;

			// ///// ���C�g��ID���܂Ƃ߂��f�[�^��p�ӂ���
			std::vector<int> pointLightIds;
			std::vector<int> lineLightIds;
			std::vector<int> spotLightIds;
		public:
			struct { int p_id, s_id; } getLightIds()
			{
				return { light->getId(), spotlight->getId() };
			};
		private:

			//�@�펞�̃J�����̉�������
			void drillingCamera(const float& delta_time);
			void drill(const float& delta_time);

			//�W�F���֘A
			void collisionGems();
			
			//���񂾂Ƃ��̃W�F���̏���
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
				const Game::Player::Team& team);
			~cPlayer() {
				mCollider.removeWorld();
				mRigidbody.removeWorld();
			}
			//�W�F���̉��o���I����Ă��邩�ǂ���
			std::unordered_map<int, bool> gem_production_end;
			/////�A�C�e�����g�p����̂Ɏg���܂�
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

			//�����I�ɂ��̍��W�ɂ������Ƃ��̃Z�b�g
			void settingPosition(const ci::vec3 pos) {
				mCollider.setPosition(pos);
			}
			bool isWatching( )
			{
				return (player_id == 3U) || (player_id == 7U);
			}

			//�v���C���[�������Ȃ���Ԃ��ǂ���
			bool getStan() {
				return stan;
			}
			//�v���C���[������ł��邩�ǂ����i���S����true�j
			bool isDead() {
				return is_dead;
			}

			bool getJumpFlag() {
				return jump_flag;
			}

			//�U�����Ă����G��id
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
			//�v���C���[�������Ă�������̃x�N�g��
			ci::vec3 getPlayerVec() {
				return normalized_player_vec;
			}

			//�v���C���[�������Ă�������̃x�N�g���̎Q��
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
			
			//�R���W�����̌�ɌĂяo��
			//�v���C���[�̈ړ�
			void setColliderSpeed() {
				mPos = mCollider.getPosition();
			}
			void gemsUpdate(const float& delta_time);

			//�@�풆�Ȃ�true������
			void Drilling(const bool& flag) {
				drilling = flag;
			}
			//�@�풆���ǂ�����Ԃ�
			bool isDrilling() {
				return drilling;
			}

			void setPlayerVec(ci::vec3 vec) {
				normalized_player_vec = glm::normalize(vec);
			}
			//�W�F���֘A
			void getGems(const int& _gemid);

			//attack = �U����
			//player_id = �U�����d�|�����v���C���[��id
			void receiveDamage(const float& attack,float player_id);
			void weaponUpdae(const float& delta_time);
			void move(const ci::vec3& velocity);
			void jump(bool flag);

			//��]
			void updatePlayerRotation();
			cinder::mat4 getWorldMatrixWeapon( ) const;

			void setup() override;
			void update(const float& delta_time) override;
			void cameraAfterUpdate( const float& delta_time );
			void draw() override;
		};
	}
}
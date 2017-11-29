#pragma once
#include <Game/cObjectBase.h>
#include <cinder/gl/gl.h>
#include <Collision/cAABBCollider.h>
#include <Collision/cRigidBody.h>
#include <Node/action.hpp>
#include <Node/node.h>
#include <Game/Weapons/WeaponBase.h>
#include<Game/Weapons/UseSubWeapon/cUseSubWeapon.h>
namespace Game {
	namespace Gem {
		class cGemStone;
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
		static const float DEFAULT_SIZE = 0.8f;

		class cPlayer : public Game::cObjectBase {
		private:
			ci::vec3 size;
			ci::ColorA8u color;
			//���b�V��
			ci::gl::VboMeshRef mesh;
			//�X�e�[�^�X
			PlayerStatus status;
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
			//�U�����ꂽ���̓G��id
			int damaged_id;
			//�v���C���[��AABB
			ci::AxisAlignedBox aabb;

			//���C������
			std::unique_ptr<Weapon::WeaponBase> main_weapon;

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
			//�N�H�[�^�j�I���̗�O�p�̊p�x
			float save_rotate_y;
			float save_rotate_x;

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


			//�C�[�W���O�p
			ci::vec3 begin_pos;
			Utility::hardptr<Node::node> root;
			std::vector<std::shared_ptr<Game::Gem::cGemStone>>getgems;
			std::unordered_map<int,bool> gem_production_end;

			//Y����]
			void playerRotationY();
			//X����]
			void playerRotationX();

			//�@�펞�̃J�����̉�������
			void drillingCamera(const float& delta_time);
			void drill(const float& delta_time);

			//�W�F���֘A
			void getGems(const int& _gemid);
			void collisionGems();
			
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
			/////�A�C�e�����g�p����̂Ɏg���܂�
			Game::Weapons::UseSubWeapon::cUseSubWeapon useSubWeapon;

			ci::vec3 getSize() {
				return size;
			}

			PlayerStatus getStatus() {
				return status;
			}
			ci::vec3 getPos() override {
				return mCollider.getPosition();
			}
			void setPos(const ci::vec3 pos) {
				if(active_user) return;
				mCollider.setPosition(pos);
			}
			//�v���C���[�������Ȃ���Ԃ��ǂ���
			bool getStan() {
				return stan;
			}

			//�v���C���[������ł��邩�ǂ����i���S����true�j
			bool isDead() {
				return is_dead;
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
			Team getWhichTeam() {
				return team;
			}

			ci::AxisAlignedBox getAABB() {
				return aabb;
			}
			Weapon::WeaponBase* getMainWeapon() {
				return main_weapon.get();
			}
			
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

			float getRotateY() {
				return save_rotate_y;
			}
			float getRotateX() {
				return save_rotate_x;
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
			//attack = �U����
			//player_id = �U�����d�|�����v���C���[��id
			void receiveDamage(const float& attack, const float& player_id);
			void weaponUpdae(const float& delta_time);
			void move(const ci::vec3& velocity);
			void jump(bool flag);

			void setup() override;
			void update(const float& delta_time) override;
			void draw() override;
		};
	}
}
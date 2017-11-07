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
			ci::vec3 player_vec;
			//�`�[��
			Team team;

			//�v���C���[��AABB
			ci::AxisAlignedBox aabb;

			//��P��
			int player_id;
			//���삷��v���C���[���ǂ���
			bool active_user;
			//�@�풆
			bool drilling;
			//�W�����v����true
			bool jump_flag;

			//�N�H�[�^�j�I���̗�O�p�̊p�x
			float save_rotate_y;
			float save_rotate_x;

			//�v���C���[����̃J�����̈ʒu
			float player_far;

			//�v���C���[�̈ړ��x�N�g���ۑ�
			ci::vec3 velocity;

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
			std::vector<std::shared_ptr<Game::Gem::cGem>>getgems;
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
			//�v���C���[�������Ă�������̃x�N�g��
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

			void move(const ci::vec3& velocity);
			void jump(bool flag);

			void setup() override;
			void update(const float& delta_time) override;
			void draw() override;
		};
	}
}
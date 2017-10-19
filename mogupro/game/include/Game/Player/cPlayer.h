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
			//���x
			float speed;
			//��P��
			int player_id;
			//���삷��v���C���[���ǂ���
			bool active_user;
			//�@�풆
			bool drilling;

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

			void playerRotation();
			float angleDifference(const float & angle1, const float & angle2);
		public:
			cPlayer(const ci::vec3& pos, const ci::vec3& center_angle, const int& id, const bool& is_active_user);
			
			ci::vec3 getSize() {
				return size;
			}

			float getSpeed() {
				return speed;
			}

			int getPlayerId() {
				return player_id;
			}
			
			//�@�풆�Ȃ�true������
			void Drilling(const bool& flag) {
				drilling = flag;
			}
			//�@�풆���ǂ�����Ԃ�
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
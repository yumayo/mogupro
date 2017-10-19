#include <Game/Player/cPlayer.h>
#include <Collision/cAABBCollider.h>
#include <Game/cFieldManager.h>
#include <CameraManager/cCameraManager.h>
//void Game::Player::cPlayer::playerRotation()
//{
//	//�v���C���[�̑O����
//	ci::vec3 rotateaxis = ci::vec3(0.0f, 0.0f, 1.0f);
//	//�ړ���̃x�N�g��
//	ci::vec3 targetvec = ci::vec3(velocity.x, 0, velocity.z);
//	//�ړ�������normalize
//	targetvec = ci::normalize(targetvec);
//	//��]��
//	ci::vec3 quataxis = glm::cross(rotateaxis, targetvec);
//	//��]
//	// �����
//	if (atan2f(velocity.x, velocity.z) > 0.0f)
//		ci::gl::rotate(atan2f(velocity.x, velocity.z), quataxis);
//	// �E���
//	else if (atan2f(velocity.x, velocity.z) < 0.0f)
//		ci::gl::rotate(-atan2f(velocity.x, velocity.z), quataxis);
//
//	if (quataxis == ci::vec3(0))
//	{
//		if (velocity.z > 0.0f)
//			ci::gl::rotate(0, ci::vec3(0, 1, 0));
//		if (velocity.z < 0.0f)
//			ci::gl::rotate((float)M_PI, ci::vec3(0, 1, 0));
//	}
//
//}

void Game::Player::cPlayer::playerRotation()
{
	//�v���C���[�̑O����
	ci::vec3 rotateaxis = ci::vec3(0.0f, 0.0f, 1.0f);
	//�ړ���̃x�N�g��
	ci::vec3 targetvec = ci::vec3(velocity.x, 0, velocity.z);
	//�ړ�������normalize
	targetvec = ci::normalize(targetvec);

	//��]��
	ci::vec3 quataxis = glm::cross(rotateaxis, targetvec);

	float rotation = 0;

	//��]
	// �����
	if (atan2f(velocity.x, velocity.z) > 0.0f)
		ci::gl::rotate(atan2f(velocity.x, velocity.z), quataxis);
	// �E���
	else if (atan2f(velocity.x, velocity.z) < 0.0f)
		ci::gl::rotate(-atan2f(velocity.x, velocity.z), quataxis);

	if (quataxis == ci::vec3(0))
	{
		if (velocity.z > 0.0f)
			ci::gl::rotate(0, ci::vec3(0, 1, 0));
		if (velocity.z < 0.0f)
			ci::gl::rotate((float)M_PI, ci::vec3(0, 1, 0));
	}
}

float Game::Player::cPlayer::angleDifference(const float & angle1, const float & angle2)
{
	if (angle1 == angle2)
	{
		return 0.0f;
	}
	float angle_difference = angle1 - angle2;
	if (angle_difference < -M_PI)
	{
		angle_difference = 2.0f * M_PI + angle_difference;
	}
	else if (angle_difference > M_PI)
	{
		angle_difference = 2.0f * -M_PI + angle_difference;
	}
	angle_difference = std::fmodf(angle_difference, 2.0f * M_PI);
	return angle_difference;
}

//�R���X�g���N�^��
//�ʒu�Ɗp�x�Ɖ�P���Ƃǂꂪ�����̃v���C���[����
//���炤
Game::Player::cPlayer::cPlayer(
	const ci::vec3& pos,
	const ci::vec3& center_angle,
	const int& id,
	const bool& is_active_user)
	: cObjectBase(pos),
	mCollider(mPos, ci::vec3(0.8F)),
	mRigidbody(mCollider)
{
	size = ci::vec3(1);
	color = ci::vec4(1);
	color = ci::ColorA8u(1, 0, 1, 1);
	velocity = ci::vec3(0);
	drilling = false;
	speed = 5.0f;
	player_id = id;
	active_user = is_active_user;
}

void Game::Player::cPlayer::move(const ci::vec3 & velocity)
{
	//�v���C���[�̈ړ��x�N�g���ۑ�
	this->velocity = velocity;

	//�v���C���[�̈ړ�
	mPos += velocity;

}

void Game::Player::cPlayer::setup()
{
}

void Game::Player::cPlayer::update(const float & delta_time)
{
	if (drilling) {
		Game::cFieldManager::getInstance()->blockBreak(mPos, 1);
	}
	/*mPos = mCollider.getPosition();
	size = mCollider.getSize();*/
}

void Game::Player::cPlayer::draw()
{
	ci::gl::pushModelView();
	ci::gl::translate(mPos);
	ci::gl::scale(ci::vec3(1));
	playerRotation();
	ci::gl::color(color);
	ci::gl::ScopedGlslProg sgp(ci::gl::getStockShader(ci::gl::ShaderDef().lambert()));
	ci::gl::drawCube(ci::vec3(0, 0, 0), ci::vec3(1, 1, 1));
	ci::gl::color(1, 1, 1, 1);
	ci::gl::popModelView();
}

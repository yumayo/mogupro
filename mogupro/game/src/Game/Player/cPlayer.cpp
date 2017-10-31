#include <Game/Player/cPlayer.h>
#include <Collision/cAABBCollider.h>
#include <Game/cFieldManager.h>
#include <CameraManager/cCameraManager.h>
#include <Resource/cObjectManager.h>
#include <Resource/TextureManager.h>


void Game::Player::cPlayer::playerRotation()
{
	//�v���C���[�̑O����
	ci::vec3 rotateaxis = ci::vec3(0.0f, 0.0f, 1.0f);
	//�ړ���̃x�N�g��
	ci::vec3 targetvec = ci::vec3(velocity.x, 0, velocity.z);
	
	//�ړ�������normalize
	targetvec = ci::normalize(targetvec);
	if (velocity.x >= 0.01f ||
		velocity.x <= -0.01f ||
		velocity.z >= 0.01f ||
		velocity.z <= -0.01f )
		installation_position = ci::vec3(velocity.x * 10, 0, velocity.z * 10);

	//��]��
	ci::vec3 quataxis = glm::cross(rotateaxis, targetvec);

	//�����x�N�g������������Ԃ��ƃN�H�[�^�j�I����
	//�O�ɂȂ�B
	//�ŏ��̃x�N�g���Ƃ̊O�ς�����Ă���̂ŁA�O
	//�ɂȂ�̂�Z�ƕ��s�ȃx�N�g���̎������B
	//�Ȃ̂�Z���O�̎��ƂP�̎�������O�Ƃ��Ċp�x
	//��^���Ȃ���΂Ȃ�Ȃ�
	if (quataxis == ci::vec3(0)) {
		if (velocity.z > 0.0f) {
			ci::gl::rotate(0, ci::vec3(0, 1, 0));
			save_rotate = 0;
		}
		if (velocity.z < 0.0f) {
			ci::gl::rotate(M_PI, ci::vec3(0, 1, 0));
			save_rotate = M_PI;
		}

		return;
	}

	float rotation = atan2f(velocity.x, velocity.z);

	//��]
	// �����
	if (rotation > 0.1f) {
		ci::gl::rotate(rotation, quataxis);
		save_rotate = rotation;
	}
	// �E���
	else if (rotation < -0.1f) {
		ci::gl::rotate(-rotation, quataxis);
		save_rotate = rotation;
	}

	//�x�N�g�����o�Ȃ����
	if (rotation <= 0.1f &&
		rotation >= -0.1f) {
		if (save_rotate > 0.1f)
			ci::gl::rotate(save_rotate, ci::vec3(0, 1, 0));
		if (save_rotate < -0.1f)
			ci::gl::rotate(save_rotate, ci::vec3(0, 1, 0));
	}

}


//�R���X�g���N�^��
//�ʒu�Ɗp�x�Ɖ�P���Ƃǂꂪ�����̃v���C���[����
//���炤
Game::Player::cPlayer::cPlayer(
	const ci::vec3& pos,
	const int& id,
	const bool& is_active_user,
	const Game::Player::Team& team)
	: cObjectBase(pos),
	mCollider(mPos, ci::vec3(0.8f, 0.8f, 0.8f)),
	mRigidbody(mCollider)
{
	size = ci::vec3(1);
	color = ci::vec4(1);
	color = ci::ColorA8u(1, 0, 1, 1);
	velocity = ci::vec3(0);
	this->team = team;
	save_rotate = 0;
	drilling = false;
	jump_flag = false;

	//�v���C���[�̃X�e�[�^�X
	status.attack = 10;
	status.drill_range = 1;
	status.jump_force = 0.6F;
	status.speed = DEFAULT_SPEED;
	//�ݒu�ʒu
	installation_position = ci::vec3(0,0,2);

	player_id = id;
	active_user = is_active_user;
}

void Game::Player::cPlayer::move(const ci::vec3 & velocity)
{

	auto speed = mRigidbody.getSpeed() - this->velocity;

	//�v���C���[�̈ړ��x�N�g���ۑ�
	this->velocity = velocity;

	if (active_user) {
		//�n�ʂ̒��Ō@�풆�Ȃ�d�͂��Ȃ���
		if (mPos.y <= Game::cFieldManager::getInstance()->getBlockTopPosition(mPos).y && drilling) {
			mRigidbody.gravityOff();
		}
		else {
			mRigidbody.gravityOn();
		}
	}

	mRigidbody.setSpeed(ci::vec3(0,speed.y,0) + velocity);
}

void Game::Player::cPlayer::jump(bool flag)
{
	if(jump_flag == false)
	jump_flag = flag;

	if (jump_flag == true) {
		if (mRigidbody.isLanding()) {
			velocity.y = -status.jump_force;
			jump_flag = false;
		}
	}
}

void Game::Player::cPlayer::setup()
{
    mCollider.setLayer( 1 << 0 );
	mCollider.addWorld();
	mRigidbody.addWorld();

	//�ŏ��Ɋp�x��ݒ肷�邽�߂ɂق�̏���������
	move(ci::vec3(0,0, 0.01f));
	mCollider.setPosition(mPos + ci::vec3(0, 0, 0.01f));
	//�v���C���[�̈ړ�
	mPos = mCollider.getPosition();

	//�����ȊO�͒ʐM�ňʒu�������Ă���̂�
	//�d�͂�������K�v���Ȃ�
	if (!active_user)mRigidbody.gravityOff();
	
	mesh = Resource::cObjectManager::getInstance()->findObject("montamogura/moguraHontai.obj");
	TEX->set("mogura", "OBJ/montamogura/moguraHontai.png");
}

#include <Game/cClientAdapter.h>
void Game::Player::cPlayer::update(const float & delta_time)
{
	if (drilling) {
		Game::cFieldManager::getInstance()->blockBreak(mCollider.getPosition() + ci::vec3(0, -1,0), 1);
	}
}

void Game::Player::cPlayer::draw()
{
    ci::gl::ScopedColor col( color );
    ci::gl::ScopedTextureBind tex( TEX->get( "mogura" ) );
    ci::gl::ScopedGlslProg glsl( ci::gl::getStockShader( ci::gl::ShaderDef( ).texture( ) ) );

	ci::gl::pushModelView();
	ci::gl::translate(mPos - ci::vec3(0, 0.5f, 0));
	playerRotation();
	ci::gl::scale(ci::vec3(0.01f, 0.01f, 0.012f));
	ci::gl::draw(mesh);
	ci::gl::popModelView();
}

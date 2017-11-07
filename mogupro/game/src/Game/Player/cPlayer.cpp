#include <Game/Player/cPlayer.h>
#include <Collision/cAABBCollider.h>
#include <Game/cFieldManager.h>
#include <CameraManager/cCameraManager.h>
#include <Resource/cObjectManager.h>
#include <Resource/TextureManager.h>
#include <Game/cGemManager.h>
#include <Game/Field/cBreakBlockType.h>

void Game::Player::cPlayer::playerRotationY()
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
		velocity.z <= -0.01f) {
		installation_position = ci::vec3(velocity.x * 10, 0, velocity.z * 10);
		ci::vec3 rotateaxis_x = ci::vec3(velocity.x * 10, velocity.y * 10, velocity.z * 10);
	}
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
			save_rotate_y = 0;
		}
		if (velocity.z < 0.0f) {
			ci::gl::rotate(M_PI, ci::vec3(0, 1, 0));
			save_rotate_y = M_PI;
		}
		return;
	}

	float rotation = atan2f(velocity.x, velocity.z);

	//��]
	// �����
	if (rotation > 0) {
		ci::gl::rotate(rotation, quataxis);
		save_rotate_y = rotation;
	}
	// �E���
	else if (rotation < 0) {
		ci::gl::rotate(-rotation, quataxis);
		save_rotate_y = rotation;
	}

	//�x�N�g�����o�Ȃ����
	if (rotation <= 0.1f &&
		rotation >= -0.1f) {
		if (save_rotate_y > 0.1f)
			ci::gl::rotate(save_rotate_y, ci::vec3(0, 1, 0));
		if (save_rotate_y < -0.1f)
			ci::gl::rotate(save_rotate_y, ci::vec3(0, 1, 0));
	}

}

void Game::Player::cPlayer::playerRotationX()
{
	if (!drilling) return;
	//�v���C���[�̑O����
	ci::vec3 rotateaxis = ci::vec3(0.0f, 1.0f, 0.0f);

	//�ړ���̃x�N�g��
	ci::vec3 targetvec = ci::vec3(0, velocity.y, velocity.z);

	//�ړ�������normalize
	targetvec = ci::normalize(targetvec);

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
			ci::gl::rotate(0, ci::vec3(1, 0, 0));
			save_rotate_x = 0;
		}
		if (velocity.z < 0.0f) {
			ci::gl::rotate(M_PI, ci::vec3(1, 0, 0));
			save_rotate_x = M_PI;
		}
		return;
	}

	float rotation = atan2f(velocity.z, velocity.y);
	ci::app::console() << rotation << std::endl;
	//��]
	// �����
	if (rotation > 0) {
		ci::gl::rotate(rotation - M_PI/2, quataxis);
		save_rotate_x = rotation;
	}
	// �E���
	else if (rotation < 0) {
		ci::gl::rotate(rotation + M_PI / 2, quataxis);
		save_rotate_x = rotation;
	}

	//�x�N�g�����o�Ȃ����
	if (rotation <= 0.1f &&
		rotation >= -0.1f) {
		if (save_rotate_x > 0.1f)
			ci::gl::rotate(save_rotate_x - 90, ci::vec3(1, 0, 0));
		if (save_rotate_x < -0.1f)
			ci::gl::rotate(-save_rotate_x + 90, ci::vec3(1, 0, 0));
	}
}

void Game::Player::cPlayer::getGems(const int& _gemid)
{
	//�����̏������Ă���W�F���Ƀv�b�V���o�b�N
	getgems.push_back(GemManager->FindGem(_gemid));
	gem_production_end.insert(std::make_pair(_gemid,false));
	int index = getgems.size() - 1;

	
	getgems[index]->setPutPos(ci::vec3(begin_pos.x, mPos.y, begin_pos.z));

	getgems[index]->root = Node::node::create();
	getgems[index]->root->set_schedule_update();
	getgems[index]->root->set_position_3d(getgems[index]->getPos());
	getgems[index]->root->set_scale_3d(getgems[index]->getScale());

	getgems[index]->root->run_action(Node::Action::move_to::create(2,mPos));

	getgems[index]->root->run_action(Node::Action::sequence::create(
		Node::Action::scale_to::create(2, ci::vec3(0.1f)),
		Node::Action::call_func::create([this, _gemid]() {
		gem_production_end[_gemid] = true;
	})));

	//�v���C���[�̃q�b�g������Ă��炤
	//GemManager->getGems()[_gemid]->setIsDrillhit(true);
}

void Game::Player::cPlayer::collisionGems()
{
	//������AABB�𐶐�
	ci::vec3 aabb_begin_pos = mPos - ci::vec3(float(size.x) / 2.f, float(size.y) / 2.f, float(size.z) / 2.f);
	ci::vec3 aabb_end_pos = mPos + ci::vec3(float(size.x) / 2.f, float(size.y) / 2.f, float(size.z) / 2.f);
	ci::AxisAlignedBox player_aabb(aabb_begin_pos, aabb_end_pos);

	for (int i = 0; i < int(GemManager->getGems().size()); i++)
	{
		if (GemManager->getGems()[i]->getIsDrillhit())continue;
		ci::vec3 gempos = GemManager->getGems()[i]->getPos();
		ci::vec3 gemscale = GemManager->getGems()[i]->getScale();

		//�W�F����AABB�𐶐�
		ci::AxisAlignedBox gem_aabb(gempos - ci::vec3(float(gemscale.x) / 2.f, float(gemscale.y) / 2.f, float(gemscale.z) / 2.f),
			gempos + ci::vec3(float(gemscale.x) / 2.f, float(gemscale.y) / 2.f, float(gemscale.z) / 2.f));

		//�ڐG���Ă�����id��send
		if (player_aabb.intersects(gem_aabb))
		{
			//�v���C���[�p�̃p�P�b�g�����Ȃ���send��������
			getGems(GemManager->getGems()[i]->getId());
			//cClientAdapter::getInstance()->sendGetGemQuarry(player_id, GemManager->getGems()[i]->getId());
		}
	}
}

void Game::Player::cPlayer::gemsUpdate(const float& delta_time)
{

	for (auto& it : getgems) {
		it->root->entry_update(delta_time);
		ci::vec3 buf_pos = it->root->get_position_3d();
		it->setPos(buf_pos);
		//���o���I�������
		if (gem_production_end[it->getId()] == true) {
			it->setPos(mCollider.getPosition() - ( installation_position * ci::vec3(0.3f) ));
		}
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
	mCollider(mPos, ci::vec3(DEFAULT_SIZE)),
	mRigidbody(mCollider)
{
	size = ci::vec3(DEFAULT_SIZE);
	color = ci::vec4(1);
	color = ci::ColorA8u(1, 0, 1, 1);
	velocity = ci::vec3(0);
	this->team = team;
	save_rotate_y = 0;
	save_rotate_x = 0;
	drilling = false;
	jump_flag = false;
	player_vec = ci::vec3(0, 0, 2);
	//�v���C���[�̃X�e�[�^�X
	status.attack = 10;
	status.drill_range = 1.5f;
	status.jump_force = 0.6F;
	status.speed = DEFAULT_SPEED;
	status.drill_speed = DEFAULT_SPEED * 2;
	//�ݒu�ʒu
	installation_position = ci::vec3(0,0,2);

	block_type = std::make_shared<Game::Field::cBreakBlockType>(Game::Field::BlockType::NORMAL);

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

	begin_pos = ci::vec3(0);
	root = Node::node::create();
	root->set_position_3d(mPos);
	
	root->set_schedule_update();
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
		//�����̈ʒu�ƁA�����̃x�N�g���̌����ɑ΂��Č@��
		Game::cFieldManager::getInstance()->blockBreak(mCollider.getPosition() + (velocity * ci::vec3(status.drill_speed/4)), status.drill_range, *block_type);
		//Game::cFieldManager::getInstance()->blockBreak(mCollider.getPosition(), status.drill_range, *block_type);
		
		collisionGems();
	}
	root->entry_update(delta_time);
}

void Game::Player::cPlayer::draw()
{
    //ci::gl::ScopedColor col( color );
    ci::gl::ScopedTextureBind tex( TEX->get( "mogura" ) );
    //ci::gl::ScopedGlslProg glsl( ci::gl::getStockShader( ci::gl::ShaderDef( ).texture( ) ) );

	ci::gl::pushModelView();
	ci::gl::translate(mPos - ci::vec3(0, 0.5f, 0));
	playerRotationY();
	playerRotationX();
	ci::gl::scale(ci::vec3(0.01f, 0.01f, 0.012f));
	ci::gl::draw(mesh);
	ci::gl::popModelView();
}

#include <Game/Player/cPlayer.h>
#include <Collision/cAABBCollider.h>
#include <Game/cFieldManager.h>
#include <CameraManager/cCameraManager.h>
#include <Resource/cObjectManager.h>
#include <Resource/TextureManager.h>
#include <Game/cGemManager.h>
#include <Game/Field/cBreakBlockType.h>
#include <Game/Weapons/WeaponFactory.h>
#include <assert.h>
#include <Resource/cSoundManager.h>
#include <Particle/cParticleManager.h>
void Game::Player::cPlayer::playerRotationY()
{
	//�v���C���[�̑O����
	ci::vec3 rotateaxis = ci::vec3(0.0f, 0.0f, 1.0f);

	//�ړ���̃x�N�g��
	auto xzVector = normalized_player_vec * ci::vec3( 1, 0, 1 );

	ci::vec3 targetvec = ci::vec3(normalized_player_vec.x, 0, normalized_player_vec.z);

	installation_position = xzVector * 10.0F;
	ci::vec3 rotateaxis_x = normalized_player_vec * 10.0F;
	//��]��
	ci::vec3 quataxis = glm::cross(rotateaxis, targetvec);

	//�����x�N�g������������Ԃ��ƃN�H�[�^�j�I����
	//�O�ɂȂ�B
	//�ŏ��̃x�N�g���Ƃ̊O�ς�����Ă���̂ŁA�O
	//�ɂȂ�̂�Z�ƕ��s�ȃx�N�g���̎������B
	//�Ȃ̂�Z���O�̎��ƂP�̎�������O�Ƃ��Ċp�x
	//��^���Ȃ���΂Ȃ�Ȃ�
	if (quataxis == ci::vec3(0)) {
		if ( normalized_player_vec.z > 0.0f) {
			ci::gl::rotate(0, ci::vec3(0, 1, 0));
			save_rotate_y = 0;
		}
		if ( normalized_player_vec.z < 0.0f) {
			ci::gl::rotate(M_PI, ci::vec3(0, 1, 0));
			save_rotate_y = M_PI;
		}
		return;
	}

	float rotation = atan2f( normalized_player_vec.x, normalized_player_vec.z);

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
	//�v���C���[�̏����
	ci::vec3 rotateaxis = ci::vec3(0.0f, 1.0f, 0.0f);

	//�ړ���̃x�N�g��
	ci::vec3 yzVector = normalized_player_vec * ci::vec3( 0, 1, 1 );

	//��]��
	ci::vec3 quataxis = glm::cross(rotateaxis, yzVector );

	//�����x�N�g������������Ԃ��ƃN�H�[�^�j�I����
	//�O�ɂȂ�B
	//�ŏ��̃x�N�g���Ƃ̊O�ς�����Ă���̂ŁA�O
	//�ɂȂ�̂�Z�ƕ��s�ȃx�N�g���̎������B
	//�Ȃ̂�Z���O�̎��ƂP�̎�������O�Ƃ��Ċp�x
	//��^���Ȃ���΂Ȃ�Ȃ�
	if (quataxis == ci::vec3(0)) {
		if ( normalized_player_vec.z > 0.0f) {
			ci::gl::rotate(0, ci::vec3(1, 0, 0));
			save_rotate_x = 0;
		}
		if ( normalized_player_vec.z < 0.0f) {
			ci::gl::rotate(M_PI, ci::vec3(1, 0, 0));
			save_rotate_x = M_PI;
		}
		return;
	}

	float rotation = atan2f( normalized_player_vec.z, normalized_player_vec.y);

	//��]
	// �����
	if (rotation > 0) {
		ci::gl::rotate(rotation - M_PI / 2, quataxis);
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
	getgems.push_back(GemManager->getGemStone(_gemid));
	gem_production_end.insert(std::make_pair(_gemid, false));
	int index = getgems.size() - 1;

	getgems[index]->setPutPos(ci::vec3(begin_pos.x, mPos.y, begin_pos.z));

	getgems[index]->root = Node::node::create();
	getgems[index]->root->set_schedule_update();
	getgems[index]->root->set_position_3d(getgems[index]->getCenterPos());
	getgems[index]->root->set_scale_3d(getgems[index]->getScale());

	getgems[index]->root->run_action(Node::Action::move_to::create(2, mPos));

	getgems[index]->root->run_action(Node::Action::sequence::create(
		Node::Action::scale_to::create(2, ci::vec3(0.1f)),
		Node::Action::call_func::create([this, _gemid]() {
		gem_production_end[_gemid] = true;
	})));

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
		ci::vec3 gempos = GemManager->getGems()[i]->getCenterPos();
		ci::vec3 gemscale = GemManager->getGems()[i]->getScale();

		//�W�F����AABB�𐶐�
		ci::AxisAlignedBox gem_aabb(gempos - ci::vec3(float(gemscale.x) / 2.f, float(gemscale.y) / 2.f, float(gemscale.z) / 2.f),
			gempos + ci::vec3(float(gemscale.x) / 2.f, float(gemscale.y) / 2.f, float(gemscale.z) / 2.f));

		//�ڐG���Ă�����id��send
		if (player_aabb.intersects(gem_aabb))
		{
			//�v���C���[�p�̃p�P�b�g�����Ȃ���
			getGems(GemManager->getGems()[i]->getId());
			//cClientAdapter::getInstance()->sendGetGemQuarry(player_id, GemManager->getGems()[i]->getId());
			GemManager->breakeGemStone(i);
		}
	}
}

void Game::Player::cPlayer::dead()
{
	//���������񂾂�J������0.5�b�h�炷
	if (active_user) {
		CAMERA->shakeCamera(0.1f,0.5f);
	}
	mRigidbody.setSpeed(ci::vec3(0));
	Particle::cParticleManager::getInstance()->create(Particle::ParticleParam().position(mPos)
		.scale(0.5f).
		vanishTime(1.0f).
		speed(0.3f).
		textureType(Particle::ParticleTextureType::SPARK).
		color(ci::ColorA::white()).
		moveType(Particle::ParticleType::EXPROTION).count(30).isTrajectory(false));
	Resource::cSoundManager::getInstance()->findSe("Player/launcher1.wav").play();
	Resource::cSoundManager::getInstance()->findSe("Player/rare.wav").play();
}

void Game::Player::cPlayer::respawn(const float & delta_time)
{
	if (!is_dead)return;
	respawn_count += delta_time;
	//���X�|�[���^�C�������ɒB������
	if (respawn_count < status.respawn_time) return;
	//�v���C���[��������
	resetPlayerStatus();
	
}

void Game::Player::cPlayer::resetPlayerStatus()
{
	status.hp = 100;
	is_dead = false;
}

void Game::Player::cPlayer::drillingCamera(const float& delta_time)
{
	
	if (!drilling) {
		player_far += delta_time * 15;
	}
	else if (drilling) {
		player_far -= delta_time * 15;
	}
	if (player_far > 5) player_far = 5;
	if (player_far < 0) player_far = 0;
	
	//FPS�ɂ���K�v�������̂ŃR�����g�A�E�g
	//CAMERA->setCameraFar(player_far);
}



void Game::Player::cPlayer::drill(const float& delta_time)
{
	drill_sound += delta_time;
	if (!active_user) return;
	drillingCamera(delta_time);
	if (!drilling)return;
	if (drill_sound > 0.1f) {
		Resource::cSoundManager::getInstance()->findSe("Player/drill.wav").play();
		drill_sound = 0;
	}
	//�����̈ʒu�ƁA�����̃x�N�g���̌����ɑ΂��Č@��
	Game::cFieldManager::getInstance()->blockBreak(mCollider.getPosition() + ( normalized_player_vec * ci::vec3(status.drill_speed / 4)), status.drill_range, *block_type);
	//Game::cFieldManager::getInstance()->blockBreak(mCollider.getPosition(), status.drill_range, *block_type);

	collisionGems();
}

void Game::Player::cPlayer::gemsUpdate(const float& delta_time)
{

	for (auto& it : getgems) {
		it->root->entry_update(delta_time);
		ci::vec3 buf_pos = it->root->get_position_3d();
		it->setPos(buf_pos);
		//���o���I�������
		if (gem_production_end[it->getId()] == true) {
			it->setPos(mCollider.getPosition() - (installation_position * ci::vec3(0.3f)));
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
	const int& main_weapon_id,
	const int& sub_weapon_id,
	const Game::Player::Team& team)
	: cObjectBase(pos),
	mCollider(mPos, ci::vec3(DEFAULT_SIZE)),
	mRigidbody(mCollider),team(team),player_id(id),damaged_id(id),
	active_user(is_active_user)
{
	size = ci::vec3(DEFAULT_SIZE);
	color = ci::vec4(1);
	color = ci::ColorA8u(1, 0, 1, 1);
	save_rotate_y = 0;
	save_rotate_x = 0;
	drill_sound = 0;
	drilling = false;
	jump_flag = false;
	//�v���C���[�̃X�e�[�^�X
	status.attack = 10;
	status.hp = 100;
	status.drill_range = 1;
	status.jump_force = 10.0F;
	status.speed = DEFAULT_SPEED;
	status.drill_speed = DEFAULT_SPEED;
	status.respawn_time = 7;
	//����̏�����
	main_weapon = Weapon::cWeaponFactory::getInstance()->InstanceMainWeapon(static_cast<Weapon::MAIN_WEAPON>(main_weapon_id));
	assert(main_weapon != NULL && "���C������̎�ނ�enum�������������Ă��܂���B");

	//�ݒu�ʒu
	installation_position = ci::vec3(0, 0, 2);
	player_far = 5;

	block_type = std::make_shared<Game::Field::cBreakBlockType>(Game::Field::BlockType::NORMAL);

	is_dead = false;
}


void Game::Player::cPlayer::receiveDamage(const float & attack, const float& player_id)
{
	//���S���̓_���[�W���󂯂Ȃ�
	if (is_dead)return;
	//hp�����炷
	status.hp -= attack;
	//�U�����Ă����v���C���[��id��؂�ւ���
	damaged_id = player_id;

	if (status.hp <= 0) {
		is_dead = true;
		respawn_count = 0;
		dead();
	}

	Particle::cParticleManager::getInstance()->create(Particle::ParticleParam().position(mPos)
		.scale(0.3f).
		vanishTime(0.2f).
		speed(1.0f).
		textureType(Particle::ParticleTextureType::SPARK).
		color(ci::ColorA::white()).
		moveType(Particle::ParticleType::EXPROTION).count(6).isTrajectory(false));
	Resource::cSoundManager::getInstance()->findSe("Player/damage6.wav").play();
}

void Game::Player::cPlayer::weaponUpdae(const float & delta_time)
{
	if(active_user)
	main_weapon->update(delta_time);
}

void Game::Player::cPlayer::move(const ci::vec3 & velocity)
{
	//���S���������Ȃ�
	if (is_dead)return;
	//�X�^�����͓����Ȃ�
	if (stan) {
		mRigidbody.setSpeed(ci::vec3(0));
		return;
	}
	//�v���C���[�̈ړ��x�N�g���ۑ�
	//�x�N�g���X�V
	if ( velocity.x >= 0.01f ||
		 velocity.x <= -0.01f ||
		 velocity.z >= 0.01f ||
		 velocity.z <= -0.01f ) {
		normalized_player_vec = glm::normalize( velocity );
	}
	if (active_user) {
		//�n�ʂ̒��Ō@�풆�Ȃ�d�͂��Ȃ���
		if (mCollider.getPosition().y <= 16.0f && drilling) {
			mRigidbody.gravityOff();
		}
		else {
			mRigidbody.gravityOn();
		}
	}

	auto vec = velocity;
	vec.y = drilling ? velocity.y : mRigidbody.getSpeed( ).y;
	mRigidbody.setSpeed( vec );
}

void Game::Player::cPlayer::jump(bool flag)
{
	if (jump_flag == false)
		jump_flag = flag;

	if (jump_flag == true) {
		if (mRigidbody.isLanding()) {
			Resource::cSoundManager::getInstance()->findSe("Player/onground.wav").play();
			mRigidbody.addSpeed( ci::vec3( 0, status.jump_force, 0 ) );
			jump_flag = false;
		}
	}
}

void Game::Player::cPlayer::setup()
{
	//���߂ɂ������������C�g�{����ǉ����܂�
	useSubWeapon.addSubWeapon(Game::Weapons::SubWeapon::LIGHT_BOMB);

	mCollider.setLayer(1 << 0);
	mCollider.addWorld();
	mRigidbody.addWorld();

	//�ŏ��Ɋp�x��ݒ肷�邽�߂ɂق�̏���������
	move(ci::vec3(0, 0, 0.01f));
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

	main_weapon->setup();

	mesh = Resource::cObjectManager::getInstance()->findObject("montamogura/moguraHontai.obj");
	TEX->set("mogura", "OBJ/montamogura/moguraHontai.png");
	
}

#include <Game/cClientAdapter.h>
void Game::Player::cPlayer::update(const float & delta_time)
{
	drill(delta_time);
	root->entry_update(delta_time);
	aabb = mCollider.createAABB(mCollider.getPosition());
	respawn(delta_time);
}

void Game::Player::cPlayer::draw()
{
	//���S���͕`�悵�Ȃ�
	if (is_dead)return;
	ci::gl::ScopedTextureBind tex(TEX->get("mogura"));

	ci::gl::pushModelView();
	main_weapon->draw();
	ci::gl::translate(mPos); 
	playerRotationY();
	playerRotationX();
	ci::gl::translate(-ci::vec3(0, 0.5f, 0));
	ci::gl::scale(ci::vec3(0.01f, 0.01f, 0.012f));
	ci::gl::draw(mesh);
	ci::gl::popModelView();
}

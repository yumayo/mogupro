#include <Game/Player/cPlayer.h>
#include <Collision/cAABBCollider.h>
#include <Game/cFieldManager.h>
#include <CameraManager/cCameraManager.h>
#include <Resource/cObjectManager.h>
#include <Resource/TextureManager.h>
#include <Game/cGemManager.h>
#include <Game/Field/cBreakBlockType.h>
#include <Game/Weapons/MainWeapon/cFactory.h>
#include <assert.h>
#include <Resource/cSoundManager.h>
#include <Particle/cParticleManager.h>
#include <random>
#include <Game/cPlayerManager.h>
#include <Game/cLightManager.h>
#include <Math/Quat.h>
#include <Math/float4x4.h>
#include <Game/cGameManager.h>
#include <Resource/cImageManager.h>
#include <Sound/Wav.h>
#include <Sound/Stereophonic.h>
#include <Game/cShaderManager.h>
void Game::Player::cPlayer::updatePlayerRotation()
{
	if (!active_user) return;

	ci::vec3 playerYAxis;
	ci::vec3 playerZAxis;
	ci::vec3 playerXAxis;
	if (drilling)
	{
		playerYAxis = glm::normalize(CAMERA->getCamera().getWorldUp());
		playerZAxis = CAMERA->getCameraLook();
		playerXAxis = glm::normalize(glm::cross(playerYAxis, playerZAxis));
	}
	else
	{
		playerYAxis = glm::normalize(CAMERA->getCamera().getWorldUp());
		playerZAxis = normalized_player_vec;
		playerZAxis.y = 0.0F;
		playerXAxis = glm::normalize(glm::cross(playerYAxis, playerZAxis));
		playerXAxis.y = 0.0F;
	}

	playerYAxis = glm::normalize(glm::cross(playerXAxis, playerZAxis));

	math::float3 x = { playerXAxis.x, playerXAxis.y, playerXAxis.z };
	math::float3 y = { playerYAxis.x, playerYAxis.y, playerYAxis.z };
	math::float3 z = { playerZAxis.x, playerZAxis.y, playerZAxis.z };

	auto quat = math::Quat::LookAt(math::float3(0, 0, 1), z, math::float3(0, 1, 0), y * -1);

	auto q = ci::quat(quat.w, quat.x, quat.y, quat.z);

	rotation = q;
}

cinder::mat4 Game::Player::cPlayer::getWorldMatrixWeapon( ) const
{
	auto m = ci::mat4( );
	m *= glm::translate( mPos );
	m *= glm::toMat4( rotation );
	m *= glm::translate( glm::vec3( -0.2F, 0.2F, 0.2F ) );
	return m;
}

void Game::Player::cPlayer::getGems(const int& _gemid)
{
	//�����̏������Ă���W�F���Ƀv�b�V���o�b�N
	getgems.push_back(GemManager->getFragmentGem(_gemid));
	GemManager->getFragmentGem(_gemid)->setIsActive(false);

	//�����_���̐���
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> random_x(-1, 1);
	std::uniform_real_distribution<float> random_y(-1, 1);
	std::uniform_real_distribution<float> random_z(-1, 1);
	float buf_x = random_x(mt);
	float buf_y = random_y(mt);
	float buf_z = random_z(mt);

	//���d�͏��
	GemManager->getFragmentGem(_gemid)->setIsRigid(false);
	gem_production_end.insert(std::make_pair(_gemid, false));
	int index = getgems.size() - 1;

	getgems[index]->setPutPos(ci::vec3(begin_pos.x, mPos.y, begin_pos.z));

	getgems[index]->node = Node::node::create();
	getgems[index]->node->set_schedule_update();
	getgems[index]->node->set_position_3d(getgems[index]->getPos());
	getgems[index]->node->set_scale_3d(getgems[index]->getScale());

	getgems[index]->node->run_action(Node::Action::sequence::create(
		Node::Action::ease<ci::EaseOutCirc>::create(
			Node::Action::move_to::create(0.5f, getgems[index]->getPos() + ci::vec3(buf_x, buf_y, buf_z))),
		Node::Action::ease<ci::EaseInCirc>::create(
			Node::Action::move_to_target::create(0.5f, root))));

	getgems[index]->node->run_action(Node::Action::sequence::create(
		Node::Action::scale_to::create(1, ci::vec3(0.1f)),
		Node::Action::call_func::create([this, _gemid]() {
		gem_production_end[_gemid] = true;
		Sound::StereophonicManager::getInstance()->add("gem1" + std::to_string(player_id), ci::app::getAssetPath("SE/Player/gem.wav").string(), mPos);
	})));
	
	gem_id_buf = _gemid;
}

void Game::Player::cPlayer::collisionGems()
{
	if ( isWatching( ) ) return;

	//������AABB�𐶐�
	ci::vec3 aabb_begin_pos = mPos - size * ci::vec3(5);
	ci::vec3 aabb_end_pos = mPos + size * ci::vec3(5);

	ci::AxisAlignedBox player_aabb(aabb_begin_pos, aabb_end_pos);
	//ci::AxisAlignedBox player_aabb(aabb_begin_pos, aabb_end_pos);
	for (int i = 0; i < GemManager->getFragmentGems().size(); i++)
	{
		//�W�F��������Ă�����Ԃ�
		if (!GemManager->getFragmentGems()[i]->isActive())continue;

		//�W�F����AABB�𐶐�
		ci::AxisAlignedBox gem_aabb = GemManager->getFragmentGems()[i]->getAabb().createAABB(GemManager->getFragmentGems()[i]->getPos());
		
		//�v���C���[�̋߂��̃W�F���ȊO�͕Ԃ�
		if (!gem_aabb.intersects(player_aabb)) continue;
		
		//�ڐG���Ă�����id��send
		if (player_aabb.intersects(gem_aabb))
		{
			//�v���C���[�p�̃p�P�b�g�����Ȃ���
			getGems(GemManager->getFragmentGems()[i]->getId());
			//cClientAdapter::getInstance()->sendGetGemQuarry(player_id, GemManager->getGems()[i]->getId());
			//GemManager->breakGemStone(i);
		}
	}
}

void Game::Player::cPlayer::deadGems()
{
	for (auto player : cPlayerManager::getInstance()->getPlayers()) {
		if (player->getPlayerId() == damaged_id) {
			for (auto& it : getgems) {
				gem_production_end[it->getId()] = false;
				it->setIsActive(true);
				player->getGems(it->getId());
			}
			//���������甲����
			break;
		}
	}
	
	getgems.clear();
}

void Game::Player::cPlayer::dead()
{
	//���������񂾂�J������0.5�b�h�炷
	if (active_user) {
		CAMERA->shakeCamera(0.1f,0.5f);
	}
	deadGems();
	drilling = false;
	main_weapon->reset();
	mRigidbody.setSpeed(ci::vec3(0));
	mCollider.setActive(false);
	Particle::cParticleManager::getInstance()->create(Particle::ParticleParam().position(mPos)
		.scale(0.5f).
		vanishTime(1.0f).
		speed(0.3f).
		textureType(Particle::ParticleTextureType::SPARK).
		color(ci::ColorA::white()).
		moveType(Particle::ParticleType::EXPROTION).count(30).isTrajectory(false));
	Sound::StereophonicManager::getInstance()->add("launcher1" + std::to_string(player_id), ci::app::getAssetPath("SE/Player/launcher1.wav").string(), mPos);
	Sound::StereophonicManager::getInstance()->add("rare" + std::to_string(player_id), ci::app::getAssetPath("SE/Player/rare.wav").string(), mPos);

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
	//�ʒu�����X�|�[���ʒu��
	mCollider.setPosition(start_position);
	mCollider.setActive(true);
	is_dead = false;
	no_damage_count = 0;
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
	if ( isWatching( ) )
		;
	else
	{
		mRigidbody.gravityOn( );
	}
	drillingCamera(delta_time);
	animation.setAnimationIncrementTime(0.016);
	//�@���ĂȂ���Ώ������Ȃ�
	if (!drilling)return;
	animation.setAnimationIncrementTime(0.1);
	if (Game::Field::WORLD_SIZE.y > mCollider.getPosition().y) {
		if ( isWatching( ) )
			;
		else
		{
			mRigidbody.gravityOff( );
		}
	}
	if (drill_sound > 0.2f) {
		if(Game::cFieldManager::getInstance()->isBreakBlock(mCollider.getPosition() + (glm::normalize(CAMERA->getCamera().getViewDirection()) * ci::vec3(status.drill_speed / 3)),1)) {
			//Resource::cSoundManager::getInstance()->findSe("Player/drill.wav").setGain(0.2f);
			//Resource::cSoundManager::getInstance()->findSe("Player/drill.wav").play();
            Game::cFieldManager::getInstance()->playBreakBlockSounds( mCollider.getPosition() ,"player" + std::to_string( player_id ));
		}
		drill_sound = 0;
	}
	//�����̈ʒu�ƁA�����̃x�N�g���̌����ɑ΂��Č@��
	Game::cFieldManager::getInstance()->blockBreak(mCollider.getPosition(), status.drill_range, *block_type);
	Game::cFieldManager::getInstance()->blockBreak(mCollider.getPosition() + normalized_player_vec, status.drill_range, *block_type);
	//Game::cFieldManager::getInstance()->blockBreak(mCollider.getPosition(), status.drill_range, *block_type);

}

Game::Weapons::MainWeapon::cBase * Game::Player::cPlayer::getMainWeapon( )
{
	return main_weapon.get( );
}

void Game::Player::cPlayer::gemsUpdate(const float& delta_time)
{
	for (auto& it : getgems) {
		it->node->entry_update(delta_time);
		ci::vec3 buf_pos = it->node->get_position_3d();
		it->setPos(buf_pos);
		//���o���I�������
		if (gem_production_end[it->getId()] == true) {
			if (getgems.size() > 1) {
				GemManager->getFragmentGem(gem_id_buf)->setVisible(false);
			}
			it->setPos(mCollider.getPosition() - (normalized_player_vec * ci::vec3(0.3f)) + ci::vec3(0,0.1f,0));
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
	: cObjectBase(pos), start_position(pos),
	mCollider(mPos, DEFAULT_SIZE ),
	mRigidbody(mCollider),team(team),player_id(id),damaged_id(id),
	active_user(is_active_user)
{
	size = DEFAULT_SIZE;
	color = ci::vec4(1);
	color = ci::ColorA8u(1, 0, 1, 1);
	drill_sound = 0;
	drilling = false;
	jump_flag = false;
	stan = false;
	//�v���C���[�̃X�e�[�^�X
	status.attack = 10;
	status.hp = 100;
	status.drill_range = 1;
	status.jump_force = 10.0F;
	status.speed = DEFAULT_SPEED;
	status.drill_speed = DEFAULT_SPEED*1.2f;
	status.respawn_time = DEFAULT_RESPAWN_TIME;
	//����̏�����
	//! main_weapon_id�͖������܂��B
	main_weapon = Weapons::MainWeapon::cFactory::create( Weapons::MainWeapon::LIGHT_SABER, *this );
	assert(main_weapon != NULL && "���C������̎�ނ�enum�������������Ă��܂���B");

	//�ݒu�ʒu
	installation_position = ci::vec3(0, 0, 2);
	player_far = 5;

	block_type = std::make_shared<Game::Field::cBreakBlockType>(Game::Field::BlockType::NORMAL);

	is_dead = false;

	cinder::vec3 lightColor;
	switch ( team )
	{
	case Player::Blue:
		lightColor = ci::vec3( 0, 0, 1 - ( status.hp / Player::MAX_HP ) / 2 );
		rotation = cinder::quat(glm::angleAxis(glm::pi<float>(), ci::vec3(0, 1, 0)));
		break;
	case Player::Red:
		lightColor = ci::vec3( 1 - ( status.hp / Player::MAX_HP ) / 2, 0, 0 );
		rotation = cinder::quat(glm::angleAxis(0.0F, ci::vec3(0, 1, 0)));
		break;
	default:
		break;
	}
	light = cLightManager::getInstance( )->addPointLight( mPos, lightColor, isWatching( ) ? 0.0F : 1.0F );
	auto pvec = glm::normalize( getPlayerVec( ) );
	spotlight = cLightManager::getInstance( )->addSpotLight( mPos + pvec * 0.2F, pvec * 3.0F, lightColor, isWatching() ? 0.0F : 2.0F );

	// /////////���C�g�̃f�[�^���l�ߍ��ށB
	pointLightIds.emplace_back(light->getId());
	// �@�Ƀ��C�g���ڂ�̂ŃX�|�b�g���C�g�͎g�p���Ȃ��B
	// spotLightIds.emplace_back(spotlight->getId());
}


void Game::Player::cPlayer::receiveDamage(const float & attack, float player_id)
{
	if (no_damage_count < DEFAULT_NO_DAMAGE_TIME) return;

	//���S���̓_���[�W���󂯂Ȃ�
	if (is_dead)return;
	
	CAMERA->shakeCamera(0.1f, 0.2f);

	//hp�����炷
	status.hp -= attack;
	//�U�����Ă����v���C���[��id��؂�ւ���
	damaged_id = player_id;

	if (status.hp <= 0) {
		is_dead = true;
		respawn_count = 0;
		cGameManager::getInstance()->kill(player_id);
		cGameManager::getInstance()->death(this->player_id);
		dead();
	}

	Particle::cParticleManager::getInstance()->create(Particle::ParticleParam().position(mPos)
		.scale(0.3f).
		vanishTime(0.2f).
		speed(1.0f).
		textureType(Particle::ParticleTextureType::SPARK).
		color(ci::ColorA::white()).
		moveType(Particle::ParticleType::EXPROTION).count(6).isTrajectory(false));
	Sound::StereophonicManager::getInstance()->add("damage6" + std::to_string(this->player_id), ci::app::getAssetPath("SE/Player/damage6.wav").string(), mPos);
}

void Game::Player::cPlayer::weaponUpdae(const float & delta_time)
{
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
	auto vec = velocity;
	vec.y = drilling || isWatching( ) ? velocity.y : mRigidbody.getSpeed( ).y;
	mRigidbody.setSpeed( vec );
}

void Game::Player::cPlayer::jump(bool flag)
{
	if (jump_flag == false)
		jump_flag = flag;

	if (jump_flag == true) {
		if (mRigidbody.isLanding()) {
			Sound::StereophonicManager::getInstance()->add("onground" + std::to_string(player_id), ci::app::getAssetPath("SE/Player/onground.wav").string(), mPos);
			mRigidbody.addSpeed( ci::vec3( 0, status.jump_force, 0 ) );
			jump_flag = false;
		}
	}
}

void Game::Player::cPlayer::setup()
{
	//���߂ɂ������������C�g�{����ǉ����܂�
	//useSubWeapon.addSubWeapon(Game::Weapons::SubWeapon::LIGHT_BOMB);

	mCollider.setLayer( isWatching( ) ? 1 << 1 : 1 << 0 );
	mCollider.addWorld( );
	if ( isWatching( ) )
	{
		mRigidbody.gravityOff( );
	}
	else
	{
		mRigidbody.gravityOn( );
	}
	mRigidbody.addWorld( );
	if (team == Team::Red) {
		//�ŏ��Ɋp�x��ݒ肷�邽�߂ɂق�̏���������
		move(ci::vec3(0, 0, 0.01f));
		mCollider.setPosition(mPos + ci::vec3(0, 0, 0.01f));
	}
	else {
		//�ŏ��Ɋp�x��ݒ肷�邽�߂ɂق�̏���������
		move(ci::vec3(0, 0, -0.01f));
		mCollider.setPosition(mPos + ci::vec3(0, 0, -0.01f));
	}
	
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

	animation.create("mogura_walk", false, true);
	animation.create("mogura_attack",true);
	animation.animationChange("mogura_walk");
	animation.setAnimationStopTime("mogura_attack",0.5);
	mesh = Resource::cObjectManager::getInstance()->findObject("montamogura/moguraHontai.obj");
	
	switch ( team )
	{
	case Game::Player::Red:
		texture = Resource::IMAGE ["in_game/UV_mogura_red.jpg" ];
		break;
	case Game::Player::Blue:
		texture = Resource::IMAGE["in_game/UV_mogura_blue.jpg"];
		break;
	default:
		assert( !"�`�[�����s���ł��B" );
		break;
	}
}

#include <Game/cClientAdapter.h>
void Game::Player::cPlayer::update(const float & delta_time)
{
	animation.update( delta_time );
	no_damage_blind++;
	no_damage_count += delta_time;
	drill(delta_time);
	root->entry_update(delta_time);
	aabb = mCollider.createAABB(mCollider.getPosition());
	respawn(delta_time);
	root->set_position_3d(mPos);
//	collisionGems(); �܂Ŕj�󂵂��u�Ԃɂ��ׂĔ��肵�Ă���̂ŕK�v�Ȃ��Ȃ�܂����B
	updatePlayerRotation();
}

void Game::Player::cPlayer::cameraAfterUpdate( const float & delta_time )
{
	// �v���C���[�͏�ɉ�ʂ̒��S�ɂ��邽�߃J�����̃A�b�v�f�[�g������ɍX�V���܂��B
	cinder::vec3 lightColor;
	switch ( team )
	{
	case Player::Blue:
		lightColor = ci::vec3( 0, 0, 1 - ( status.hp / Player::MAX_HP ) / 2 );
		break;
	case Player::Red:
		lightColor = ci::vec3( 1 - ( status.hp / Player::MAX_HP ) / 2, 0, 0 );
		break;
	default:
		break;
	}
	light->color = lightColor;
	light->reAttachPositionWithRadius( mPos, isWatching( ) ? 0.0F : 1 + 2 - ( status.hp / Player::MAX_HP ) * 2 );
	auto pvec = glm::normalize( getPlayerVec( ) );
	spotlight->reAttachPositionWithDirection( mPos + pvec * 0.1F, pvec * 3.0F );
}

void Game::Player::cPlayer::draw()
{
	// �ϐ�҂ł��B 2017/12/14
	if ( isWatching( ) )
	{
		return;
	}

	//���S���͕`�悵�Ȃ�
	if (is_dead)return;

	ci::gl::pushModelView();
	if (active_user&&
		(CAMERA->getCameraMode() != CameraManager::CAMERA_MODE::TPS)) {
		return;
	}

	//�ďo���Ƃ��ɓ_�ł����鏈��
	if (no_damage_count < DEFAULT_NO_DAMAGE_TIME) {
		if (no_damage_blind % 2 == 0) {
			return;
		}
	}

	// ////////////�܂Ƃ߂Ă�����ID���g���ă��C�g�𔽉f������B
	cShaderManager::getInstance()->uniformUpdate(pointLightIds, lineLightIds, spotLightIds);

	// ����̕`��
	{
		ci::gl::pushModelView( );
		ci::gl::setModelMatrix( getWorldMatrixWeapon( ) );
		main_weapon->draw( );
		ci::gl::popModelView( );
	}

	// �v���C���[���̂̕`��
	{
		ci::gl::ScopedTextureBind tex( texture );
		ci::gl::translate( mPos );
		ci::gl::rotate( rotation );
		ci::gl::scale( ci::vec3( 0.25f, 0.25f, 0.25f ) );
		animation.draw( );
	}

	ci::gl::popModelView();
}

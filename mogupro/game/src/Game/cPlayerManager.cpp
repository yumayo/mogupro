#include <Game/cPlayerManager.h>
#include <Utility/cInput.h>
#include <CameraManager/cCameraManager.h>
#include <Game/cStrategyManager.h>
#include <Game/Strategy/cDrill.h>
#include <Game/cFieldManager.h>
#include <Game/cClientAdapter.h>
#include <Game/cGemManager.h>
#include <Node/renderer.hpp>
#include <Game/cGameManager.h>
#include <Resource/cImageManager.h>
void Game::cPlayerManager::playerInstance(std::vector<ci::vec3> positions, const int& player_number, const int& active_player_id, std::vector<int> teams)
{
	//����
	for (int i = 0; i < player_number; i++) {
		//�ʐM�ő��
		if (i == active_player_id) {
			players.push_back(std::make_shared<Player::cPlayer>(positions[i], i, true, 0, 0, static_cast<Game::Player::Team>(teams[i])));
			//�A�N�e�B�u���[�U�ɑ��
			active_player = players[i];
			this->active_player_id = active_player_id;
			this->active_player_team_id = teams[i];
		}
		else {
			players.push_back(std::make_shared<Player::cPlayer>(positions[i], i, false, 0, 0, static_cast<Game::Player::Team>(teams[i])));
		}
	}
}

void Game::cPlayerManager::playerDrillMove(const float & delta_time)
{
	//CAMERA->shakeCamera(0.1f, 0.1f);
	//�J�����̕����Ɉړ�
	active_player->move(CAMERA->getCameraLook() * active_player->getDrillSpeed());
}

void Game::cPlayerManager::playerAttack(const float & delta_time)
{
	if ( active_player->isWatching( ) ) return; // yumayo
	if (active_player->isDead())return;

	//���C���U�� R2 or �E�N���b�N
	if (ENV->pushKey(ci::app::MouseEvent::RIGHT_DOWN) || ENV->isPadPush(ENV->BUTTON_8))
	{
		cClientAdapter::getInstance()->sendPlayerAttack(active_player_id, 1);
	}
	if (ENV->pullKey(ci::app::MouseEvent::RIGHT_DOWN) || ENV->isPadPull(ENV->BUTTON_8))
	{
		cClientAdapter::getInstance()->sendPlayerAttack(active_player_id, 2);
	}
}

ci::vec3 Game::cPlayerManager::playerNormalMoveKey(const float& delta_time)
{
	//�v���C���[�̑��x
	float player_speed = active_player->getSpeed() * delta_time;

	ci::vec3 keybord_velocity = ci::vec3(0);

	float x_axis = 0;
	float z_axis = 0;
	//�΂ߗp�X�^�b�N
	int diagonal = 0;

	//���Z�b�g
	if (ENV->pressKey(ci::app::KeyEvent::KEY_r)) {
		active_player->resetPos();
	}

	if (ENV->pressKey(ci::app::KeyEvent::KEY_w)) {
		z_axis = active_player->getSpeed();
		diagonal++;
	}

	if (ENV->pressKey(ci::app::KeyEvent::KEY_s)) {
		z_axis = -active_player->getSpeed();
		diagonal++;
	}

	if (ENV->pressKey(ci::app::KeyEvent::KEY_d)) {
		x_axis = -active_player->getSpeed();
		diagonal++;
	}

	if (ENV->pressKey(ci::app::KeyEvent::KEY_a)) {
		x_axis = active_player->getSpeed();
		diagonal++;
	}

	if ( active_player->isWatching( ) )
	{
		if ( ENV->pressKey( ci::app::KeyEvent::KEY_LSHIFT ) ) {
			keybord_velocity.y = -active_player->getSpeed( );
		}

		if ( ENV->pressKey( ci::app::KeyEvent::KEY_SPACE ) ) {
			keybord_velocity.y = active_player->getSpeed( );
		}
	}

	keybord_velocity += ci::vec3(z_axis*sin(CAMERA->getCameraAngle().x), 0.0f, z_axis*cos(CAMERA->getCameraAngle().x));
	keybord_velocity += ci::vec3(x_axis*cos(CAMERA->getCameraAngle().x), 0.0f, -x_axis*sin(CAMERA->getCameraAngle().x));

	if (diagonal >= 2) {
		std::sqrtf(keybord_velocity.x);
		std::sqrtf(keybord_velocity.z);
	}

	if ( active_player->isWatching( ) )
		;
	else
	{
		//�W�����v��Move�̌�ɌĂ�
		if ( ENV->pushKey( ci::app::KeyEvent::KEY_SPACE ) ) {
			active_player->jump( true );
		}
	}

	return keybord_velocity;
}
ci::vec3 Game::cPlayerManager::playerNormalMovePad(const float & delta_time)
{
	ci::vec3 pad_velocity = ci::vec3(0);

	float x_axis = -50 * ENV->getPadAxis(0) * delta_time * active_player->getSpeed();
	float z_axis = -50 * ENV->getPadAxis(1) * delta_time * active_player->getSpeed();

	pad_velocity += ci::vec3(z_axis*sin(CAMERA->getCameraAngle().x), 0.0f, z_axis*cos(CAMERA->getCameraAngle().x));
	pad_velocity += ci::vec3(x_axis*cos(CAMERA->getCameraAngle().x), 0.0f, -x_axis*sin(CAMERA->getCameraAngle().x));
	return pad_velocity;
}
void Game::cPlayerManager::playerMove(const float & delta_time)
{

	//�J�����̃}�E�X����ON�@OFF
	if (ENV->pushKey(ci::app::KeyEvent::KEY_ESCAPE)) {
		ENV->setMouseControl(mouse_on);
		if (mouse_on == true) {
			mouse_on = false;
		}
		else {
			mouse_on = true;
		}
	}
	if (!active_player->isDead()) {
		CAMERA->addCameraAngle(ci::vec2(ENV->getPadAxis(2)*(-0.05f), ENV->getPadAxis(3)*(-0.05f)));
	}
	// �^�[�Q�b�g��������v���C���[�͓����܂���B
	if ( watching_target_player_id != -1 ) return;
	ENV->setMouseCursorAvtive(!active_player->isDead());
	//�v���C���[������ł�����J�����ȊO����s�\
	if (active_player->isDead())return;

	CAMERA->addCameraAngle(ci::vec2(ENV->getPadAxis(2)*(-0.07f), ENV->getPadAxis(3)*(-0.07f)));
	
	//��C�ɃW�F��������
	auto cannon = cStrategyManager::getInstance()->getCannons()[static_cast<Player::Team>(active_player->getWhichTeam())];
	if (cannon->getAABB().intersects(active_player->getAABB())) {
		cannon->receivePlayerGem(active_player->getgems.size(), active_player_id);
		cGemManager::getInstance()->deleteFragmentGems(active_player->getgems);
		active_player->gem_production_end.clear();
		active_player->getgems.clear();
	}


	keyMove(delta_time);

	//�p�b�h�̎��ɍU�����㏑������Ȃ��悤�ɒ���
	padMove(delta_time);

	//�@�풆�͓��������ς��
	if (active_player->isDrilling()) {
		playerDrillMove(delta_time);
	}
	else {
		ci::vec3 buf_axis = ci::vec3(0);
		buf_axis += playerNormalMoveKey(delta_time);
		buf_axis += playerNormalMovePad(delta_time);
		active_player->move(buf_axis);
	}

}
void Game::cPlayerManager::padMove(const float & delta_time)
{
	if ( active_player->isWatching( ) )
		;
	else
	{
		//�@�� L1
		if ( ENV->isPadPress( ENV->BUTTON_5 ) &&
			 Game::Field::WORLD_SIZE.y + 1 > active_player->getPos( ).y ) {
			active_player->Drilling( true );
		}

		//�T�u���� R1
		if ( ENV->isPadPush( ENV->BUTTON_7 ) ) {
			active_player->useSubWeapon.useWeapon( active_player_id );
		}

		//�W�����v X
		if ( ENV->isPadPush( ENV->BUTTON_1 ) ) {
			active_player->jump( true );
		}
	}

	//�_�b�V�� ��
	if (ENV->isPadPull(ENV->BUTTON_3)) {
		active_player->setDefaultSpeed();
	}
	if (ENV->isPadPress(ENV->BUTTON_3)) {
		active_player->setSpeed(10.0f);
	}
	

}
void Game::cPlayerManager::keyMove(const float & delta_time)
{
	if ( active_player->isWatching( ) )
		;
	else
	{
		//�@�풆��true 
		if ( ENV->pressKey( ci::app::MouseEvent::LEFT_DOWN ) &&
			 Game::Field::WORLD_SIZE.y + 1 > active_player->getPos( ).y ) {
			active_player->Drilling( true );
		}
		else {
			active_player->Drilling( false );
		}
		if ( ENV->pullKey( ci::app::MouseEvent::LEFT_DOWN ) ) {
			active_player->Drilling( false );
		}
	}

	//�_�b�V��
	if ( ENV->pullKey( ci::app::KeyEvent::KEY_LCTRL ) ) {
		active_player->setDefaultSpeed( );
	}
	if ( ENV->pushKey( ci::app::KeyEvent::KEY_LCTRL ) ) {
		active_player->setSpeed( 10.0f );
	}

	//G-BACK �ӂ������������͂͂͂͂͂�
	if (ENV->pressKey(ci::app::KeyEvent::KEY_l)) {
		active_player->receiveDamage(10.0f, 5);
	}
	
	if (!active_player->isDead()) {
		if (ENV->pressKey(ci::app::KeyEvent::KEY_UP)) {
			CAMERA->addCameraAngle(ci::vec2(0, 0.05f));
		}
		if (ENV->pressKey(ci::app::KeyEvent::KEY_DOWN))
			CAMERA->addCameraAngle(ci::vec2(0, -0.05f));

		if (ENV->pressKey(ci::app::KeyEvent::KEY_RIGHT))
			CAMERA->addCameraAngle(ci::vec2(-0.05f, 0));

		if (ENV->pressKey(ci::app::KeyEvent::KEY_LEFT))
			CAMERA->addCameraAngle(ci::vec2(0.05f, 0));
	}
	
	if ( active_player->isWatching( ) )
		;
	else
	{
		///////////////////�f�o�b�N�Ń��C�g�{���𑝂₷
		if ( ENV->pushKey( ci::app::KeyEvent::KEY_h ) ) {
			active_player->useSubWeapon.addSubWeapon( Game::Weapons::SubWeapon::LIGHT_BOMB );
		}
		if ( ENV->pushKey( ci::app::KeyEvent::KEY_j ) ) {
			active_player->useSubWeapon.addSubWeapon( Game::Weapons::SubWeapon::QUARRY );
		}
		/////////////////�A�C�e�����g��
		if ( ENV->pushKey( ci::app::KeyEvent::KEY_g ) ) {
			active_player->useSubWeapon.useWeapon( active_player_id );
		}
		///////////////////
	}
}
void Game::cPlayerManager::killCamera(const float & delta_time)
{
	if (!active_player->isDead())return;
	if (active_player->getRespawnCount() > 1.5f) {
		for (auto& it : players) {
			if (active_player->getDamagedId() == it->getPlayerId()) {

				CAMERA->refPosition = it->getPos() + ci::vec3(0, 0, 0);
			}
		}
	}
}
void Game::cPlayerManager::watchingCamera( const float & delta_time )
{
	if ( !isActivePlayerWatching( ) ) return;
	if ( ENV->pushKey( ci::app::KeyEvent::KEY_1 ) )
	{
		watching_target_player_id = 0;
	}
	else if ( ENV->pushKey( ci::app::KeyEvent::KEY_2 ) )
	{
		watching_target_player_id = 1;
	}
	else if ( ENV->pushKey( ci::app::KeyEvent::KEY_3 ) )
	{
		watching_target_player_id = 2;
	}
	else if ( ENV->pushKey( ci::app::KeyEvent::KEY_4 ) )
	{
		watching_target_player_id = 4; // 3�͊ϐ��
	}
	else if ( ENV->pushKey( ci::app::KeyEvent::KEY_5 ) )
	{
		watching_target_player_id = 5;
	}
	else if ( ENV->pushKey( ci::app::KeyEvent::KEY_6 ) )
	{
		watching_target_player_id = 6;
	}
	else if ( ENV->pushKey( ci::app::KeyEvent::KEY_ESCAPE ) )
	{
		watching_target_player_id = -1;
	}
	// 7�͊ϐ��
}
void Game::cPlayerManager::setPlayersPosition(std::vector<ci::vec3> positions)
{
	for (int i = 0; i < players.size(); i++) {
		ci::vec3 vec = positions[i] - players[i]->getPos();
		players[i]->move(vec);
	}
}
//�v���C���[�̃R���W�����X�s�[�h�̓R���W�����}�l�[�W����
//��ɌĂ�
void Game::cPlayerManager::playerCollisionAfterUpdate(const float& delta_time)
{
	playerAttack(delta_time);
	for (auto& it : players) {
		it->setColliderSpeed();
		it->gemsUpdate(delta_time);
		it->weaponUpdae(delta_time);
	}
	if ( active_player->isWatching( ) )
	{
		if ( isTargetWatching( ) )
		{
			CAMERA->refPosition = active_player->getPos( );
		}
		else
		{
			CAMERA->refPosition = getPlayer( watching_target_player_id )->getPos( );
		}
	}
	else
	{
		if ( !active_player->isDead( ) ) {
			CAMERA->refPosition = active_player->getPos( ) + ci::vec3( 0, 0, 0 );
		}
	}
}
void Game::cPlayerManager::cameraAfterUpdate( const float & delta_time )
{
	for ( auto& it : players )
	{
		it->cameraAfterUpdate( delta_time );
	}
}
void Game::cPlayerManager::setup(std::vector<ci::vec3> positions, const int& player_number, const int& active_player_id, std::vector<int> teams)
{
	playerInstance(positions, player_number, active_player_id, teams);
	//�|�W�V�����̎Q�ƂƃJ�����̃Y�[����ݒ�
	for (auto& it : players) {
		it->setup();
	}
}
void Game::cPlayerManager::update(const float& delta_time)
{
	playerMove( delta_time );
	for (auto& it : players) {
		it->update(delta_time);
	}
	killCamera(delta_time);
	watchingCamera(delta_time);
	cClientAdapter::getInstance()->sendPlayer(active_player->getPos(), ci::vec2(0));
}

void Game::cPlayerManager::draw()
{
	for (auto& it : players) {
		it->draw();
	}
}

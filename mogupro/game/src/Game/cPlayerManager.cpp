#include <Game/cPlayerManager.h>
#include <Utility/cInput.h>
#include <CameraManager/cCameraManager.h>
#include <Game/cStrategyManager.h>
#include <Game/Strategy/cDrill.h>
#include <Game/cFieldManager.h>
#include <Network.hpp>


void Game::cPlayerManager::playerInstance(std::vector<ci::vec3> positions, const int& player_number, const int& active_player_id)
{
	
	std::vector<ci::vec3> player_pos = positions;
	

	//����
	for (int i = 0; i < player_number; i++) {
		//�ʐM�ő��
		if (i == active_player_id) {
			players.push_back(std::make_shared<Player::cPlayer>(player_pos[i], ci::vec3(0, 0, 0), i, true));
			//�A�N�e�B�u���[�U�ɑ��
			active_player = players[i];
		}
		else {
			players.push_back(std::make_shared<Player::cPlayer>(player_pos[i], ci::vec3(0, 0, 0), i, false));
		}
	}
}

void Game::cPlayerManager::playerDrillMove(const float & delta_time)
{
	//�J�����̕����Ɉړ�
	float player_speed = delta_time * active_player->getSpeed();
	active_player->move(ci::vec3(CAMERA->getCameraLook().x * player_speed, CAMERA->getCameraLook().y * player_speed, CAMERA->getCameraLook().z * player_speed));
}

void Game::cPlayerManager::playerNormalMove(const float& delta_time)
{
	//�v���C���[�̑��x
	float player_speed = active_player->getSpeed() * delta_time;

	ci::vec3 keybord_velocity = ci::vec3(0);

	float x_axis = 0;
	float z_axis = 0;

	
	if (ENV->pressKey(ci::app::KeyEvent::KEY_w)) {
		z_axis = delta_time * active_player->getSpeed();
	}

	if (ENV->pressKey(ci::app::KeyEvent::KEY_s)) {
		z_axis = -delta_time * active_player->getSpeed();
	}

	if (ENV->pressKey(ci::app::KeyEvent::KEY_d)) {
		x_axis = -delta_time * active_player->getSpeed();
	}

	if (ENV->pressKey(ci::app::KeyEvent::KEY_a)) {
		x_axis = delta_time * active_player->getSpeed();
	}

	keybord_velocity += ci::vec3(z_axis*sin(CAMERA->getCameraAngle().x), 0.0f, z_axis*cos(CAMERA->getCameraAngle().x));
	keybord_velocity += ci::vec3(x_axis*cos(CAMERA->getCameraAngle().x), 0.0f, -x_axis*sin(CAMERA->getCameraAngle().x));
	
	active_player->move(keybord_velocity);


	if (ENV->pressKey(ci::app::KeyEvent::KEY_q)) {
		active_player->move(ci::vec3(0, -player_speed, 0));
	}

	if (ENV->pressKey(ci::app::KeyEvent::KEY_e)) {
		active_player->move(ci::vec3(0, player_speed * 10, 0));
	}
	//�@��@�ݒu
	if (ENV->pushKey(ci::app::KeyEvent::KEY_o)) {
		auto drill_pos = Game::cFieldManager::getInstance()->getBlockCenterTopPosition(active_player->getPos());
		Game::cStrategyManager::getInstance()->CreateDrill(active_player->getPos(), 0,Game::Strategy::cDrill::DrillType::Level1, 0);
	}
	
	/*static int id = 0;
	auto& p = Network::cRequestManager::getInstance()->mReqCheckSetQuarry;
	while (!p.empty()) {
		auto q = p.top();
		p.pop();
		auto drill_pos = Game::cFieldManager::getInstance()->getBlockCenterTopPosition(ci::vec3(q.mXPos, q.mYPos, q.mZPos));
		Game::Strategy::cDrill::DrillType type = static_cast<Game::Strategy::cDrill::DrillType>(q.mType >> 1);
		bool isTeam = q.mType & 0x1 == 1;
		auto objectId = ++id;
		Game::cStrategyManager::getInstance()->CreateDrill(ci::vec3(q.mXPos, q.mYPos, q.mZPos), objectId, type, isTeam);
		Network::cUDPManager::getInstance()->send(q.handle, new Network::Packet::Response::cResCheckSetQuarry(true, q.mXPos, q.mYPos, q.mZPos, 0, id++));
	}*/
	
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

	//�@�풆��true 
	active_player->Drilling(false);
	if (ENV->pressKey(ci::app::MouseEvent::LEFT_DOWN)) {
		active_player->Drilling(true);
	}
	if (ENV->isPadPress(ENV->BUTTON_2)) {
		active_player->Drilling(true);
	}

	//�_�b�V��
	if (ENV->pullKey(ci::app::KeyEvent::KEY_SPACE)) {
		active_player->setDefaultSpeed();
	}
	if (ENV->pushKey(ci::app::KeyEvent::KEY_SPACE)) {
		active_player->setSpeed(10.0f);
	}

	//�p�b�h�̔��艼�u��
	if (ENV->isPadPush(ENV->BUTTON_1)) {
		CAMERA->shakeCamera(0.1f, 0.1f);
	}
	CAMERA->setCameraAngle(ci::vec2(ENV->getPadAxis(2)*(-0.05f), ENV->getPadAxis(3)*(-0.05f)));

	padMove(delta_time);

	//�@�풆�͓��������ς��
	if (active_player->isDrilling()) {
		playerDrillMove(delta_time);
	}
	else {
		playerNormalMove(delta_time);
	}

	
	
	if (ENV->pressKey(ci::app::KeyEvent::KEY_UP)) {
		CAMERA->setCameraAngle(ci::vec2(0, 0.05f));
	}
	if (ENV->pressKey(ci::app::KeyEvent::KEY_DOWN))
		CAMERA->setCameraAngle(ci::vec2(0, -0.05f));

	if (ENV->pressKey(ci::app::KeyEvent::KEY_RIGHT))
		CAMERA->setCameraAngle(ci::vec2(-0.05f, 0));

	if (ENV->pressKey(ci::app::KeyEvent::KEY_LEFT))
		CAMERA->setCameraAngle(ci::vec2(0.05f, 0));
}
void Game::cPlayerManager::padMove(const float & delta_time)
{
	ci::vec3 pad_velocity = ci::vec3(0);

	float x_axis = -1 * ENV->getPadAxis(0) * delta_time * active_player->getSpeed();
	float z_axis = -1 * ENV->getPadAxis(1) * delta_time * active_player->getSpeed();

	pad_velocity += ci::vec3(z_axis*sin(CAMERA->getCameraAngle().x), 0.0f, z_axis*cos(CAMERA->getCameraAngle().x));
	pad_velocity += ci::vec3(x_axis*cos(CAMERA->getCameraAngle().x), 0.0f, -x_axis*sin(CAMERA->getCameraAngle().x));
	
	active_player->move(pad_velocity);
}
void Game::cPlayerManager::setPlayersPosition(std::vector<ci::vec3> positions)
{
	for (int i = 0; i < players.size(); i++) {
		ci::vec3 vec = positions[i] - players[i]->getPos();
		players[i]->move(vec);
	}
}
void Game::cPlayerManager::setup(std::vector<ci::vec3> positions, const int& player_number, const int& active_player_id)
{
	playerInstance(positions, player_number, active_player_id);
	//�|�W�V�����̎Q�ƂƃJ�����̃Y�[����ݒ�
	CAMERA->followingCamera(&active_player->getReferencePos(), 15);
	for (auto it : players) {
		it->setup();
	}
}
#include <Network.hpp>
void Game::cPlayerManager::update(const float& delta_time)
{
	//auto& p = Network::cResponseManager::getInstance()->mResPlayer;
	//while (!p.empty())
	//{
	//	auto top = p.top();
	//	p.pop();
	//	player[0]->setPos(cinder::vec3(top.xPos, top.yPos, top.zPos));
	//}

	for (auto it : players) {
		it->update(delta_time);
	}
	playerMove(delta_time);

	//auto pos = player[1]->getPos();
	//Network::cUDPManager::getInstance()->send(Network::cNetworkHandle("10.25.36.137", 25565),
	//	new Network::Packet::Request::cReqPlayer( pos.x, pos.y, pos.z, 0, 0 ));


}

void Game::cPlayerManager::draw()
{
	for (auto it : players) {
		it->draw();
	}
}

#include <Game/cPlayerManager.h>
#include <Utility/cInput.h>
#include <CameraManager/cCameraManager.h>
#include <Game/cStrategyManager.h>
#include <Game/Strategy/cDrill.h>
#include <Game/cFieldManager.h>
#include <Game/cClientAdapter.h>


void Game::cPlayerManager::playerInstance(std::vector<ci::vec3> positions, const int& player_number, const int& active_player_id, std::vector<int> teams)
{
	
	//生成
	for (int i = 0; i < player_number; i++) {
		//通信で代入
		if (i == active_player_id) {
			players.push_back(std::make_shared<Player::cPlayer>(positions[i], i, true,static_cast<Game::Player::Team>(teams[i])));
			//アクティブユーザに代入
			active_player = players[i];
            this->active_player_id = active_player_id;
            this->active_player_team_id = teams[i];
		}
		else {
			players.push_back(std::make_shared<Player::cPlayer>(positions[i], i, false, static_cast<Game::Player::Team>(teams[i])));
		}
	}
}

void Game::cPlayerManager::playerDrillMove(const float & delta_time)
{
	//カメラの方向に移動
	float player_speed = delta_time * active_player->getSpeed();
	active_player->move(ci::vec3(CAMERA->getCameraLook().x * player_speed, CAMERA->getCameraLook().y * player_speed, CAMERA->getCameraLook().z * player_speed));
}

void Game::cPlayerManager::playerNormalMove(const float& delta_time)
{
	//プレイヤーの速度
	float player_speed = active_player->getSpeed() * delta_time;

	ci::vec3 keybord_velocity = ci::vec3(0);

	float x_axis = 0;
	float z_axis = 0;
	//斜め用スタック
	int diagonal = 0;
	
	if (ENV->pressKey(ci::app::KeyEvent::KEY_w)) {
		z_axis = delta_time * active_player->getSpeed();
		diagonal++;
	}

	if (ENV->pressKey(ci::app::KeyEvent::KEY_s)) {
		z_axis = -delta_time * active_player->getSpeed();
		diagonal++;
	}

	if (ENV->pressKey(ci::app::KeyEvent::KEY_d)) {
		x_axis = -delta_time * active_player->getSpeed();
		diagonal++;
	}

	if (ENV->pressKey(ci::app::KeyEvent::KEY_a)) {
		x_axis = delta_time * active_player->getSpeed();
		diagonal++;
	}

	keybord_velocity += ci::vec3(z_axis*sin(CAMERA->getCameraAngle().x), 0.0f, z_axis*cos(CAMERA->getCameraAngle().x));
	keybord_velocity += ci::vec3(x_axis*cos(CAMERA->getCameraAngle().x), 0.0f, -x_axis*sin(CAMERA->getCameraAngle().x));
	
	if (diagonal >= 2) {
		std::sqrtf(keybord_velocity.x);
		std::sqrtf(keybord_velocity.z);
	}

	if (ENV->pressKey(ci::app::KeyEvent::KEY_SPACE)) {
		active_player->jump(true);
	}

	active_player->move(keybord_velocity);

	//掘削機設置
	if (ENV->pushKey(ci::app::KeyEvent::KEY_o)) {
		auto drill_pos = Game::cFieldManager::getInstance()->getBlockTopPosition(active_player->getPos() + active_player->getInstallationPosition());
		//Game::cStrategyManager::getInstance()->CreateDrill(drill_pos, 0,Game::Strategy::cDrill::DrillType::Level1, 0);
		//ci::app::console() << drill_pos << std::endl;
        cClientAdapter::getInstance( )->sendSetQuarry( drill_pos, Game::Strategy::cDrill::DrillType::Level1 );
	}
	
}
void Game::cPlayerManager::playerMove(const float & delta_time)
{
	//カメラのマウス操作ON　OFF
	if (ENV->pushKey(ci::app::KeyEvent::KEY_ESCAPE)) {
		ENV->setMouseControl(mouse_on);
		if (mouse_on == true) {
			mouse_on = false;
		}
		else {
			mouse_on = true;
		}
	}

	//掘削中はtrue 
	active_player->Drilling(false);
	if (ENV->pressKey(ci::app::MouseEvent::LEFT_DOWN)) {
		active_player->Drilling(true);
	}
	if (ENV->isPadPress(ENV->BUTTON_2)) {
		active_player->Drilling(true);
	}

	//ダッシュ
	//304 = シフト
	if (ENV->pullKey(304)) {
		active_player->setDefaultSpeed();
	}
	if (ENV->pushKey(304)) {
		active_player->setSpeed(10.0f);
	}

	//パッドの判定仮置き
	if (ENV->isPadPush(ENV->BUTTON_1)) {
		CAMERA->shakeCamera(0.1f, 0.1f);
	}
	CAMERA->setCameraAngle(ci::vec2(ENV->getPadAxis(2)*(-0.05f), ENV->getPadAxis(3)*(-0.05f)));

	padMove(delta_time);

	//掘削中は動き方が変わる
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
void Game::cPlayerManager::setup(std::vector<ci::vec3> positions, const int& player_number, const int& active_player_id, std::vector<int> teams)
{
	playerInstance(positions, player_number, active_player_id,teams);
	//ポジションの参照とカメラのズームを設定
	for (auto& it : players) {
		it->setup();
	}
}
void Game::cPlayerManager::update(const float& delta_time)
{
    CAMERA->refPosition = active_player->getPos( );
	for (auto& it : players) {
		it->update(delta_time);
	}
	
	playerMove(delta_time);
    cClientAdapter::getInstance( )->sendPlayer( active_player->getPos(), ci::quat() );
}

void Game::cPlayerManager::draw()
{
	for (auto& it : players) {
		it->draw();
	}
}

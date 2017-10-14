#include <Game/Player/cPlayer.h>
#include <Game/Player/cPlayerManager.h>
#include <Utility/cInput.h>
#include <Game/ProductionCamera/cProdactionCamera.h>




void Game::Player::cPlayerManager::playerInstance()
{
	//人数(通信で代入)
	int player_number = 2;
	//自分が操作するユーザーかどうか
	int active_user_number = 0;

	std::vector<ci::vec3> player_pos;
	player_pos.push_back(ci::vec3(0, 0, 0));
	player_pos.push_back(ci::vec3(5, 0, 5));


	//生成
	for (int i = 0; i < player_number; i++) {
		//通信で代入
		if (active_user_number == 0) {
			player.push_back(std::make_shared<Game::Player::cPlayer>(player_pos[i], ci::vec3(0, 0, 0), i, true));
			//アクティブユーザに代入
			active_player = player[i];
		}
		else {
			player.push_back(std::make_shared<Game::Player::cPlayer>(player_pos[i], ci::vec3(0, 0, 0), i, false));
		}

	}
}

void Game::Player::cPlayerManager::playerMove()
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

	if (ENV->isPadPush(ENV->BUTTON_1)) {
		CAMERA->shakeCamera(0.1f, 0.1f);
	}
	CAMERA->setCameraAngle(ci::vec2(ENV->getPadAxis(2)*(-0.05f), ENV->getPadAxis(3)*(-0.05f)));

	if (ENV->pressKey(ci::app::KeyEvent::KEY_w)) {
		active_player->move(ci::vec3(0, 0, active_player->getSpeed()));
	}

	if (ENV->pressKey(ci::app::KeyEvent::KEY_s)) {
		active_player->move(ci::vec3(0, 0, -active_player->getSpeed()));
	}

	if (ENV->pressKey(ci::app::KeyEvent::KEY_d)) {
		active_player->move(ci::vec3(active_player->getSpeed(), 0, 0));
	}

	if (ENV->pressKey(ci::app::KeyEvent::KEY_a)) {
		active_player->move(ci::vec3(-active_player->getSpeed(), 0, 0));
	}

	if (ENV->pressKey(ci::app::KeyEvent::KEY_q)) {
		active_player->move(ci::vec3(0, -active_player->getSpeed(), 0));
	}

	if (ENV->pressKey(ci::app::KeyEvent::KEY_e)) {
		active_player->move(ci::vec3(0, -active_player->getSpeed(), 0));
	}

	if (ENV->pressKey(ci::app::KeyEvent::KEY_UP)) {
		CAMERA->setCameraAngle(ci::vec2(0, 0.05f));
	}

	if (ENV->pressKey(ci::app::KeyEvent::KEY_DOWN))
		CAMERA->setCameraAngle(ci::vec2(0, -0.05f));

	if (ENV->pressKey(ci::app::KeyEvent::KEY_RIGHT))
		CAMERA->setCameraAngle(ci::vec2(0.05f, 0));

	if (ENV->pressKey(ci::app::KeyEvent::KEY_LEFT))
		CAMERA->setCameraAngle(ci::vec2(-0.05f, 0));
}

void Game::Player::cPlayerManager::setup()
{
	playerInstance();
	//ポジションの参照とカメラのズームを設定
	CAMERA->followingCamera(&active_player->getReferencePos(), 15);
	for (auto it : player) {
		it->setup();
	}
}

void Game::Player::cPlayerManager::update(const float& delta_time)
{
	playerMove();
	for (auto it : player) {
		it->update(delta_time);
	}
}

void Game::Player::cPlayerManager::draw()
{
	for (auto it : player) {
		it->draw();
	}
}

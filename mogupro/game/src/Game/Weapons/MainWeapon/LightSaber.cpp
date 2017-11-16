#include <Game/Weapons/MainWeapon/LightSaber.h>
#include <cinder/gl/gl.h>
#include <Game/cPlayerManager.h>
#include <Utility/cInput.h>
#include <CameraManager/cCameraManager.h>
void Game::Weapon::LightSaber::Attack(const float & delta_time)
{
	player_pos = root->get_position_3d();
	//プレイヤーの位置からrayかaabbを生成
	ray.setOrigin(player_pos);
	ray.setDirection(player_vec);
	//aabb.set(player_pos - player_size, player_pos + player_size);

	//hitsのベクターに入れるためのi
	int i = 0;
	for (auto& it : cPlayerManager::getInstance()->getPlayers()) {
		i++;
		//自分の操作しているプレイヤーなら返す
		if (it->getActiveUser())continue;

		//rayが当たったらis_hitがtrueになる
		bool is_hit = it->getAABB().intersects(ray);
		//is_hit = it->getAABB().intersects(aabb);

		//対象のプレイヤーがhitしたら
		if (is_hit) {
			//当たった時の演出として一瞬カメラを揺らす
			CAMERA->shakeCamera(0.1f, 0.1f);
			it->receiveDamage(is_hit, attack);
			//対象のhitをtrueにして
			//対象限定でヒットストップをつけたり当たり判定をなくしたり
			//するんご
			hits[i - 1] = true;
		}
	}
}
using namespace Node::Action;
void Game::Weapon::LightSaber::Operation()
{
	if (ENV->pushKey(ci::app::KeyEvent::KEY_t)) {
		ci::app::console() << push << std::endl;
		//プレイヤーのposとVecとsizeを取得
		player_pos = cPlayerManager::getInstance()->getActivePlayer()->getPos();
		player_vec = player_pos + cPlayerManager::getInstance()->getActivePlayer()->getPlayerVec() * ci::vec3(range * 10);
		root->set_position_3d(player_pos);
		root->set_schedule_update();
		root->run_action(Node::Action::sequence::create(
			ease<ci::EaseOutCubic>::create(move_to::create(1.0F, player_pos + ci::vec3(2,0,0))),
			ease<ci::EaseOutCubic>::create(move_to::create(1.0F, player_pos + ci::vec3(-2, 0, 0)))
		));
	}
}

Game::Weapon::LightSaber::LightSaber()
{
	attack = 30;
	range = 3;
	player_pos = ci::vec3(0);
	player_vec = ci::vec3(0);
}

void Game::Weapon::LightSaber::setup()
{
	//当たった状態をプレイヤーごとに保持するために
	//プレイヤーの個数文プッシュバック
	for (int i = 0; i < cPlayerManager::getInstance()->getPlayers().size();i++) {
		hits.push_back(false);
	}
	root = Node::node::create();
	root->set_schedule_update();
}

void Game::Weapon::LightSaber::update(const float & delta_time)
{
	Operation();
	root->entry_update(delta_time);
	Attack(delta_time);
}

void Game::Weapon::LightSaber::draw()
{
	ci::gl::drawVector(ray.getOrigin(), ray.getDirection());
}

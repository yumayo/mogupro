#include <Game/Weapons/MainWeapon/LightSaber.h>
#include <cinder/gl/gl.h>
#include <Game/cPlayerManager.h>
#include <Utility/cInput.h>
void Game::Weapon::LightSaber::Attack()
{

}

void Game::Weapon::LightSaber::Operation()
{
}

Game::Weapon::LightSaber::LightSaber()
{
	attack = 30;
	range = 3;
}

void Game::Weapon::LightSaber::setup()
{
	//当たった状態をプレイヤーごとに保持するために
	//プレイヤーの個数文プッシュバック
	for (int i = 0; i < cPlayerManager::getInstance()->getPlayers().size();i++) {
		hits.push_back(false);
	}
	
}

void Game::Weapon::LightSaber::update(const float & delta_time)
{

	//プレイヤーのposとVecとsizeを取得
	ci::vec3 player_pos = cPlayerManager::getInstance()->getActivePlayer()->getPos();
	ci::vec3 player_vec = cPlayerManager::getInstance()->getActivePlayer()->getPlayerVec();
	ci::vec3 player_size = ci::vec3(cPlayerManager::getInstance()->getActivePlayer()->getSize());

	//プレイヤーの位置からrayかaabbを生成
	ray.setOrigin(player_pos);
	ray.setDirection(player_pos + player_vec * ci::vec3(range));
	aabb.set(player_pos - player_size, player_pos + player_size);
	
	//hitsのベクターに入れるためのi
	int i = 0;
	for (auto& it : cPlayerManager::getInstance()->getPlayers()) {
		i++;
		//自分の操作しているプレイヤーなら返す
		if (it->getActiveUser())continue;

		//rayが当たったらis_hitがtrueになる
		bool is_hit = it->getAABB().intersects(ray);
		is_hit = it->getAABB().intersects(aabb);

		//対象のプレイヤーがhitしたら
		if (hits[i - 1] == false) {
			it->receiveDamage(is_hit, attack);
			//対象のhitをtrueにして
			//対象限定でヒットストップをつけたり当たり判定をなくしたり
			//するんご
			hits[i-1] = true;
		}
	}
}

void Game::Weapon::LightSaber::draw()
{
	ci::gl::drawVector(ray.getOrigin(), ray.getDirection()/ci::vec3(10));
}

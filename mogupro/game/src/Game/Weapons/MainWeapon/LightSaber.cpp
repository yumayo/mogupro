#include <Game/Weapons/MainWeapon/LightSaber.h>
#include <cinder/gl/gl.h>
#include <Game/cPlayerManager.h>
#include <Utility/cInput.h>
#include <CameraManager/cCameraManager.h>
#include <Resource/cObjectManager.h>
#include <Resource/TextureManager.h>


void Game::Weapon::LightSaber::Attack(const float & delta_time)
{

	weapon_pos = player_pos + ci::vec3(sin(rotate.x + player_rotate_x), sin(rotate.y), cos(rotate.x + player_rotate_x));

	weapon_vec = ci::vec3(sin(rotate.x + player_rotate_x), sin(rotate.y), cos(rotate.x + player_rotate_x));
	
	//プレイヤーの位置からrayかaabbを生成
	ray.setOrigin(weapon_pos);
	ray.setDirection(weapon_pos + weapon_vec);

	//hitsのベクターに入れるためのi
	int i = 0;
	for (auto& it : cPlayerManager::getInstance()->getPlayers()) {
		i++;
		//自分の操作しているプレイヤーなら返す
		if (it->getActiveUser())continue;

		//rayが当たったらis_hitがtrueになる
		bool is_hit = it->getAABB().intersects(ray);
		float min;
		float max;
		it->getAABB().intersect(ray,&min,&max);
		debug_pos = ray.calcPosition(max);

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
		light = Game::cLightManager::getInstance()->addPointLight(weapon_pos,ci::vec3(1,0.5f,0.5f),0.5f);
		//プレイヤーのposとVecとsizeを取得
		weapon_pos = cPlayerManager::getInstance()->getActivePlayer()->getPos();
		player_pos = cPlayerManager::getInstance()->getActivePlayer()->getPos();
		weapon_vec = cPlayerManager::getInstance()->getActivePlayer()->getPos();
		player_rotate_x = cPlayerManager::getInstance()->getActivePlayer()->getRotateY();
		player_rotate_y = cPlayerManager::getInstance()->getActivePlayer()->getRotateX();

		root_x->set_schedule_update();
		root_x->run_action(Node::Action::sequence::create(
			ease<ci::EaseInOutAtan>::create(float_to::create(0.5f, rotate.x, M_PI/2, [this](float t) {
			light->reAttachPositionWithRadius(light, weapon_pos, 0.5f);
			this->rotate.x = t;
		})), Node::Action::call_func::create([this]() {
			this->rotate.x = -M_PI/2;
			Game::cLightManager::getInstance()->removePointLight(light);
		})
		));

		root_y->set_schedule_update();
		root_y->run_action(Node::Action::sequence::create(
			ease<ci::EaseInOutAtan>::create(float_to::create(0.5f, rotate.y, -M_PI / 2, [this](float t) {
			this->rotate.y = t;
		})), Node::Action::call_func::create([this]() {
			this->rotate.y = M_PI/2;
		})
			));

	}
}

Game::Weapon::LightSaber::LightSaber()
{
	attack = 30;
	range = 3;
	player_rotate_x = 0;
	player_rotate_y = 0;
	debug_pos = ci::vec3(0);
	player_pos = ci::vec3(0);
	rotate = ci::vec2(-M_PI/2, M_PI / 2);
	weapon_pos = ci::vec3(0);
	weapon_vec = ci::vec3(0);
}

void Game::Weapon::LightSaber::setup()
{
	mesh = Resource::cObjectManager::getInstance()->findObject("montamogura/weapon.obj");
	TEX->set("weapon", "OBJ/montamogura/weapon.png");
	//当たった状態をプレイヤーごとに保持するために
	//プレイヤーの個数文プッシュバック
	for (int i = 0; i < cPlayerManager::getInstance()->getPlayers().size();i++) {
		hits.push_back(false);
	}
	root_x = Node::node::create();
	root_x->set_schedule_update();
	root_y = Node::node::create();
	root_y->set_schedule_update();
}

void Game::Weapon::LightSaber::update(const float & delta_time)
{
	Operation();
	root_x->entry_update(delta_time);
	root_y->entry_update(delta_time);
	Attack(delta_time);
}

void Game::Weapon::LightSaber::draw()
{
	//ci::gl::drawSphere(debug_pos, 0.1f, 12);
	ci::gl::drawVector(ray.getOrigin(), ray.getDirection());
	/*ci::gl::ScopedTextureBind tex(TEX->get("weapon"));
	ci::gl::pushModelView();
	ci::gl::translate(weapon_pos);
	ci::gl::rotate(M_PI/2, ci::vec3(0, 1, 0));
	ci::gl::rotate(-M_PI / 2, ci::vec3(0, 0, 1));

	
	ci::gl::rotate(player_rotate_x,ci::vec3(0,1,0));

	ci::gl::scale(ci::vec3(0.02f, 0.02f, 0.02f));
	ci::gl::draw(mesh);
	ci::gl::popModelView();*/
}

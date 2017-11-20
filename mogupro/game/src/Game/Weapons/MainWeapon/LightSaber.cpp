#include <Game/Weapons/MainWeapon/LightSaber.h>
#include <cinder/gl/gl.h>
#include <Game/cPlayerManager.h>
#include <Utility/cInput.h>
#include <CameraManager/cCameraManager.h>
#include <Resource/cObjectManager.h>
#include <Resource/TextureManager.h>
#include <Resource/cSoundManager.h>
float Linear(float t, float b, float e) {
	return (e - b) * t + b;
}
using namespace Node::Action;
void Game::Weapon::LightSaber::Attack1()
{
	float time = motion1;
	root_x->set_schedule_update();
	root_x->run_action(Node::Action::sequence::create(
		ease<ci::EaseOutCubic>::create(float_to::create(time, rotate.x, M_PI / 2, [this](float t) {
		this->rotate.x = t;
	})), Node::Action::call_func::create([this]() {
		//初期化
		if (light != nullptr) {
			Game::cLightManager::getInstance()->removePointLight(light);
		}
	})
		));

	root_y->set_schedule_update();
	root_y->run_action(Node::Action::sequence::create(
		ease<ci::EaseOutCubic>::create(float_to::create(time, rotate.y, -M_PI / 2, [this](float t) {
		this->rotate.y = t;
	}))));

}

void Game::Weapon::LightSaber::Attack2()
{
	float time = motion2;
	motion = Motion::attack_2;
	root_x->set_schedule_update();
	root_x->run_action(Node::Action::sequence::create(
		ease<ci::EaseOutCubic>::create(float_to::create(time, rotate.x, -M_PI / 2, [this](float t) {
		this->rotate.x = t;
	})), Node::Action::call_func::create([this]() {
		//初期化
		if (light != nullptr) {
			Game::cLightManager::getInstance()->removePointLight(light);
		}
	})
		));

	rotate.y = 0;

}

void Game::Weapon::LightSaber::Attack(const float & delta_time)
{
	if (!is_attack)return;
	weapon_draw_pos = player_pos + glm::normalize(ci::vec3(sin(rotate.x + player_rotate_x), sin(rotate.y), cos(rotate.x + player_rotate_x)));
	aabb = ci::AxisAlignedBox(weapon_draw_pos - ci::vec3(range / 2), weapon_draw_pos + ci::vec3(range / 2));
	if(light != nullptr)
	light->reAttachPositionWithRadius(light, weapon_draw_pos, 2);
	//hitsのベクターに入れるためのi
	int i = -1;
	for (auto& it : cPlayerManager::getInstance()->getPlayers()) {
		i++;
		//自分の操作しているプレイヤーなら返す
		if (it->getActiveUser()) {
			continue;
		}
		//既に当たっていたら返す
		if (hits[i - 1]) {
			continue;
		}
		//AABBに入ってなければ判定しない
		if (!aabb.intersects(it->getAABB()))continue;

		//rayが当たったらis_hitがtrueになる
		bool is_hit = false;

		float min = 0.0F, max = 0.0F;

		ci::vec2 rotate_buf;
		//速度が速くても当たるように１フレームに５０分割
		int split = 50;
		for (int k = 0; k < split; k++) {

			rotate_buf.x = Linear((float)k / split, rotate_before_frame.x, rotate.x);
			rotate_buf.y = Linear((float)k / split, rotate_before_frame.y, rotate.y);

			weapon_pos = player_pos + glm::normalize(ci::vec3(sin(rotate_buf.x + player_rotate_x), sin(rotate_buf.y), cos(rotate_buf.x + player_rotate_x))) / ci::vec3(10);

			weapon_vec = player_pos + glm::normalize(ci::vec3(sin(rotate_buf.x + player_rotate_x), sin(rotate_buf.y), cos(rotate_buf.x + player_rotate_x))) * ci::vec3(10) * ci::vec3(range);

			//プレイヤーの位置からrayかaabbを生成
			ray[0].setOrigin(weapon_pos);
			ray[0].setDirection(weapon_vec);

			//１回目の攻撃
			Rotation1(rotate_buf, M_PI / 5);

			for (int m = 0; m < 3; m++) {

				if (it->getAABB().intersect(ray[m], &min, &max) != 0)
				{
					if (min >= 0.0F && min <= 1.0F)
					{
						if (min < std::numeric_limits<float>::max())
						{
							is_hit = true;
						}
					}
				}
				if (is_hit)break;
			}
			if (is_hit)break;
		}
		//対象のプレイヤーがhitしたら
		if (is_hit) {
			//当たった時の演出として一瞬カメラを揺らす
			CAMERA->shakeCamera(0.1f, 0.1f);
			it->receiveDamage(attack);
			ci::app::console() << i - 1 << std::endl;
			//対象のhitをtrueにして
			//対象限定でヒットストップをつけたり当たり判定をなくしたり
			//するんご
			hits[i - 1] = true;
		}

	}

	rotate_before_frame = rotate;
}
void Game::Weapon::LightSaber::Rotation1(ci::vec2 rotate_buf, float rotation)
{
	weapon_pos = player_pos + glm::normalize(ci::vec3(sin(rotate_buf.x + player_rotate_x), sin(rotate_buf.y + rotation), cos(rotate_buf.x + player_rotate_x))) / ci::vec3(10);

	weapon_vec = player_pos + glm::normalize(ci::vec3(sin(rotate_buf.x + player_rotate_x), sin(rotate_buf.y + rotation), cos(rotate_buf.x + player_rotate_x))) * ci::vec3(10) * ci::vec3(range);

	//プレイヤーの位置からrayかaabbを生成
	ray[1].setOrigin(weapon_pos);
	ray[1].setDirection(weapon_vec);

	weapon_pos = player_pos + glm::normalize(ci::vec3(sin(rotate_buf.x + player_rotate_x), sin(rotate_buf.y - rotation), cos(rotate_buf.x + player_rotate_x))) / ci::vec3(10);

	weapon_vec = player_pos + glm::normalize(ci::vec3(sin(rotate_buf.x + player_rotate_x), sin(rotate_buf.y - rotation), cos(rotate_buf.x + player_rotate_x))) * ci::vec3(10) * ci::vec3(range);

	//プレイヤーの位置からrayかaabbを生成
	ray[2].setOrigin(weapon_pos);
	ray[2].setDirection(weapon_vec);
}

void Game::Weapon::LightSaber::Operation(const float & delta_time)
{
	//プレイヤーのposとVecとsizeを取得
	weapon_pos = cPlayerManager::getInstance()->getActivePlayer()->getPos();
	player_pos = cPlayerManager::getInstance()->getActivePlayer()->getPos();
	weapon_vec = cPlayerManager::getInstance()->getActivePlayer()->getPos();
	player_rotate_x = cPlayerManager::getInstance()->getActivePlayer()->getRotateY();
	player_rotate_y = cPlayerManager::getInstance()->getActivePlayer()->getRotateX();
	if (!is_attack) {
		player_pos = cPlayerManager::getInstance()->getActivePlayer()->getPos();
		ray[0].setOrigin(player_pos);
		ray[0].setDirection(player_pos);

	}
	else {
		return;
	}

	if (pull) {
		Resource::cSoundManager::getInstance()->findSe("Player/swing2.wav").play();
		if (light == nullptr) {
			light = Game::cLightManager::getInstance()->addPointLight(player_pos + glm::normalize(ci::vec3(sin(rotate.x + player_rotate_x), sin(rotate.y), cos(rotate.x + player_rotate_x))), ci::vec3(0.5f, 0.5f, 0.0f), 0.5f);
		}
		
		
		
		if (motion == Motion::attack_2) {
			rotate = ci::vec2(-M_PI / 2, M_PI / 2);
			Attack1();
			motion = Motion::attack_1;
			is_attack = true;
			for (int i = 0; i < cPlayerManager::getInstance()->getPlayers().size();i++) {
				hits[i] = false;
			}
			timer = 0;
		}

		if (timer > 0.3f && timer <= 1 && motion == Motion::attack_1) {
			rotate = ci::vec2(2*M_PI / 3, 0);
			Attack2();
			motion = Motion::attack_2;
			timer = 0;
			for (int i = 0; i < cPlayerManager::getInstance()->getPlayers().size();i++) {
				hits[i] = false;
			}
			is_attack = true;
		}
		rotate_before_frame = rotate;
	}
}


Game::Weapon::LightSaber::LightSaber()
{
	attack = 30;
	range = 2;
	player_rotate_x = 0;
	player_rotate_y = 0;
	is_attack = false;
	player_pos = ci::vec3(0);
	rotate = ci::vec2(-M_PI / 2, M_PI / 2);
	timer = 0;
	weapon_pos = ci::vec3(0);
	weapon_vec = ci::vec3(0);
	weapon_draw_pos = ci::vec3(0);
	motion = Motion::attack_2;
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
	timer += delta_time;
	
	if (timer > motion1) {
		is_attack = false;
	}
	if (timer > 1) {
		motion = Motion::attack_2;
	}
	Operation(delta_time);

	cPlayerManager::getInstance()->getActivePlayer()->setStan(is_attack);
	root_x->entry_update(delta_time);
	root_y->entry_update(delta_time);
	Attack(delta_time);
}

void Game::Weapon::LightSaber::DrawRotate1()
{
	if (motion == Motion::attack_1) {
		ci::gl::translate(weapon_draw_pos);
		ci::gl::rotate(M_PI / 2, ci::vec3(0, 1, 0));
		ci::gl::rotate(player_rotate_x, ci::vec3(0, 1, 0));
		ci::gl::rotate(-M_PI / 2, ci::vec3(0, 0, 1));
		ci::gl::rotate(-M_PI / 4, ci::vec3(0, 1, 0));
		ci::gl::rotate(M_PI / 2, ci::vec3(0, 0, 1));
		ci::gl::rotate(rotate.x, ci::vec3(0, 0, 1));
	}
}

void Game::Weapon::LightSaber::DrawRotate2()
{
	if (motion == Motion::attack_2) {
		ci::gl::translate(weapon_draw_pos);
		ci::gl::rotate(M_PI / 2, ci::vec3(0, 1, 0));
		ci::gl::rotate(player_rotate_x, ci::vec3(0, 1, 0));
		ci::gl::rotate(-M_PI / 2, ci::vec3(0, 0, 1));
		ci::gl::rotate(M_PI / 4, ci::vec3(0, 1, 0));
		ci::gl::rotate(-rotate.x + M_PI / 2, ci::vec3(0, 0, 1));
	}
}

void Game::Weapon::LightSaber::DrawRotate3()
{
}
void Game::Weapon::LightSaber::draw()
{
	if (!is_attack) return;
	ci::gl::drawVector(ray[0].getOrigin(), ray[0].getDirection());
	//ci::gl::drawVector(ray[1].getOrigin(), ray[1].getDirection());
	//ci::gl::drawVector(ray[2].getOrigin(), ray[2].getDirection());
	ci::gl::ScopedTextureBind tex(TEX->get("weapon"));
	ci::gl::pushModelView();
	DrawRotate1();
	DrawRotate2();
	ci::gl::scale(ci::vec3(0.02f, 0.02f, 0.02f));
	ci::gl::draw(mesh);
	ci::gl::popModelView();
}

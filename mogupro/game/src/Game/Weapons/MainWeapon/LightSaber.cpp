#include <Game/Weapons/MainWeapon/LightSaber.h>
#include <cinder/gl/gl.h>

void Game::Weapon::LightSaber::Attack()
{

}

Game::Weapon::LightSaber::LightSaber()
{
	attack = 30;
	range = 2;
	is_attack = false;
}

void Game::Weapon::LightSaber::setup()
{
}

void Game::Weapon::LightSaber::update(const float & delta_time)
{
	ray.setOrigin(center_pos);
	ray.setDirection(center_pos + player_vec * range);
}

void Game::Weapon::LightSaber::draw()
{
	//ci::gl::drawSphere(,0.1f,12);
	ci::gl::drawVector(ray.getOrigin(),ray.getDirection());
}

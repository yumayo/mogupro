#include <Game/Player/cPlayer.h>
#include <Collision/cAABBCollider.h>
#include <Game/cFieldManager.h>
//コンストラクタで
//位置と角度と何Pかとどれが自分のプレイヤーかを
//もらう
Game::Player::cPlayer::cPlayer(
	const ci::vec3& pos,
	const ci::vec3& center_angle,
	const int& id,
	const bool& is_active_user)
	: cObjectBase(pos), 
	mCollider(mPos, ci::vec3(0.8F)),
	mRigidbody(mCollider)
{
	size = ci::vec3(1);
	color = ci::vec4(1);
	color = ci::ColorA8u(1, 0, 1, 1);
	drilling = false;
	speed = 5.0f;
	this->center_angle = center_angle;
	player_id = id;
	active_user = is_active_user;
}

void Game::Player::cPlayer::setup()
{
}

void Game::Player::cPlayer::update(const float & delta_time)
{
	if (drilling) {
		Game::cFieldManager::getInstance()->blockBreak(mPos, 1);
	}
	//mPos = mCollider.getPosition();
	//size = mCollider.getSize();
}

void Game::Player::cPlayer::draw()
{
	ci::gl::pushModelView();
	ci::gl::translate(mPos);
	ci::gl::rotate(center_angle.x, ci::vec3(1, 0, 0));
	ci::gl::rotate(center_angle.y, ci::vec3(0, 1, 0));
	ci::gl::rotate(center_angle.z, ci::vec3(0, 0, 1));
	ci::gl::scale(size);
	ci::gl::color(color);
	ci::gl::ScopedGlslProg sgp(ci::gl::getStockShader(ci::gl::ShaderDef().lambert()));
	ci::gl::drawCube(ci::vec3(0,0,0), ci::vec3(1, 1, 1));
	ci::gl::color(1,1,1,1);
	ci::gl::popModelView();
}

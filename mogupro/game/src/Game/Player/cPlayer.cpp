#include <Game/Player/cPlayer.h>
#include <Collision/cAABBCollider.h>
#include <Game/cFieldManager.h>
#include <CameraManager/cCameraManager.h>
#include <Resource/cObjectManager.h>
#include <Resource/TextureManager.h>
//void Game::Player::cPlayer::playerRotation()
//{
//	//プレイヤーの前方向
//	ci::vec3 rotateaxis = ci::vec3(0.0f, 0.0f, 1.0f);
//	//移動先のベクトル
//	ci::vec3 targetvec = ci::vec3(velocity.x, 0, velocity.z);
//	//移動方向をnormalize
//	targetvec = ci::normalize(targetvec);
//	//回転軸
//	ci::vec3 quataxis = glm::cross(rotateaxis, targetvec);
//	//回転
//	// 左回り
//	if (atan2f(velocity.x, velocity.z) > 0.0f)
//		ci::gl::rotate(atan2f(velocity.x, velocity.z), quataxis);
//	// 右回り
//	else if (atan2f(velocity.x, velocity.z) < 0.0f)
//		ci::gl::rotate(-atan2f(velocity.x, velocity.z), quataxis);
//
//	if (quataxis == ci::vec3(0))
//	{
//		if (velocity.z > 0.0f)
//			ci::gl::rotate(0, ci::vec3(0, 1, 0));
//		if (velocity.z < 0.0f)
//			ci::gl::rotate((float)M_PI, ci::vec3(0, 1, 0));
//	}
//
//}

void Game::Player::cPlayer::playerRotation()
{
	//プレイヤーの前方向
	ci::vec3 rotateaxis = ci::vec3(0.0f, 0.0f, 1.0f);
	//移動先のベクトル
	ci::vec3 targetvec = ci::vec3(velocity.x, 0, velocity.z);
	//移動方向をnormalize
	targetvec = ci::normalize(targetvec);

	//回転軸
	ci::vec3 quataxis = glm::cross(rotateaxis, targetvec);

	float rotation = atan2f(velocity.x, velocity.z);

	//回転
	// 左回り
	if (rotation > 0.1f) {
		ci::gl::rotate(rotation, quataxis);
		save_rotate = rotation;
	}
	// 右回り
	else if (rotation < -0.1f) {
		ci::gl::rotate(-rotation, quataxis);
		save_rotate = rotation;
	}

	//ベクトルが出なければ
	if (rotation <= 0.1f &&
		rotation >= -0.1f) {
		if (save_rotate > 0.1f)
			ci::gl::rotate(save_rotate, ci::vec3(0, 1, 0));
		if (save_rotate < -0.1f)
			ci::gl::rotate(save_rotate, ci::vec3(0, 1, 0));
	}

}


//コンストラクタで
//位置と角度と何Pかとどれが自分のプレイヤーかを
//もらう
Game::Player::cPlayer::cPlayer(
	const ci::vec3& pos,
	const ci::vec3& center_angle,
	const int& id,
	const bool& is_active_user)
	: cObjectBase(pos),
	mCollider(mPos, ci::vec3(1.5, 2.3, 1.5)),
	mRigidbody(mCollider)
{
	size = ci::vec3(1);
	color = ci::vec4(1);
	color = ci::ColorA8u(1, 0, 1, 1);
	velocity = ci::vec3(0);
	save_rotate = 0;
	drilling = false;
	speed = DEFAULT_SPEED;
	player_id = id;
	active_user = is_active_user;
}

void Game::Player::cPlayer::move(const ci::vec3 & velocity)
{

	auto speed = mRigidbody.getSpeed() - this->velocity;

	//プレイヤーの移動ベクトル保存
	this->velocity = velocity;

	mRigidbody.setSpeed(ci::vec3(0,speed.y,0) + velocity);
	//プレイヤーの移動
	mPos = mCollider.getPosition();

}

void Game::Player::cPlayer::setup()
{
	mCollider.addWorld();
	mRigidbody.addWorld();
	
	mesh = Resource::cObjectManager::getInstance()->findObject("montamogura/moguraHontai.obj");
	TEX->set("mogura", "OBJ/montamogura/moguraHontai.png");
}

void Game::Player::cPlayer::update(const float & delta_time)
{
	if (drilling) {
		Game::cFieldManager::getInstance()->blockBreak(mPos, 3);
	}
	/*mPos = mCollider.getPosition();
	size = mCollider.getSize();*/
}

void Game::Player::cPlayer::draw()
{
	ci::gl::pushModelView();
	ci::gl::translate(mPos - ci::vec3(0, 1, 0));
	ci::gl::scale(ci::vec3(0.02f) + ci::vec3(0.01f, 0, 0.01f));
	playerRotation();
	ci::gl::color(color);
	TEX->get("mogura")->bind();
	ci::gl::ScopedGlslProg sgp(ci::gl::getStockShader(ci::gl::ShaderDef().texture()));
	ci::gl::draw(mesh);
	Resource::TextureManager::getInstance()->get("mogura")->unbind();
	ci::gl::color(1, 1, 1, 1);
	ci::gl::popModelView();
}

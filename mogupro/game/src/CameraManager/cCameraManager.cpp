#include <CameraManager/cCameraManager.h>
#include <random>
#include <cinder/gl/gl.h>
#include <Utility/cInput.h>
#include <Collision/cCollisionManager.h>
#include <Game/cPlayerManager.h>
namespace CameraManager
{
void cCameraManager::shakeCamera( const float & scatter, const float & seconds )
{
    this->scatter = scatter;
    this->seconds = seconds;
}

void cCameraManager::shakeCamera2D(const float & scatter, const float & seconds)
{
	scatter2D = scatter;
	seconds2D = seconds;
}

void cCameraManager::setup( ) {
    camera.setAspectRatio( ci::app::getWindowAspectRatio( ) );
    camera.lookAt( refPosition, ci::vec3( 0 ), ci::vec3( 0, 1, 0 ) );
	camera_mode = TPS;
	up = ci::vec3( 0, 1, 0 );
	scheduleUpdate = true;
    camera.setFarClip( 10000 );
    auto size = ci::app::getWindowSize( );
    camera_2d.setOrtho( -size.x / 2, size.x / 2, -size.y / 2, size.y / 2, 0.125F, 1000.0F );
    camera_2d.lookAt( ci::vec3( 0, 0, 500.0F ), ci::vec3( 0, 0, 0.0F ), ci::vec3( 0, 1, 0 ) );
	camera_angle.y = -0.25f;
	my_scatter2D = ci::vec2(0.0f);
	scatter2D = 0.0f;
	seconds2D = 0.0f;
}

//慣性つきカメラ移動
void cCameraManager::MovingCamera( )
{
	//プレイヤーが死んだらキルカメラ
	if (Game::cPlayerManager::getInstance()->getActivePlayer() != nullptr &&
		( Game::cPlayerManager::getInstance()->getActivePlayer()->isDead() || 
		!Game::cPlayerManager::getInstance( )->isTargetWatching( ) ) ) {
		//慣性移動
		buf_pos = refPosition - pos;
		buf_pos *= 0.25f;
		pos += buf_pos;
		
	}
	else {
		//生きているときは常にプレイヤーの位置
		pos = refPosition;
	}
}
void cCameraManager::ScatterCamera( )
{
	std::random_device rd;
	std::mt19937 mt(rd());
	{
		my_scatter = ci::vec3(0);
		if (seconds < 0)return;
		std::uniform_real_distribution<float> random_x(-scatter, scatter);
		std::uniform_real_distribution<float> random_y(-scatter, scatter);
		std::uniform_real_distribution<float> random_z(-scatter, scatter);
		float buf_x = random_x(mt);
		float buf_y = random_y(mt);
		float buf_z = random_z(mt);
		my_scatter = ci::vec3(buf_x, buf_y, buf_z);
	}
	
	
}
void cCameraManager::setCameraAngle( ci::vec2 const & angle )
{
	camera_angle = angle;
	camera_angle.y = std::min( float( M_PI / 2 ) - 0.01f,
							   std::max( camera_angle.y, -float( M_PI / 2 ) + 0.01f ) );
	camera_angle.x = std::fmod( camera_angle.x, M_PI * 2.0 );
}

void cCameraManager::ScatterCamera2D()
{
	std::random_device rd;
	std::mt19937 mt(rd());
	{
		my_scatter2D = ci::vec2(0);
		if (seconds2D < 0)return;
		std::uniform_real_distribution<float> random_x(-scatter2D, scatter2D);
		std::uniform_real_distribution<float> random_y(-scatter2D, scatter2D);
		float buf_x = random_x(mt);
		float buf_y = random_y(mt);
		my_scatter2D = ci::vec2(buf_x, buf_y);
	}
}

void cCameraManager::update(const float& delta_time) {

	//ブレるカメラの秒数をデルタタイムで引く
	seconds -= delta_time;
	seconds2D -= delta_time;
	ScatterCamera();
	ScatterCamera2D();
	if (scheduleUpdate)
	{
		looking_point.x = camera_far * sin(camera_angle.x) * cos(camera_angle.y);
		looking_point.z = camera_far * cos(camera_angle.x) * cos(camera_angle.y);
		looking_point.y = camera_far * sin(camera_angle.y);

		MovingCamera();

		looking_position = pos - looking_point;
	}

	auto origin = pos;
	auto target = looking_position;
	ci::vec3 direction;
	if (camera_mode == CAMERA_MODE::TPS) {
		direction = target - origin;
	}
	else if (camera_mode == CAMERA_MODE::FPS) {
		direction = origin - target;
	}

	if (scheduleUpdate)
	{
		cinder::Ray ray(origin, direction);

		target = Collision::cCollisionManager::getInstance()->calcNearestPoint(ray, 1 << 2);
	}


	if (camera_mode == CAMERA_MODE::TPS) {
		camera.lookAt(target + my_scatter, origin + my_scatter, up);
	}
	else if (camera_mode == CAMERA_MODE::FPS) {
		camera.lookAt(origin + my_scatter, target + my_scatter, up);
	}
	camera_2d.lookAt(ci::vec3(my_scatter2D.x, my_scatter2D.y, 500.0F), ci::vec3(my_scatter2D.x, my_scatter2D.y, 0.0F), ci::vec3(0, 1, 0));
}

void cCameraManager::bind3D( )
{
	ci::gl::enableDepthRead( );
	ci::gl::enableDepthWrite( );
    ci::gl::pushMatrices( );
    ci::gl::setMatrices( camera );
}

void cCameraManager::unBind3D( )
{
    ci::gl::popMatrices( );
}

void cCameraManager::bind2D( )
{
	ci::gl::disableDepthRead( );
	ci::gl::disableDepthWrite( );
    ci::gl::pushMatrices( );
    ci::gl::setMatrices( camera_2d );
}

void cCameraManager::unBind2D( )
{
    ci::gl::popMatrices( );
}
}

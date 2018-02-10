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

void cCameraManager::setup( ) {
    camera.setAspectRatio( ci::app::getWindowAspectRatio( ) );
    camera.lookAt( refPosition, ci::vec3( 0 ), ci::vec3( 0, 1, 0 ) );
	camera_mode = TPS;
	up = ci::vec3( 0, 1, 0 );
	scheduleUpdate = true;
    camera.setFarClip( 10000 );
    auto size = ci::app::getWindowSize( );
    camera_2d.setOrtho( -size.x / 2, size.x / 2, -size.y / 2, size.y / 2, 0.125F, 100.0F );
    camera_2d.lookAt( ci::vec3( 0, 0, 50.0F ), ci::vec3( 0, 0, 0.0F ), ci::vec3( 0, 1, 0 ) );
	camera_angle.y = -0.25f;
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
	my_scatter = ci::vec2(0,0);
    if ( seconds < 0 )return;
    std::random_device rd;
    std::mt19937 mt( rd( ) );
    std::uniform_real_distribution<float> random_x( -scatter, scatter );
    std::uniform_real_distribution<float> random_y( -scatter, scatter );
    float buf_x = random_x( mt );
    float buf_y = random_y( mt );
    my_scatter = ci::vec2( buf_x, buf_y );

}
void cCameraManager::setCameraAngle( ci::vec2 const & angle )
{
	camera_angle = angle;
	camera_angle.y = std::min( float( M_PI / 2 ) - 0.01f,
							   std::max( camera_angle.y, -float( M_PI / 2 ) + 0.01f ) );
	camera_angle.x = std::fmod( camera_angle.x, M_PI * 2.0 );
}
void cCameraManager::update(const float& delta_time) {

	//ブレるカメラの秒数をデルタタイムで引く
	seconds -= delta_time;

	ScatterCamera();

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
		camera.lookAt(target + ci::vec3(my_scatter.x, my_scatter.y, 0), origin + ci::vec3(my_scatter.x, my_scatter.y, 0), up);
	}
	else if (camera_mode == CAMERA_MODE::FPS) {
		camera.lookAt(origin + ci::vec3(my_scatter.x, my_scatter.y, 0), target + ci::vec3(my_scatter.x, my_scatter.y, 0), up);
	}
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

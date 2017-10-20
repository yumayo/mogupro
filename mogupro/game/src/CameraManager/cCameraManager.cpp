#include <CameraManager/cCameraManager.h>
#include <random>
#include <cinder/gl/gl.h>
#include <Utility/cInput.h>
namespace CameraManager
{
void cCameraManager::shakeCamera( const float & scatter, const float & seconds )
{
    this->scatter = scatter;
    this->seconds = seconds;
}

void cCameraManager::setup( ) {
    camera.setAspectRatio( ci::app::getWindowAspectRatio( ) );
    camera.lookAt( ci::vec3( ( *reference_pos )->x, ( *reference_pos )->y, ( *reference_pos )->z ), ci::vec3( 0 ), ci::vec3( 0, 1, 0 ) );
    camera.setFarClip( 10000 );
    auto size = ci::app::getWindowSize( );
    camera_2d.setOrtho( -size.x / 2, size.x / 2, -size.y / 2, size.y / 2, 0.125F, 5.0F );
    camera_2d.lookAt( ci::vec3( 0, 0, 1.0F ), ci::vec3( 0, 0, 0.0F ), ci::vec3( 0, 1, 0 ) );
}

//慣性つきカメラ移動
void cCameraManager::MovingCamera( )
{
    buf_pos = ci::vec3( ( *reference_pos )->x, ( *reference_pos )->y, ( *reference_pos )->z ) - pos;
    buf_pos *= 0.8f;
    pos += buf_pos;
}
void cCameraManager::ScatterCamera( )
{
    if ( seconds < 0 )return;
    std::random_device rd;
    std::mt19937 mt( rd( ) );
    std::uniform_real_distribution<float> random_x( -scatter, scatter );
    std::uniform_real_distribution<float> random_y( -scatter, scatter );
    float buf_x = random_x( mt );
    float buf_y = random_y( mt );
    my_scatter = ci::vec2( buf_x, buf_y );

}
void cCameraManager::update( const float& delta_time ) {
    //ブレるカメラの秒数をデルタタイムで引く
    seconds -= delta_time;

    ScatterCamera( );

    looking_point.x = camera_far * sin( camera_angle.x ) * cos( camera_angle.y );
    looking_point.z = camera_far * cos( camera_angle.x ) * cos( camera_angle.y );
    looking_point.y = camera_far * sin( camera_angle.y );

    MovingCamera( );

    looking_position = pos - looking_point;

    camera.lookAt( looking_position, pos + ci::vec3( my_scatter.x, my_scatter.y, 0 ) );

}

void cCameraManager::bind3D( )
{
    ci::gl::pushMatrices( );
    ci::gl::setMatrices( camera );
}

void cCameraManager::unBind3D( )
{
    ci::gl::popMatrices( );
}

void cCameraManager::bind2D( )
{
    ci::gl::pushMatrices( );
    ci::gl::setMatrices( camera_2d );
}

void cCameraManager::unBind2D( )
{
    ci::gl::popMatrices( );
}
}

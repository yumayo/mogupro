#pragma once
#include <cinder/app/App.h>
#include <cinder/Camera.h>
#include <Utility/cSingletonAble.h>
#define CAMERA ::CameraManager::cCameraManager::getInstance()
namespace CameraManager
{
enum WINDOW_SIZE {
    WIDTH = 640,
    HEIGHT = 960,
    TEXTURE_WIDTH = 3000,
    TEXTURE_HEIGHT = 4500
};
enum CAMERA_MODE {
	TPS,
	FPS
};
class cCameraManager : public Utility::cSingletonAble<cCameraManager>
{
public:
    ci::vec3 refPosition;
private:
    ci::CameraPersp camera;
    ci::CameraOrtho camera_2d;
    ci::vec3 pos;
    //���炩�ɖړI���W�ɒǏ]���邽�߂�buf
    ci::vec3 buf_pos;

    //�ڕW�ƑΏƓI�ȃJ�����̈ʒu
    ci::vec3 looking_point;

	//�J�����̌����Ă���ʒu
	ci::vec3 looking_position;

    //�J�����̌����Ă���p�x
    ci::vec2 camera_angle;

	CAMERA_MODE camera_mode;

    ci::vec2 my_scatter;
    //�h��镝
    float scatter;
    //�h���b��
    float seconds;

    //�J�����̑Ώۂ���̉���
    float camera_far;

    //�J������

    ////��ʂ��Â������薾�邭�����邷��p�̉摜
    //ci::gl::TextureRef fade_tex;

    //�������J�����ړ�
    void MovingCamera( );

    //�u���铮��
    void ScatterCamera( );
public:


    //��ʂ̃t�F�[�h�l
    ci::vec4 fade_out;


    cCameraManager( ) {
        scatter = 0;
        pos = ci::vec3( 0 );
        buf_pos = ci::vec3( 0 );
        fade_out = ci::vec4( 0, 0, 0, 0 );
        camera_angle = ci::vec2( 0 );
        looking_point = ci::vec3( 0 );
        my_scatter = ci::vec2( 0 );
		looking_position = ci::vec3(0);
		camera_mode = CAMERA_MODE::TPS;
        camera_far = 5;
    }
    ~cCameraManager( ) {

    }

    const ci::CameraPersp& getCamera( ) {
        return camera;
    }


    void setCameraAngle( const ci::vec2& angle ) {
        camera_angle += angle;
        camera_angle.y = std::min( float( M_PI / 2 ) - 0.01f,
                                   std::max( camera_angle.y, -float( M_PI / 2 ) + 0.01f ) );
        camera_angle.x = std::fmod( camera_angle.x, M_PI * 2.0 );
    }
    ci::vec3 getPos( ) {
        return ci::vec3( pos.x, pos.y, pos.z );
    }
    ci::vec2 getCameraAngle( ) {
        return camera_angle;
    }

	//�J�����̌����Ă��������Ԃ�
	ci::vec3 getCameraLook() {
		if (camera_mode = CAMERA_MODE::FPS) {
			return ci::vec3(looking_point.x, looking_point.y, looking_point.z);
		}
		return ci::vec3(looking_point.x / 10, looking_point.y / 10, looking_point.z / 10);
	}
	CAMERA_MODE getCameraMode() {
		return camera_mode;
	}
	void setCameraFar(float& set_far) {
		camera_far = set_far;
		if (camera_far <= 0.5f) {
			camera_mode = CAMERA_MODE::FPS;
			camera_far = 0.4f;
		}
		else if(camera_far > 0.5f) {
			camera_mode = CAMERA_MODE::TPS;
		}
	}

    //�J������h�炷�֐�
    //scattar =�@�u�����A�@seconds�@���@�b��
    void shakeCamera( const float& scatter, const float& seconds );
    void setup( );
    void update( const float& delta_time );
    void bind3D( );
    void unBind3D( );
    void bind2D( );
    void unBind2D( );
};
}

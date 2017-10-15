#pragma once
#include <cinder/app/App.h>
#include <cinder/Camera.h>
#include <Utility/cSingletonAble.h>
#define CAMERA Game::cCameraManager::getInstance()
namespace Game {
enum WINDOW_SIZE {
	WIDTH = 640,
	HEIGHT = 960,
	TEXTURE_WIDTH = 3000,
	TEXTURE_HEIGHT = 4500
};


class cCameraManager : public ::Utility::cSingletonAble<cCameraManager> {
private:
	ci::CameraPersp camera;
	ci::CameraOrtho camera_2d;
	ci::vec3 pos;
	//滑らかに目的座標に追従するためのbuf
	ci::vec3 buf_pos;
	//目標のposの参照
	std::shared_ptr<ci::vec3*> reference_pos;

	//目標と対照的なカメラの位置
	ci::vec3 looking_point;

	//カメラの向いている角度
	ci::vec2 camera_angle;


	ci::vec2 my_scatter;
	//揺れる幅
	float scatter;
	//揺れる秒数
	float seconds;

	//カメラの対象からの遠さ
	float camera_far;

	//カメラの

	////画面を暗くしたり明るくしたるする用の画像
	//ci::gl::TextureRef fade_tex;

	//慣性つきカメラ移動
	void MovingCamera();

	//ブレる動作
	void ScatterCamera();
public:


	//画面のフェード値
	ci::vec4 fade_out;


	cCameraManager() {
		scatter = 0;
		pos = ci::vec3(0);
		buf_pos = ci::vec3(0);
		fade_out = ci::vec4(0, 0, 0, 0);
		camera_angle = ci::vec2(0);
		looking_point = ci::vec3(0);
		my_scatter = ci::vec2(0);
		camera_far = 5;
	}
	~cCameraManager() {

	}

	const ci::CameraPersp& getCamera() {
		return camera;
	}


	void setCameraAngle(const ci::vec2& angle) {
		camera_angle += angle;
		camera_angle.y = std::min(float(M_PI / 2) - 0.01f,
			std::max(camera_angle.y, -float(M_PI / 2) + 0.01f));
		camera_angle.x = std::fmod(camera_angle.x, M_PI * 2.0);
	}
	ci::vec3 getPos() {
		return ci::vec3(pos.x, pos.y, pos.z);
	}
	ci::vec2 getCameraAngle() {
		return camera_angle;
	}

	void followingCamera(ci::vec3* pos, const float& camera_farZ) {
		reference_pos = std::make_shared<ci::vec3*>(pos);
		this->camera_far = camera_farZ;
	}

	//カメラを揺らす関数
	//scattar =　ブレ幅、　seconds　＝　秒数
	void shakeCamera(const float& scatter, const float& seconds);
	void setup();
	void update(const float& delta_time);
	void bind3D();
	void unBind3D();
	void bind2D();
	void unBind2D();
};
}
#include <ProdactionCamera.h>


void ProductionCamera::shakeCamera(const float & scatter, const float & seconds)
{
	this->scatter = scatter;
	this->seconds = seconds;
}

void ProductionCamera::setup() {
	camera.setAspectRatio(ci::app::getWindowAspectRatio());
	camera.lookAt(ci::vec3((*reference_pos)->x, (*reference_pos)->y, (*reference_pos)->z), ci::vec3(0), ci::vec3(0, -1, 0));
	camera.setFarClip(10000);
}

//慣性つきカメラ移動
void ProductionCamera::MovingCamera()
{
	buf_pos = ci::vec3((*reference_pos)->x, (*reference_pos)->y, (*reference_pos)->z) - pos;
	buf_pos *= 0.3f;
	pos += buf_pos;
}
void ProductionCamera::ScatterCamera()
{
	if (seconds < 0)return;
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> random_x(-scatter, scatter);
	std::uniform_real_distribution<float> random_y(-scatter, scatter);
	float buf_x = random_x(mt);
	float buf_y = random_y(mt);
	my_scatter = ci::vec2(buf_x, buf_y);

}
void ProductionCamera::update(const float& delta_time) {
	//ブレるカメラの秒数をデルタタイムで引く
	seconds -= delta_time;

	ScatterCamera();

	looking_point.x = camera_far * sin(camera_angle.x) * cos(camera_angle.y);
	looking_point.z = camera_far * cos(camera_angle.x) * cos(camera_angle.y);
	looking_point.y = camera_far * sin(camera_angle.y);

	MovingCamera();

	

	camera.lookAt(pos + looking_point, pos + ci::vec3(my_scatter.x, my_scatter.y,0));
}

void ProductionCamera::draw()
{
	ci::gl::pushModelMatrix();
	//ci::gl::translate(ci::vec3(pos.x - camera_::TEXTURE_WIDTH /2, pos.x - camera_::TEXTURE_HEIGHT / 2,camera_z + 1500));
	//fade_tex->bind();
	ci::gl::color(fade_out.r, fade_out.g, fade_out.b, fade_out.a);
	/*ci::Rectf drawRect(ci::vec2(
		0,
		0),
		ci::vec2(
			camera_::TEXTURE_WIDTH,
			camera_::TEXTURE_HEIGHT));*/
			//ci::gl::draw(fade_tex, drawRect);
	ci::gl::color(1, 1, 1, 1);
	//fade_tex->unbind();
	ci::gl::popModelMatrix();

}

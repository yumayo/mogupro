#include"TitleCircle.h"

using namespace ci;

TitleCircle::TitleCircle()
{

}

TitleCircle::TitleCircle(cinder::vec2 pos, cinder::vec2 size, cinder::gl::TextureRef tex)
{
	pos_ = pos;
	size_ = size;
	tex_ = tex;
	rotate = 0.0f;
}

void TitleCircle::draw(float posx)
{
	if (pos_.y >-400 && pos_.y<(cinder::app::getWindowHeight() + 400)) {
		gl::pushModelView();
		gl::translate(vec2(pos_.x + posx, pos_.y));
		gl::rotate(rotate);
		gl::ScopedTextureBind tex(tex_);
		gl::drawSolidRect(Rectf(-size_ / 2.0F, size_ / 2.0F));
		gl::popModelView();
	}

}

void TitleCircle::update(float y_speed)
{
	pos_.y += y_speed;
	rotate += 12;
}


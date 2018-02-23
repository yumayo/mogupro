#pragma once
#include "cinder/Vector.h"
#include "cinder/app/App.h"
#include"../Utilitys/Hirasawa.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"

class TitleCircle {
public:
	TitleCircle();
	TitleCircle(cinder::vec2 pos,cinder::vec2 size,cinder::gl::TextureRef tex);
	void draw(float posx);
	void update(float y_speed);
private:
	cinder::vec2 pos_;
	cinder::vec2 size_;
	cinder::gl::TextureRef tex_;
	float rotate;
};
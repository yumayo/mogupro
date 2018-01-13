#pragma once
#include <cinder/app/App.h>
#include <cinder/gl/gl.h>
#include <cinder/svg/Svg.h>
#include <cinder/imageIo.h>
#include <cinder/gl/Texture.h> 
#include "../UIManager/UIManager.h"

class UIPlate : public UIManager
{
protected:
	std::unordered_map<std::string, ci::TextLayout> font;
	
	std::unordered_map<std::string, ci::gl::TextureRef>	gauge_texture;

	ci::Font font_data;
	
public:
	std::unordered_map<std::string, ci::gl::TextureRef>	textures;
	UIPlate() {
		
	};
	virtual void setup(const dess::SceneName& name);
	virtual void update(const float& delta_time);
	virtual void draw();
};
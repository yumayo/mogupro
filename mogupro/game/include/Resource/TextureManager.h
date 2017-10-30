#pragma once
#include <cinder/gl/Texture.h>           
#include <cinder/app/App.h>
#include <cinder/gl/gl.h>
#include <unordered_map>
#include <Utility/cSingletonAble.h>


#define TEX Resource::TextureManager::getInstance()

namespace Resource {
	class TextureManager : public Utility::cSingletonAble<TextureManager>
	{
	private:
		std::unordered_map<std::string, ci::gl::TextureRef> texture;
		ci::Font font;
	public:
		TextureManager() {

		};
		~TextureManager() {};

		//�^�O��Path������B
		//cinder0.9����getAssetPath�͒��ŌĂ΂�Ă���̂�
		//Asset��������Q�Ƃ����OK
		//��@TEX.set("sky_dome","SkyDome/Skydome151004y.jpg");
		void set(const std::string& name, const std::string& path)
		{
			if (texture.find(name) == texture.end()) {
				auto img = ci::loadImage(ci::app::loadAsset(path));
				texture[name] = ci::gl::Texture::create(img);
			}
		}
		ci::gl::TextureRef get(std::string name) {
			return texture[name];
		}
		ci::Font getFont() {
			return font;
		}
	};
}
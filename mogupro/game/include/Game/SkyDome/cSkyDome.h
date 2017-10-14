#pragma once
#include <Game/cObjectBase.h>
#include <cinder/gl/gl.h>
namespace Game {
	namespace SkyDome {
		class cSkyDome : public cObjectBase {
		private:
			//画像
			ci::gl::TextureRef texture;
			//Sphereのメッシュデータ
			ci::gl::BatchRef mSphere;
			//画像を貼るためのシェーダー
			ci::gl::GlslProgRef mGlsl;

			ci::vec3 size;
		public:
			
			void setup() override;
			void update(const float& delta_time) override {};
			void draw() override;
		};
	}
}
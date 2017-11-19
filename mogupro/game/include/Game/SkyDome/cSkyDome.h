#pragma once
#include <Game/cObjectBase.h>
#include <cinder/gl/gl.h>
#include <cinder/gl/VboMesh.h>
namespace Game {
	namespace SkyDome {
		class cSkyDome : public cObjectBase {
		private:
			ci::gl::GlslProgRef mGlsl;
			ci::vec3 worldCenter;
			cinder::gl::VboMeshRef caveMesh;
		public:
			
			void setup() override;
			void update(const float& delta_time) override {};
			void draw() override;
		};
	}
}
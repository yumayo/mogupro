#pragma once
#include <cinder/gl/gl.h>
#include <cinder/app/App.h>

namespace Utility
{
	class TriMeshAnimation
	{
	public:
		//windowSize -> âÊñ ÉTÉCÉY
		//cutSize -> ï™äÑêî
		void make(ci::vec2 windowSize, ci::vec2 cutSize);
		
		void update();

		void draw();

	private:
		std::vector<ci::vec3> centers;
		std::vector<ci::vec3> posAs;
		std::vector<ci::vec3> posBs;
		std::vector<ci::vec3> posCs;
		std::vector<ci::vec3> vertices;
		std::vector<ci::vec2> texCoords;
		std::vector<uint32_t> indices;
		std::vector<ci::vec3> vectors;
		std::vector<ci::vec2> rotate;
		int indexCount;
		ci::vec2 windowSize;
	};
}

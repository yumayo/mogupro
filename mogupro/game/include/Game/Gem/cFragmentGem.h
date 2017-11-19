#pragma once
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"

namespace Game
{
	namespace Gem
	{
		using namespace ci::gl;
		using namespace ci;
		class cFragmentGem
		{
		public:
			cFragmentGem(vec3 position, vec3 scale, ColorA color) : mPosition(position), mScale(scale), mColorA(color) {};
			~cFragmentGem() {};
			void draw();
			void update();

			void setPos(vec3 position) { mPosition = position; }
			void setScale(vec3 scale) { mScale = scale; }
			void setColor(ColorA colorA) { mColorA = colorA; }

		private:

			vec3 mPosition;
			vec3 mScale;
			ColorA mColorA;


		};
	}
}

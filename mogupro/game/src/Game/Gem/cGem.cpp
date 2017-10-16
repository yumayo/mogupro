#include<Game/Gem/cGem.h>

namespace Game
{
	namespace Gem
	{
		void cGem::setUp(ci::vec3 positin, ci::vec3 scale, ci::Color color, GemType type)
		{
			mPosition = positin;
			mScale = scale;
			mColor = color;
		}

		void cGem::draw()
		{
			//tex.binde();
			ci::gl::color(mColor);
			ci::gl::drawCube(mPosition, mScale);
			//tex.bind
		}

		void cGem::update()
		{

		}
	}
}
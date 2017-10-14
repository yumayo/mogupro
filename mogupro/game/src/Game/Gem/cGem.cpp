#include<Game/Gem/cGem.h>

namespace Game
{
	namespace Gem
	{
		void cGem::setUp(vec3 positin, vec3 scale, Color color, GemType type)
		{
			mPosition = positin;
			mScale = scale;
			mColor = color;
		}

		void cGem::draw()
		{
			//tex.binde();
			gl::color(mColor);
			gl::drawCube(mPosition, mScale);
			//tex.bind
		}

		void cGem::update()
		{

		}
	}
}
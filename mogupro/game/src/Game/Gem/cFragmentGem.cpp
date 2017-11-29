#include <Game/Gem/cFragmentGem.h>
namespace Game
{
	namespace Gem
	{
		void cFragmentGem::draw()
		{
			ci::gl::color(ci::Color(mColorA));
			drawCube(vec3(mPosition),vec3(mScale));
			color(ColorA(1,1,1,1));
		}

		void cFragmentGem::update()
		{

		}
	}
}

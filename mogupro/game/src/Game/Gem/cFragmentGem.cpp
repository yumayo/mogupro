#include <Game/Gem/cFragmentGem.h>
namespace Game
{
	namespace Gem
	{
		void cFragmentGem::draw()
		{

			drawCube(vec3(mPosition),vec3(mScale));
			color(ColorA(1,1,1,1));
		}

		void cFragmentGem::update()
		{

		}
	}
}

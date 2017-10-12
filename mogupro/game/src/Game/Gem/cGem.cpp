#include<Game/Gem/cGem.h>

namespace Game
{
	namespace Gem
	{
		void cGem::SetUp(vec3 postion_, vec3 size_, Color color_, GemType type)
		{
			mPosition = postion_;
			mSize = size_;
			mColor = color_;
		}

		void cGem::Draw()
		{
			gl::color(mColor);
			gl::drawCube(mPosition, mSize);
		}

		void cGem::Update()
		{

		}
	}
}
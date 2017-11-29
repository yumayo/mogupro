#include<Game/Gem/cGemStone.h>

namespace Game
{
	namespace Gem
	{
		void cGemStone::setUp(ci::vec3 positin, ci::vec3 scale, ci::ColorA color, GemType type)
		{
			mPosition = positin;
			mScale = scale;
			mColor = color;
		}

		void cGemStone::draw()
		{
		}

		void cGemStone::drawFbo()
		{
		}

		void cGemStone::update()
		{
		}

		void cGemStone::setIndices(int offset)
		{
			for (int i = 0; i < 36; i++)
			{
				indices.push_back(BOXINDICES[i] + offset);
			}
		}


		void cGemStone::deleteGem()
		{
			for (int i = 0; i < indices.size(); i++)
			{
				indices[i] = 0;
			}
		}

	}
}
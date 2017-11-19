#include<Game/Gem/cGem.h>

namespace Game
{
	namespace Gem
	{
		void cGem::setUp(ci::vec3 positin, ci::vec3 scale, ci::ColorA color, GemType type, float delay)
		{
			mPosition = positin;
			mScale = scale;
			mColor = color;
			mDelay = delay;
		}

		void cGem::draw()
		{
			//tex.binde();
			ci::gl::color(mColor);
			ci::gl::drawCube(mPosition, mScale);
		}

		void cGem::drawFbo()
		{

		}

		void cGem::update()
		{
		/*	float distance = glm::distance(cPlayerManager::getInstance()->getActivePlayer()->getPos(), mPosition);
			float Alpha = 0.5 + 0.3 * std::sinf(cTimeMeasurement::getInstance()->totalTime() + mDelay);
			ci::gl::color(ci::ColorA(mColor, Alpha* std::min(visibleRange / distance, 1.0f)));
			for (int i = 0; i < 24; i++)
			{
				fboColorAs[i] = ci::ColorA(mColor, Alpha* std::min(visibleRange / distance, 1.0f));
			}*/
		}

		void cGem::setIndices(int offset)
		{
			for (int i = 0; i < 36; i++)
			{
				indices.push_back(BOXINDICES[i] + offset);
			}
		}


		void cGem::deleteGem()
		{
			for (int i = 0; i < indices.size(); i++)
			{
				indices[i] = 0;
			}
		}

	}
}
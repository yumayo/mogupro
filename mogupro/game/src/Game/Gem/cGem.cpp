#include<Game/Gem/cGem.h>

namespace Game
{
	namespace Gem
	{
		void cGem::setUp(ci::vec3 positin, ci::vec3 scale, ci::Color color, GemType type, float delay)
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
			//tex.bind
		}

		void cGem::drawFbo()
		{
			cTimeMeasurement::getInstance()->make();
			float distance =  glm::distance(cPlayerManager::getInstance()->getActivePlayer()->getPos(), mPosition);
			float Alpha = 0.5 + 0.3 * std::sinf(cTimeMeasurement::getInstance()->totalTime() +mDelay);
			ci::gl::color(ci::ColorA(mColor, Alpha* std::min(visibleRange / distance, 1.0f)));
			ci::gl::drawCube(mPosition, mScale);
		}

		void cGem::update()
		{

		}

	}
}
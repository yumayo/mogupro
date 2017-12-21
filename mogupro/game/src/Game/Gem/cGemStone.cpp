#include <Game/Gem/cGemStone.h>
#include <Utility/cTimeMeasurement.h>

namespace Game
{
	namespace Gem
	{

		cGemStone::cGemStone(int id, ci::vec3 postion, ci::vec3 scale, ci::ColorA color, GemType type)
			: mId(id), mPosition(postion), mScale(scale), mColor(color), mType(type), mIsActive(true), mAabb(postion, scale)
		{
			setColorAs();
			setNomals();
		};
		cGemStone::~cGemStone()
		{
			mAabb.removeWorld();
		};


		void cGemStone::setUp()
		{
			mAabb.addWorld();
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

		//---------------------------//
		//        •óÎ‚ðÁ‚·         //
		//---------------------------//
		//’¸“_‚ðˆê‚©Š‚ÉW‚ß‚Ä•óÎ‚ðŒ©‚¦‚È‚­‚·‚é
		void cGemStone::deleteGem()
		{
			for (size_t i = 0; i < indices.size(); i++)
			{
				indices[i] = 0;
			}
		}
	}
}
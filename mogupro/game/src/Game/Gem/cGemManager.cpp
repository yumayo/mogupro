#include "Game/Gem/cGemManager.h"

namespace Game
{
	namespace Gem
	{
		void cGemManager::SetUp(vec3 center, vec3 randomRange, int gemMaxNum, unsigned long seed)
		{
			mRandomRange = randomRange;
			mGemMaxNum = gemMaxNum;
		}

		void cGemManager::Draw()
		{
			for each (auto g in mGems)
			{
				g.Draw();
			}
		};

		void cGemManager::Update()
		{

		};

		void cGemManager::Create()
		{
			mGems.push_back(cGem(vec3(0, 0, 0), vec3(1, 1, 1), Color(1, 0, 0), Game::Gem::GemType::Dia));
			for (size_t i = 0; i < mGemMaxNum; i++)
			{
				std::random_device seed_gen;
				std::mt19937 engine(seed_gen());
				unsigned long seed = engine();
				randSeed(seed);
				float x = randInt(0, mRandomRange.x);
				float y = randInt(0, mRandomRange.y);
				float z = randInt(0, mRandomRange.z);

				Game::Gem::GemType type = Game::Gem::GemType(randInt(0, Game::Gem::GemType::Iron));

				//F•ª‚¯
				Color color = Color(0, 0, 0);
				switch (type)
				{
				case Game::Gem::GemType::Dia:
					color = Color(0.5f, 0.5f, 0.5f);
					break;
				case Game::Gem::GemType::Gold:
					color = Color(1, 1, 0);
					break;
				case Game::Gem::GemType::Silver:
					color = Color(0, 0, 1);
					break;
				case Game::Gem::GemType::Iron:
					color = Color(0, 1, 0);
					break;
				default:
					break;
				}

				mGems.push_back(cGem(vec3(x, y, z) + mCenter, vec3(1, 1, 1), color, type));
				mGems.push_back(cGem(vec3(-(x + 1), y, mRandomRange.z - z - 1) + mCenter, vec3(1, 1, 1), color, type));
			}
		};
	}
}
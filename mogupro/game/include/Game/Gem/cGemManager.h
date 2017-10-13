#pragma once
#include "cinder/Rand.h"
#include "cGem.h"
#include "../include/Utility/cSingletonAble.h"
#include <vector>
#define GemManager Game::Gem::cGemManager::getInstance()

using namespace std;

namespace Game
{
	namespace Gem
	{
		class cGemManager : public Utility::cSingletonAble<cGemManager>
		{
		public:
			cGemManager() {}
			//center       中心座標
			//randomRange   ランダムの生成範囲
			//ｍGemMaxNuｍ  gemの生成数
			//seed          シード値
			void SetUp(vec3 postion,vec3 center, vec3 randomRange, int mGemMaxNum, unsigned long seed);
			void Draw();
			void Update();
			void Create();
			vector<cGem> GetGems() { return mGems; }
		private:
			vector<cGem> mGems;
			vec3 mPosition;
			vec3 mCenter;
			vec3 mRandomRange;
			int mGemMaxNum;
			unsigned long mSeed;
			//vector<cFuel> fuels;
		};
	}
}
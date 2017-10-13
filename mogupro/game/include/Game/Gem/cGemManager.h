#pragma once
#include "cinder/Rand.h"
#include "cGem.h"
#include "../include/Utility/cSingletonAble.h"
#include <vector>
#include <map>
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
			~cGemManager() {}
			// position           中心座標(基準値)   x,y,z全て+方向にマップチップで生成します。
			// randomRange        ランダムの生成範囲(片方陣地のマップサイズ)
			// mapChipSize        マップチップ一マスの大きさ
			// gemMaxNum          gemの生成数
			// seed               シード値(現在は入力しても変動なし)
			void setUp(vec3 position, vec3 randomRange, float mapChipSize, int gemMaxNum, unsigned long seed);
			void draw();
			void update();

			void create();
			// team     0がfirst,1がsecond
			void gemCountUp(int team, int it);
			void gemDelete(int it);

			vector<cGem> getGems() { return mGems; }

		private:

			vector<cGem> mGems;
			map<GemType, int> mTeamGems[2];
			vec3 mPosition;
			vec3 mRandomRange;
			float mMapChipSize;
			int mGemMaxNum;
			unsigned long mSeed;
		};
	}
}
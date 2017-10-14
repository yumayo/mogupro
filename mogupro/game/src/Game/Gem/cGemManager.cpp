#include "Game/Gem/cGemManager.h"

namespace Game
{
	namespace Gem
	{
		void cGemManager::setUp(vec3 position, vec3 randomRange, float mapChipSize,float gemScale, int gemMaxNum, unsigned long seed)
		{
			mPosition = position;
			mRandomRange = randomRange;
			mMapChipSize = mapChipSize;
			mGemScale = gemScale;
			mGemMaxNum = gemMaxNum;
			for (int i = 0; i < 2; i++)
			{
				mTeamGems[i].insert(map<GemType, int>::value_type(GemType::Dia, 0));
				mTeamGems[i].insert(map<GemType, int>::value_type(GemType::Gold, 0));
				mTeamGems[i].insert(map<GemType, int>::value_type(GemType::Silver, 0));
				mTeamGems[i].insert(map<GemType, int>::value_type(GemType::Iron, 0));
			}
			gl::Fbo::Format format;
			mGemBuffer = gl::Fbo::create(getWindowWidth(), getWindowHeight(), format.depthTexture());

			create();
			gl::enableDepthWrite();
			gl::enableDepthRead();
			gl::enable(GL_CULL_FACE);

		}

		void cGemManager::draw()
		{
			//gl::ScopedFramebuffer fbScp(mGemBuffer);
			//gl::ScopedViewport scpVp(ivec2(0), mGemBuffer->getSize());
			gl::enable(GL_TEXTURE_2D);
			gl::pushModelView();
			for each (auto g in mGems)
			{
				g.draw();
			}
			gl::color(Color(1, 1, 1));
			gl::pushModelView();


			//mGemBuffer->bindTexture();
			//gl::color(Color(1, 1, 1));
			//gl::draw(mGemBuffer->getColorTexture(), Rectf(0, 0, 10, 10));
			//mGemBuffer->unbindTexture();
		};

		void cGemManager::update()
		{

		};

		void cGemManager::create()
		{
			for (size_t i = 0; i < mGemMaxNum; i += 2)
			{
				//DeBug:本来はサーバーからもらってくる
				std::random_device seed_gen;
				std::mt19937 engine(seed_gen());
				unsigned long seed = engine();
				randSeed(seed);

				float x = randInt(0, mRandomRange.x-1);
				float y = randInt(0, mRandomRange.y-1);
				float z = randInt(0, mRandomRange.z-1);
				Game::Gem::GemType type = Game::Gem::GemType(randInt(0, Game::Gem::GemType::Iron));

				//テクスチャーの張替え
				//TexManager::geInstance()->loadTexture("~/~/gem"+type);W
				Color color = Color(1,1,1);
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

				mGems.push_back(cGem(i, (vec3(x, y, z) * mMapChipSize) +mPosition, vec3(mGemScale), color, type));
				mGems.push_back(cGem(i+1, mPosition + vec3(mRandomRange.x - x +mRandomRange.x-1 , y, mRandomRange.z-z-1) * mMapChipSize,vec3(mGemScale),color,type));
			}
		};

		
		void cGemManager::gemCountUp(int team,int it)
		{
			//指定の宝石があるか
			bool isNothing = true;
			for each(auto g in mGems)
				if (g.getId() == mGems[it].getId()) isNothing = false;
			if (isNothing) return;

			//削除
			gemDelete(it);

			//DeBug:
			console() << "チーム" << team << "が"<< GemType(it) << "を取得" << endl;
			for (int i = 0; i < 2; i++)
			{
				mTeamGems[i].at(mGems.at(it).getType())++;
				console() << "チームは" << i << endl;
				for each (auto t in mTeamGems[i])
				{
					console() << t.first << "が" << t.second << endl;
				}
			}

		}
		
		void cGemManager::gemDelete(int it)
		{
			mGems.erase(mGems.begin() + it);
		}

}
}
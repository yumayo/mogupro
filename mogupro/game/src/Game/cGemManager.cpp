#include "Game/cGemManager.h"

namespace Game
{
	void cGemManager::setUp(ci::vec3 position, ci::vec3 randomRange, float mapChipSize, float gemScale, int gemMaxNum, unsigned long seed)
	{
		mPosition = position;
		mRandomRange = randomRange;
		mMapChipSize = mapChipSize;
		mGemScale = gemScale;
		mGemMaxNum = gemMaxNum;
		for (int i = 0; i < 2; i++)
		{
			mTeamGems[i].insert(std::map<Gem::GemType, int>::value_type(Gem::GemType::Dia, 0));
			mTeamGems[i].insert(std::map<Gem::GemType, int>::value_type(Gem::GemType::Gold, 0));
			mTeamGems[i].insert(std::map<Gem::GemType, int>::value_type(Gem::GemType::Iron, 0));
			mTeamGems[i].insert(std::map<Gem::GemType, int>::value_type(Gem::GemType::Coal, 0));
		}
		ci::gl::Fbo::Format format;
		mGemBuffer = ci::gl::Fbo::create(ci::app::getWindowWidth(), ci::app::getWindowHeight(), format.depthTexture());

		create();
		ci::gl::enableDepthWrite();
		ci::gl::enableDepthRead();
		ci::gl::enable(GL_CULL_FACE);

	}

	void cGemManager::draw()
	{
		//gl::ScopedFramebuffer fbScp(mGemBuffer);
		//gl::ScopedViewport scpVp(ivec2(0), mGemBuffer->getSize());
		//ci::gl::enable(GL_TEXTURE_2D);
		ci::gl::pushModelView();
		for each (auto g in mGems)
		{
			g.draw();
		}
		ci::gl::color(ci::Color(1, 1, 1));
		ci::gl::pushModelView();


		//mGemBuffer->bindTexture();
		//ci::gl::color(ci::Color(1, 1, 1));
		//ci::gl::draw(mGemBuffer->getColorTexture(), ci::Rectf(0, 0, 10, 10));
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
			ci::randSeed(seed);

			float x = ci::randInt(0, mRandomRange.x - 1);
			float y = ci::randInt(0, mRandomRange.y - 1);
			float z = ci::randInt(0, mRandomRange.z - 1);
			Game::Gem::GemType type = Game::Gem::GemType(ci::randInt(0, Game::Gem::GemType::Iron));

			//テクスチャーの張替え
			//TexManager::geInstance()->loadTexture("~/~/gem"+type);W
			ci::Color color = ci::Color(1, 1, 1);
			switch (type)
			{
			case Game::Gem::GemType::Dia:
				color = ci::Color(0.5f, 0.5f, 0.5f);
				break;
			case Game::Gem::GemType::Gold:
				color = ci::Color(1, 1, 0);
				break;
			case Game::Gem::GemType::Coal:
				color = ci::Color(0, 0, 1);
				break;
			case Game::Gem::GemType::Iron:
				color = ci::Color(0, 1, 0);
				break;
			default:
				break;
			}

			mGems.push_back(Gem::cGem(i, (ci::vec3(x, y, z) * mMapChipSize) + mPosition, ci::vec3(mGemScale), color, type));
			mGems.push_back(Gem::cGem(i + 1, mPosition + ci::vec3(mRandomRange.x - x + mRandomRange.x - 1, y, mRandomRange.z - z - 1) * mMapChipSize, ci::vec3(mGemScale), color, type));
		}
	};


	void cGemManager::gemCountUp(int team, Gem::GemType type)
	{

		//DeBug:
		ci::app::console() << "チーム" << team << "が" << type << "を取得" << std::endl;
		for (int i = 0; i < 2; i++)
		{
			mTeamGems[i].at(mGems.at(type).getType())++;
			ci::app::console() << "チームは" << i << std::endl;
			for each (auto t in mTeamGems[i])
			{
				ci::app::console() << t.first << "が" << t.second << std::endl;
			}
		}
	}

	void cGemManager::gemDelete(int id)
	{
		std::vector<Gem::cGem>::iterator iterator = mGems.begin();
		//指定の宝石があるか
		for (int i = 0; i < mGems.size(); i++)
		{
			if (mGems[i].getId() == id) break;
			iterator++;
		}

		mGems.erase(iterator);
	}

	void cGemManager::gemReset(Gem::cGem gem)
	{
		mGems.push_back(gem);
	}
}

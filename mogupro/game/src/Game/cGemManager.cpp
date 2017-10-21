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
		create();

		ci::gl::Fbo::Format format;
		mGemBuffer = ci::gl::Fbo::create(ci::app::getWindowWidth(), ci::app::getWindowHeight(), true);
		mShader = ci::gl::GlslProg::create(ci::app::loadAsset("Gem/GemManager.vert"), ci::app::loadAsset("Gem/GemManager.frag"));
		//batch->replaceGlslProg(ci::gl::GlslProg::create(ci::app::loadAsset("Gem/GemManager.vert"), ci::app::loadAsset("Gem/GemManager.frag")));
		ci::gl::enableDepthWrite();
		ci::gl::enableDepthRead();
		ci::gl::enableAlphaBlending();
	}

	void cGemManager::draw()
	{

		for each (auto g in mGemsptr)
		{
			g->draw();
		}

		ci::gl::color(ci::Color(1, 1, 1));
	};

	void cGemManager::drawFbo()
	{
		ci::gl::ScopedGlslProg shaderScp(ci::gl::getStockShader(ci::gl::ShaderDef().texture()));
		ci::gl::ScopedTextureBind texture(mGemBuffer->getColorTexture());
		mShader->uniform("texture", 0);
		auto rect = ci::Rectf(-ci::app::getWindowWidth()/2, -ci::app::getWindowHeight()/2, ci::app::getWindowWidth() / 2, ci::app::getWindowHeight() / 2);
		ci::gl::Texture2dRef tex = mGemBuffer->getColorTexture();
	    //ci::gl::VboMeshRef vboRect = ci::gl::VboMesh::create(ci::geom::Rect(rect));
		//batch = ci::gl::Batch::create(vboRect,mShader);
		//batch->draw();
		tex->setTopDown();
		ci::gl::draw(tex,rect);
		//ci::gl::drawSolidRect(rect);
		//ci::gl::drawSolidRect(ci::Rectf(0, 0, 20, 20));
	}

	void cGemManager::update()
	{
		ci::gl::ScopedFramebuffer fbScp(mGemBuffer);
		ci::gl::clear(ci::ColorA(0,0,0,0));
		ci::gl::ScopedViewport scpVp2(ci::ivec2(0), mGemBuffer->getSize());

		ci::gl::setMatrices(CAMERA->getCamera());
		ci::gl::ScopedGlslProg shaderScp(ci::gl::getStockShader(ci::gl::ShaderDef().color()));

		for each (auto g in mGemsptr)
		{
			g->draw();
		}
		ci::gl::color(ci::Color(1, 1, 1));
	};

	void cGemManager::create()
	{

		for (int i = 0; i < mGemMaxNum; i += 2)
		{
			//DeBug:本来はサーバーからもらってくる
			std::random_device seed_gen;
			std::mt19937 engine(seed_gen());
			unsigned long seed = engine();
			ci::randSeed(seed);

			int x = ci::randInt(0, mRandomRange.x - 1);
			int y = ci::randInt(0, mRandomRange.y - 1);
			int z = ci::randInt(0, mRandomRange.z - 1);
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

			mGemsptr.push_back(std::make_shared<Gem::cGem>(i, (ci::vec3(x, y, z) * mMapChipSize) + mPosition, ci::vec3(mGemScale), color, type));
			mGemsptr.push_back(std::make_shared<Gem::cGem>(i + 1, mPosition + ci::vec3(mRandomRange.x - x + mRandomRange.x - 1, y, mRandomRange.z - z - 1) * mMapChipSize, ci::vec3(mGemScale), color, type));
		}
	};


	void cGemManager::gemCountUp(int team, Gem::GemType type)
	{

		//DeBug:
		ci::app::console() << "チーム" << team << "が" << type << "を取得" << std::endl;
		for (int i = 0; i < 2; i++)
		{
			mTeamGems[i].at(mGemsptr.at(type)->getType())++;
			ci::app::console() << "チームは" << i << std::endl;
			for each (auto t in mTeamGems[i])
			{
				ci::app::console() << t.first << "が" << t.second << std::endl;
			}
		}
	}

	void cGemManager::gemDelete(int id)
	{
		std::vector<std::shared_ptr<Gem::cGem>>::iterator iterator = mGemsptr.begin();
		bool isNothig = true;
		//指定の宝石があるか
		for (int i = 0; i < mGemsptr.size(); i++)
		{
			if (mGemsptr[i]->getId() == id)
			{
				isNothig = false;
				break;
			}
			iterator++;
		}
		if (isNothig) return;

		mGemsptr.erase(iterator);
	}
}

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
		mBloom = 1.0f;
		mSeed = seed;
		mTime = 0.0f;
		mLightingSpeed = 0.4;

		mGemBuffer = ci::gl::Fbo::create(ci::app::getWindowWidth(), ci::app::getWindowHeight(), true);
		mShader = ci::gl::GlslProg::create(ci::app::loadAsset("Gem/GemManager.vert"), ci::app::loadAsset("Gem/GemManager.frag"));
		mVboShader = ci::gl::GlslProg::create(ci::app::loadAsset("Gem/GemVbo.vert"), ci::app::loadAsset("Gem/GemVbo.frag"));
		mesh = ci::TriMesh::create(ci::TriMesh::Format().colors(4).positions().normals().texCoords0());
		create();
	}


	void cGemManager::draw()
	{

		ci::gl::draw(mGemsVbo);

		for (int i = 0; i < mFragmentGems.size(); i++)
		{
			mFragmentGems[i]->draw();
		}
		ci::gl::color(ci::Color(1, 1, 1));
	};


	void cGemManager::drawFbo()
	{
			using namespace ci;

			auto rect = ci::Rectf(-ci::app::getWindowSize() / 2, ci::app::getWindowSize() / 2);
			ci::gl::ScopedGlslProg glsl(mShader);
			ci::gl::ScopedTextureBind tex(mGemBuffer->getColorTexture());

			mShader->uniform("uTex0", 0);
			mShader->uniform("uColor", ci::vec4(1, 1, 1, 1));
			mShader->uniform("uWindowSize", ci::vec2(ci::app::getWindowSize() / 2));

			ci::gl::drawSolidRect(rect);
	}


	void cGemManager::update(float deltaTime)
	{
		mTime += deltaTime * mLightingSpeed;
		ci::gl::ScopedFramebuffer fbScp(mGemBuffer);
		ci::gl::ScopedViewport scpVp2(ci::ivec2(0), mGemBuffer->getSize());

		ci::gl::setMatrices(CAMERA->getCamera());
		ci::gl::ScopedGlslProg shaderScp(mVboShader);
		mVboShader->uniform("deltaTime", mTime);
		ci::gl::clear(ci::ColorA(0, 0, 0, 0));
		
		ci::gl::draw(mGemsVbo);
		ci::gl::color(ci::Color(1, 1, 1));
	};


	void cGemManager::create()
	{
		ci::randSeed(uint32_t(mSeed));
		for (int i = 0; i < mGemMaxNum; i += 2)
		{
			//DeBug:本来はサーバーからもらってくる
			int x = ci::randInt(0, mRandomRange.x - 1);
			int y = ci::randInt(0, mRandomRange.y - 1);
			int z = ci::randInt(0, mRandomRange.z - 1);
			float delay = ci::randFloat(0, 20);
			Game::Gem::GemType type = Game::Gem::GemType(ci::randInt(0, Game::Gem::GemType::Coal + 1));


			//テクスチャーの張替え
			ci::ColorA color = ci::Color(1, 1, 1);
			switch (type)
			{
			case Game::Gem::GemType::Dia:
				color = ci::ColorA8u(52, 152, 219);
				break;
			case Game::Gem::GemType::Gold:
				color = ci::ColorA8u(241, 196, 15);
				break;
			case Game::Gem::GemType::Coal:
				color = ci::ColorA8u(155, 89, 182);
				break;
			case Game::Gem::GemType::Iron:
				color = ci::ColorA8u(139, 195, 74);
				break;
			default:
				break;
			}
			color.a = cinder::randFloat(0,1);

			
			//oldcode-------
			mGemsptr.push_back(std::make_shared<Gem::cGemStone>(i, (ci::vec3(x, y, z) * mMapChipSize) + mPosition, ci::vec3(mGemScale), color, type, delay));
			mGemsptr.push_back(std::make_shared<Gem::cGemStone>(i + 1, mPosition + ci::vec3(mRandomRange.x - x + mRandomRange.x - 1, y, mRandomRange.z - z - 1) * mMapChipSize, ci::vec3(mGemScale), color, type, delay));
			//--------------

			mStaticGem.push_back(std::make_shared<Gem::cGemStone>(i, (ci::vec3(x, y, z) * mMapChipSize) + mPosition, ci::vec3(mGemScale), color, type, delay));
			mStaticGem.push_back(std::make_shared<Gem::cGemStone>(i + 1, mPosition + ci::vec3(mRandomRange.x - x + mRandomRange.x - 1, y, mRandomRange.z - z - 1) * mMapChipSize, ci::vec3(mGemScale), color, type, delay));
		}

		int offset = 0;
		for each (auto& g in mStaticGem)
		{
			g->setIndices(offset);
			g->setColorAs();
			g->setNomals();
			offset += 24;
		}
		buildMesh();
	};


	void cGemManager::buildMesh()
	{
		mesh->clear();
		for (int i = 0; i < mStaticGem.size(); i++)
		{
			auto indices = mStaticGem[i]->getIndices();
			mesh->appendIndices(&indices[0], indices.size());
			mesh->appendNormals(&mStaticGem[i]->getNomals()[0], mStaticGem[i]->getNomals().size());
			mesh->appendColors(&mStaticGem[i]->getColorAs()[0], mStaticGem[i]->getColorAs().size());
			ci::vec3 pos = mStaticGem[i]->getPos();
			ci::vec3 scale = mStaticGem[i]->getScale() / 2.0f;
			mesh->appendPosition(pos + ci::vec3(scale.x, scale.y, scale.z));
			mesh->appendPosition(pos + ci::vec3(scale.x, -scale.y, scale.z));
			mesh->appendPosition(pos + ci::vec3(scale.x, -scale.y, -scale.z));
			mesh->appendPosition(pos + ci::vec3(scale.x, scale.y, -scale.z));

			mesh->appendPosition(pos + ci::vec3(scale.x, scale.y, scale.z));
			mesh->appendPosition(pos + ci::vec3(scale.x, scale.y, -scale.z));
			mesh->appendPosition(pos + ci::vec3(-scale.x, scale.y, -scale.z));
			mesh->appendPosition(pos + ci::vec3(-scale.x, scale.y, scale.z));

			mesh->appendPosition(pos + ci::vec3(scale.x, scale.y, scale.z));
			mesh->appendPosition(pos + ci::vec3(-scale.x, scale.y, scale.z));
			mesh->appendPosition(pos + ci::vec3(-scale.x, -scale.y, scale.z));
			mesh->appendPosition(pos + ci::vec3(scale.x, -scale.y, scale.z));

			mesh->appendPosition(pos + ci::vec3(-scale.x, scale.y, scale.z));
			mesh->appendPosition(pos + ci::vec3(-scale.x, scale.y, -scale.z));
			mesh->appendPosition(pos + ci::vec3(-scale.x, -scale.y, -scale.z));
			mesh->appendPosition(pos + ci::vec3(-scale.x, -scale.y, scale.z));

			mesh->appendPosition(pos + ci::vec3(-scale.x, -scale.y, -scale.z));
			mesh->appendPosition(pos + ci::vec3(scale.x, -scale.y, -scale.z));
			mesh->appendPosition(pos + ci::vec3(scale.x, -scale.y, scale.z));
			mesh->appendPosition(pos + ci::vec3(-scale.x, -scale.y, scale.z));

			mesh->appendPosition(pos + ci::vec3(scale.x, -scale.y, -scale.z));
			mesh->appendPosition(pos + ci::vec3(-scale.x, -scale.y, -scale.z));
			mesh->appendPosition(pos + ci::vec3(-scale.x, scale.y, -scale.z));
			mesh->appendPosition(pos + ci::vec3(scale.x, scale.y, -scale.z));

		}
		mGemsVbo = ci::gl::VboMesh::create(*mesh);
	}


	std::shared_ptr<Gem::cGemStone> cGemManager::getGemStone(int id)
	{
		for (int i = 0; i < mStaticGem.size(); i++)
		{
			if (mStaticGem[i]->getId() == id)
			{
				return  mStaticGem[i];
			}
		}
		ci::app::console() << "This is no GemStone that has that " << id << std::endl;
		return nullptr;
	}


	std::shared_ptr<Gem::cGemStone> cGemManager::breakeGemStone(int id)
	{

		std::vector<std::shared_ptr<Gem::cGemStone>>::iterator iterator = mStaticGem.begin();
		for (int i = 0; i < mStaticGem.size(); i++)
		{
			if (mStaticGem[i]->getId() == id)
			{
				if (!mStaticGem[i]->isActive()) return nullptr;
				mFragmentGems.push_back(std::make_shared<Gem::cFragmentGem>(mFragmentGems.size(), mStaticGem[i]->getPos(), mStaticGem[i]->getScale() / 2.0f, mStaticGem[i]->getColor(),mStaticGem[i]->getType()));
				mStaticGem[i]->deleteGem();
				buildMesh();
				mStaticGem[i]->setIsActive(false);
				return  mStaticGem[i];
			}
			iterator++;
		}
		ci::app::console() << "This is no Gem that has that " << id << std::endl;
		return nullptr;
	}


	std::shared_ptr<Gem::cFragmentGem> cGemManager::getFragmentGem(int id)
	{
		for (int i = 0; i < mFragmentGems.size(); i++)
		{
			if (mFragmentGems[i]->getId() == id)
			{
				return  mFragmentGems[i];
			}
		}
		ci::app::console() << "This is no Gem that has that " << id << std::endl;
		return nullptr;
	}
}

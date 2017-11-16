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
		for (int i = 0; i < 2; i++)
		{
			mTeamGems[i].insert(std::map<Gem::GemType, int>::value_type(Gem::GemType::Dia, 0));
			mTeamGems[i].insert(std::map<Gem::GemType, int>::value_type(Gem::GemType::Gold, 0));
			mTeamGems[i].insert(std::map<Gem::GemType, int>::value_type(Gem::GemType::Iron, 0));
			mTeamGems[i].insert(std::map<Gem::GemType, int>::value_type(Gem::GemType::Coal, 0));
		}

		mGemBuffer = ci::gl::Fbo::create(ci::app::getWindowWidth(), ci::app::getWindowHeight(), true);
		mShader = ci::gl::GlslProg::create(ci::app::loadAsset("Gem/GemManager.vert"), ci::app::loadAsset("Gem/GemManager.frag"));
		mVboShader = ci::gl::GlslProg::create(ci::app::loadAsset("Gem/GemVbo.vert"), ci::app::loadAsset("Gem/GemVbo.frag"));
		mesh = ci::TriMesh::create(ci::TriMesh::Format().colors(4).positions().normals().texCoords0());
		create();
	}

	void cGemManager::draw()
	{
		ci::gl::draw(mGemsVbo);
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

	void cGemManager::update()
	{
		//sort();
		ci::gl::ScopedFramebuffer fbScp(mGemBuffer);
		ci::gl::ScopedViewport scpVp2(ci::ivec2(0), mGemBuffer->getSize());

		ci::gl::setMatrices(CAMERA->getCamera());
		ci::gl::ScopedGlslProg shaderScp(mVboShader);
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
			ci::Color color = ci::Color(1, 1, 1);
			switch (type)
			{
			case Game::Gem::GemType::Dia:
				color = ci::Color8u(52, 152, 219);
				break;
			case Game::Gem::GemType::Gold:
				color = ci::Color8u(241, 196, 15);
				break;
			case Game::Gem::GemType::Coal:
				color = ci::Color8u(155, 89, 182);
				break;
			case Game::Gem::GemType::Iron:
				color = ci::Color8u(139, 195, 74);
				break;
			default:
				break;
			}
			//oldcode-------
			mGemsptr.push_back(std::make_shared<Gem::cGem>(i, (ci::vec3(x, y, z) * mMapChipSize) + mPosition, ci::vec3(mGemScale), color, type, delay));
			mGemsptr.push_back(std::make_shared<Gem::cGem>(i + 1, mPosition + ci::vec3(mRandomRange.x - x + mRandomRange.x - 1, y, mRandomRange.z - z - 1) * mMapChipSize, ci::vec3(mGemScale), color, type, delay));
			//--------------
			mStaticGem.push_back(std::make_shared<Gem::cGem>(i, (ci::vec3(x, y, z) * mMapChipSize) + mPosition, ci::vec3(mGemScale), color, type, delay));
			mStaticGem.push_back(std::make_shared<Gem::cGem>(i + 1, mPosition + ci::vec3(mRandomRange.x - x + mRandomRange.x - 1, y, mRandomRange.z - z - 1) * mMapChipSize, ci::vec3(mGemScale), color, type, delay));
		}

		int offset = 0;
		for each (auto& g in mStaticGem)
		{
			g->setIndices(offset);
			g->setColorA();
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

	//プレイヤーが現在持っているジェムでやった方がいいかも
	void cGemManager::gemCountUp(int team, Gem::GemType type)
	{
		//ゲームマネージャーに通知
	}

	void cGemManager::gemDelete(int id)
	{
		std::vector<std::shared_ptr<Gem::cGem>>::iterator iterator = mActiveGem.begin();
		bool isNothig = true;
		//指定の宝石があるか
		for (int i = 0; i < mActiveGem.size(); i++)
		{
			if (mActiveGem[i]->getId() == id)
			{
				isNothig = false;
				break;
			}
			iterator++;
		}
		if (isNothig) return;

		mActiveGem.erase(iterator);
	}

	std::shared_ptr<Gem::cGem> cGemManager::FindGem(int id)
	{
		bool isNothig = true;
		for (int i = 0; i < mStaticGem.size(); i++)
		{
			if (mStaticGem[i]->getId() == id)
			{
				return  mStaticGem[i];
			}
		}
		ci::app::console() << "This is no Gem that has that " << id << std::endl;
		return nullptr;
	}

	std::shared_ptr<Gem::cGem> cGemManager::AcquisitionGem(int id)
	{
		std::vector<std::shared_ptr<Gem::cGem>>::iterator iterator = mStaticGem.begin();
		bool isNothig = true;
		for (int i = 0; i < mStaticGem.size(); i++)
		{
			if (mStaticGem[i]->getId() == id)
			{
				mActiveGem.push_back(mStaticGem[i]);
				//該当する□のindicesを一点に
				//std::vector<uint32_t> indices = std::vector<uint32_t>(mStaticGem[i]->getIndices().size());
				//ci::app::console() << mStaticGem[i]->getIndices().size() << std::endl;
				//auto vbo = mGemsVbo->getIndexVbo();
				//vbo->bufferSubData(mStaticGem[i]->getIndices()[0] * 4, indices.size() * 4, &indices[0]);
				//ci::app::console() << "delete" << i << std::endl;
				//mStaticGem[i]->deleteGem();
				//mStaticGem.erase(iterator);
				mStaticGem[i]->deleteGem();
				ci::app::console() << "delete" << id << std::endl;
				buildMesh();
				//mStaticGem.erase(iterator);
				return  mActiveGem[mActiveGem.size() - 1];
			}
			iterator++;
		}
		ci::app::console() << "This is no Gem that has that " << id << std::endl;
		return nullptr;
	}

	//VBOにしたらいらないかも
	void cGemManager::sort()
	{
		ci::vec3 pPos = Game::cPlayerManager::getInstance()->getActivePlayer()->getPos();
		std::sort(mStaticGem.begin(), mStaticGem.end(),
			[&](const std::shared_ptr<Gem::cGem> a, const std::shared_ptr<Gem::cGem> b)
		{
			return glm::distance(pPos, a->getPos()) < glm::distance(pPos, b->getPos());
		});

	}
}

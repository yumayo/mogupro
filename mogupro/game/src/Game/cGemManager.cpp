#include <Game/cGemManager.h>
#include <Game/cPlayerManager.h>
#include <Utility/cTimeMeasurement.h>
#include <CameraManager/cCameraManager.h>
#include <cinder/Rand.h>
#include <Resource\cSoundManager.h>
namespace Game
{

	void cGemManager::setUp(ci::vec3 position, ci::vec3 randomRange, float mapChipSize, float gemScale, int gemMaxNum, unsigned long seed)
	{
		mPosition = position;
		mRandomRange = randomRange;
		mMapChipSize = mapChipSize;
		mGemScale = gemScale;
		mGemMaxNum = gemMaxNum;
		mSeed = seed;
		mTime = 0.0f;
		mLightingSpeed = 0.4f;

		mGemBuffer = ci::gl::Fbo::create(ci::app::getWindowWidth(), ci::app::getWindowHeight(), true);
		mShader = ci::gl::GlslProg::create(ci::app::loadAsset("Gem/GemManager.vert"), ci::app::loadAsset("Gem/GemManager.frag"));
		mVboShader = ci::gl::GlslProg::create(ci::app::loadAsset("Gem/GemVbo.vert"), ci::app::loadAsset("Gem/GemVbo.frag"));
		mesh = ci::TriMesh::create(ci::TriMesh::Format().colors(4).positions().normals().texCoords());



		create();
	}


	void cGemManager::draw()
	{
		//auto con = ci::gl::context();
		//const ci::gl::GlslProg* curGlslProg = con->getGlslProg();
		//con->pushVao();
		//con->getDefaultVao()->replacementBindBegin();
		//mGemsVbo->buildVao(curGlslProg);
		//con->getDefaultVao()->replacementBindEnd();
		//con->setDefaultShaderVars();
		//   mGemsVbo->drawImpl();
		//con->popVao();

		auto ctx = ci::gl::context();
		ci::gl::ScopedVao vaoScp(ctx->getDrawTextureVao());
		ci::gl::ScopedBuffer vboScp(ctx->getDrawTextureVbo());
		ci::gl::draw(mGemsVbo);
		for (size_t i = 0; i < mFragmentGems.size(); i++)
		{
			mFragmentGems[i]->draw();
		}
		ci::gl::color(ci::Color(1, 1, 1));
	};


	void cGemManager::drawFbo()
	{
		using namespace ci;

		{
			ci::gl::pushMatrices();
			ci::gl::ScopedFramebuffer fbScp(mGemBuffer);
			ci::gl::ScopedViewport    scpVp2(ci::ivec2(0), mGemBuffer->getSize());
			ci::gl::setMatrices(CAMERA->getCamera());

			ci::gl::ScopedGlslProg    shaderScp(mVboShader);
			mVboShader->uniform("deltaTime", mTime);
			mVboShader->uniform("visibleRange", mVisibleRange);
			mVboShader->uniform("playerPos", Game::cPlayerManager::getInstance()->getActivePlayer()->getPos());

			ci::gl::clear(ci::ColorA(0, 0, 0, 0));
			for (size_t i = 0; i < mFragmentGems.size(); i++)
			{
				mFragmentGems[i]->update();
				mFragmentGems[i]->draw();
			}

			//auto con = ci::gl::context();
			//const ci::gl::GlslProg* curGlslProg = con->getGlslProg();
			//con->pushVao();
			//con->getDefaultVao()->replacementBindBegin();
			//mGemsVbo->buildVao(curGlslProg);
			//con->getDefaultVao()->replacementBindEnd();
			//con->setDefaultShaderVars();
			//mGemsVbo->drawImpl();
			//con->popVao();
			ci::gl::draw(mGemsVbo);
			ci::gl::color(ci::Color(1, 1, 1));
			ci::gl::popMatrices();
		}

		auto rect = ci::Rectf(-ci::app::getWindowSize() / 2, ci::app::getWindowSize() / 2);
		ci::gl::ScopedGlslProg    glsl(mShader);
		ci::gl::ScopedTextureBind tex(mGemBuffer->getColorTexture());

		mShader->uniform("uTex0", 0);
		mShader->uniform("uColor", ci::vec4(1, 1, 1, 1));
		mShader->uniform("uWindowSize", ci::vec2(ci::app::getWindowSize() / 2));

		ci::gl::drawSolidRect(rect);
	}


	void cGemManager::update(float deltaTime)
	{
		mTime += deltaTime * mLightingSpeed;

	};

	void cGemManager::lateUpdate(const float& delta_time)
	{
		for (size_t i = 0; i < mFragmentGems.size(); i++)
		{
			mFragmentGems[i]->lateUpdate(delta_time);
		}
	};


	void cGemManager::create()
	{
		ci::randSeed(uint32_t(mSeed));
		for (int i = 0; i < mGemMaxNum; i += 2)
		{

			int x = ci::randInt(0, int32_t(mRandomRange.x - 1));
			int y = ci::randInt(0, int32_t(mRandomRange.y - 1));
			int z = ci::randInt(0, int32_t(mRandomRange.z - 1));
			float delay = ci::randFloat(0, 20);
			Gem::GemType type = Game::Gem::GemType(ci::randInt(0, Game::Gem::GemType::Coal + 1));


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

			color.a = cinder::randFloat(0, 1);

			mGemStone.push_back(std::make_shared<Gem::cGemStone>(i, (ci::vec3(x, y, z) * mMapChipSize) + mPosition, ci::vec3(mGemScale), color, type));
			mGemStone.push_back(std::make_shared<Gem::cGemStone>(i + 1, mPosition + ci::vec3(mRandomRange.x - x + mRandomRange.x - 1, y, mRandomRange.z - z - 1) * mMapChipSize, ci::vec3(mGemScale), color, type));
		}

		int offset = 0;
		for each (auto& g in mGemStone)
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
		for (size_t i = 0; i < mGemStone.size(); i++)
		{
			auto indices = mGemStone[i]->getIndices();
			ci::vec3 pos = mGemStone[i]->getPos();
			ci::vec3 scale = mGemStone[i]->getScale() / 2.0f;

			mesh->appendIndices(&indices[0], indices.size());
			mesh->appendNormals(&mGemStone[i]->getNomals()[0], mGemStone[i]->getNomals().size());
			mesh->appendColors(&mGemStone[i]->getColorAs()[0], mGemStone[i]->getColorAs().size());

			std::vector<cinder::vec2> texCoords;
			for (int i = 0; i < 24; ++i) { texCoords.emplace_back(cinder::vec2(0, 0)); }
			mesh->appendTexCoords0(texCoords.data(), texCoords.size());

			//Vert
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


	void cGemManager::clearMesh()
	{
		if (mesh == nullptr) return;
		mesh->clear();
	}

	std::shared_ptr<Gem::cGemStone> cGemManager::getGemStone(int id)
	{
		for (size_t i = 0; i < mGemStone.size(); i++)
		{
			if (mGemStone[i]->getId() == id)
			{
				return  mGemStone[i];
			}
		}

		ci::app::console() << "This is no GemStone that has that " << id << std::endl;
		return nullptr;
	}


	std::vector<std::shared_ptr<Gem::cFragmentGem>> cGemManager::breakGemStone(int id)
	{
		std::vector<std::shared_ptr<Gem::cFragmentGem>> addGems;
		for (size_t i = 0; i < mGemStone.size(); i++)
		{
			if (mGemStone[i]->getId() == id)
			{
				if (!mGemStone[i]->isActive())
				{
					ci::app::console() << "This is no GemStone that has that " << id << std::endl;
					return addGems;
				}

				auto& Se = Resource::cSoundManager::getInstance()->findSe("Gem/stonebreak.wav");

				Se.play();

				ci::vec3 dir;
				ci::vec3 offset;
				for (int j = 0; j < mCreateFGemNum; j++)
				{
					dir.x = ci::randInt(-6, 6);
					dir.z = ci::randInt(-6, 6);
					dir.y = 10;
					offset.x = ci::randFloat(-0.8, 0.8);
					offset.y = ci::randFloat(-0.8, 0.8);
					offset.z = ci::randFloat(-0.8, 0.8);
					mFragmentGems.push_back(std::make_shared<Gem::cFragmentGem>(mFragGemIDCount, mGemStone[i]->getPos() + offset, mGemStone[i]->getScale() / 2.0f, mGemStone[i]->getColor(), mGemStone[i]->getType(), glm::normalize(dir)));
					mFragmentGems[mFragmentGems.size() - 1]->setup();
					addGems.push_back(mFragmentGems[mFragmentGems.size() - 1]);
					mFragGemIDCount++;
				}
				mGemStone[i]->deleteGem();
				buildMesh();
				mGemStone[i]->setIsActive(false);
				return  addGems;
			}
		}

		ci::app::console() << "This is no GemStone that has that " << id << std::endl;
		return addGems;
	}


	std::shared_ptr<Gem::cFragmentGem> cGemManager::getFragmentGem(int id)
	{
		for (size_t i = 0; i < mFragmentGems.size(); i++)
		{
			if (mFragmentGems[i]->getId() == id)
			{
				return  mFragmentGems[i];
			}
		}

		ci::app::console() << "This is no FragmentGem that has that " << id << std::endl;
		return nullptr;
	}


	void  cGemManager::deleteFragmentGem(int id)
	{
		std::vector<std::shared_ptr<Gem::cFragmentGem>>::iterator iterator = mFragmentGems.begin();
		for (size_t i = 0; i < mFragmentGems.size(); i++)
		{
			if (mFragmentGems[i]->getId() == id)
			{
				mFragmentGems.erase(iterator);
				return;
			}
			iterator++;
		}

		ci::app::console() << "This is no FragmentGem that has that " << id << std::endl;
		return;
	};

	void  cGemManager::deleteFragmentGems(std::vector<std::shared_ptr<Gem::cFragmentGem>> gems)
	{
		for (size_t g = 0; g < gems.size(); g++)
		{
			size_t i = 0;
			for (i = 0; i < mFragmentGems.size(); i++)
			{
				if (mFragmentGems[i]->getId() == gems[g]->getId())
				{
					mFragmentGems.erase(mFragmentGems.begin() + i);
					break;
				}
			}
			if (i == mFragmentGems.size())
			{
				ci::app::console() << "This is no FragmentGem that has that " << gems[g]->getId() << std::endl;
				break;
			}
		}
	}
}
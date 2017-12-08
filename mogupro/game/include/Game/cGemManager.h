#pragma once
#include <memory>
#include <vector>
#include <map>
#include <cinder/app/RendererGl.h>
#include <cinder/gl/Texture.h>
#include <cinder/gl/Fbo.h>
#include <cinder/gl/GlslProg.h>
#include <Utility/cSingletonAble.h>
#include <Game/Gem/cGemStone.h>
#include <Game/Gem/cFragmentGem.h>
#define GemManager Game::cGemManager::getInstance()



namespace Game
{
	class cGemManager : public Utility::cSingletonAble<cGemManager>
	{
	public:

		cGemManager() {}
		~cGemManager() {}
		// position           中心座標(基準値)   x,y,z全て+方向にマップチップで生成します。
		// randomRange        ランダムの生成範囲(片方陣地のマップサイズ)
		// mapChipSize        マップチップ一マスの大きさ
		// gemScale           ジェム一個の大きさ
		// gemMaxNum          gemの生成数
		// seed               シード値(現在は入力しても変動なし)
		void      setUp(ci::vec3 position, ci::vec3 randomRange, float mapChipSize, float gemScale, int gemMaxNum, unsigned long seed);
		void      draw();
		void      drawFbo();
		void      update(float deltaTime);
		void      lateUpdate(const float& delta_time);

		void      create();
		void      buildMesh();
		void      clearMesh();

		std::vector<std::shared_ptr<Gem::cGemStone>>         getGemStones()          { return mGemStone; }
		std::vector<std::shared_ptr<Gem::cFragmentGem>>      getFragmentGems()       { return mFragmentGems; }
		std::shared_ptr<Gem::cGemStone>                      getGemStone(int id);
		std::vector<std::shared_ptr<Gem::cFragmentGem>>      breakGemStone(int id);
		std::shared_ptr<Gem::cFragmentGem>                   getFragmentGem(int id);
		void  deleteFragmentGem(int id);
		void  deleteFragmentGems(std::vector<std::shared_ptr<Gem::cFragmentGem>> gems);

	private:


		const int mDrawNum = 100;

		std::vector<std::shared_ptr<Gem::cGemStone>>      mGemStone;
		std::vector<std::shared_ptr<Gem::cFragmentGem>>   mFragmentGems;

		ci::vec3             mPosition;
		ci::vec3             mRandomRange;
		float                mMapChipSize;
		float                mGemScale;
		int                  mGemMaxNum;
		ci::gl::FboRef       mGemBuffer;
		ci::gl::GlslProgRef  mShader;
		ci::gl::GlslProgRef  mVboShader;
		ci::TriMeshRef       mesh;
		ci::gl::VboMeshRef   mGemsVbo;
		unsigned long        mSeed;
		float                mTime;
		float                mLightingSpeed;
		const float          mCreateFGemNum = 3;
		
		//ci::gl::GlslProgRef mHShader;
		//ci::gl::GlslProgRef mVShader;
	};
}
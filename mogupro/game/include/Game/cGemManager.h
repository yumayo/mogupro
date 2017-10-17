#pragma once
#include "cinder/Rand.h"
#include "cinder/gl/Fbo.h"
#include "Game/Gem/cGem.h"
#include "Utility/cSingletonAble.h"
#include "cinder/app/RendererGl.h"
#include "CameraManager\cCameraManager.h"
#include <vector>
#include <map>

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
    void setUp(ci::vec3 position, ci::vec3 randomRange, float mapChipSize, float gemScale, int gemMaxNum, unsigned long seed );
    void draw();
    void update();

    void create();
    // team     0がfirst,1がsecond
    void gemCountUp( int team, Gem::GemType type);
    void gemDelete( int it );
	void gemReset(Gem::cGem gem);
    std::vector<Gem::cGem> getGems() { return mGems; }

private:

    std::vector<Gem::cGem> mGems;
    std::map<Gem::GemType, int> mTeamGems[2];
    ci::vec3 mPosition;
    ci::vec3 mRandomRange;
    float mMapChipSize;
    float mGemScale;
    int mGemMaxNum;
    ci::gl::FboRef mGemBuffer;
    unsigned long mSeed;
};

}
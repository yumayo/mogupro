#pragma once
#include <cinder/app/App.h>
#include <cinder/gl/gl.h>
#include <Utility/cUserPointer.hpp>
#include <Game/Light/cPointLightParam.h>
#include <Utility/cSingletonAble.h>

namespace Particle
{
enum class ParticleType
{
    NONE,
    SCATTER,    // 飛び散り
    EXPROTION,  // 爆発
    GLITTER,    // キラキラ
    CONVERGE,   // 収束
};

enum class ParticleTextureType
{
    NONE,
    SAND,
    SPARK,
};

class ParticleParam
{
public:
    friend class cParticleHolder;

    // デフォルトの設定
    ParticleParam();

    // 生成の中心位置
    ParticleParam& position( const ci::vec3& position );
    // パーティクルのスケール
    ParticleParam& scale( const float& scale );
    // パーティクルの移動の仕方
    ParticleParam& moveType( const ParticleType& move_type );
    // パーティクルの画像の種類
    ParticleParam& textureType( const ParticleTextureType& texture_type );
    // 色
    ParticleParam& color( const ci::ColorA& color );
    // 一フレームでパーティクルが生成される数
    ParticleParam& count( const int& count );
    // 消滅時間
    ParticleParam& vanishTime( const float& vanish_time );
    // 消滅時間のランダムの範囲
    ParticleParam& vanishTimeRange( const float& vanish_time_range );
    // 消滅し始める時間
    ParticleParam& vanishBeginTime( const float& vanish_begin_time );
    // 生成時間 ( time < 0 になると生成終了 )
    ParticleParam& effectTime( const float& effect_time );
    // パーティクルの移動速度
    ParticleParam& speed( const float& speed );
    // ベクトルの追加量 ( ベクトルに偏りを持たせる )
    ParticleParam& addVec( const ci::vec3& add_vec );
    // 重力の強さ ( vec.y - gravity )
    ParticleParam& gravity( const float& gravity );
    // パーティクルの効果範囲　( moveType == GLITTER or CONVERGE )
    ParticleParam& effectRange( const float& effect_range );
    // 生成の中心位置にライトを生成するかどうか
    ParticleParam& isLighting( const bool& is_lighting = true );
    // パーティクルの軌跡を生成するかどうか
    ParticleParam& isTrajectory( const bool& is_trajectory = true );
    // パーティクルをCubeにするかどうか
    ParticleParam& isCube( const bool& is_cube = true );

private:

    ci::vec3 mPosition;
    float mScale;
    ParticleType mMoveType;
    ParticleTextureType mTextureType;
    ci::ColorA mColor;
    int mCount;
    float mVanishTime;
    float mVanishTimeRange;
    float mVainshBeginTime;
    float mEffectTime;
    float mSpeed;
    ci::vec3 mAddVec;
    float mGravity;
    float mEffectRange;
    float mEaseTime;
    bool mIsLighting;
    bool mIsTrajectory;
    bool mIsCube;

};

class cParticle
{
public:

    cParticle( const ci::vec3& position,
               const ci::vec3& vec,
               const float& time,
               const float& vanish_begin_time );
    ~cParticle();

    void update( const float& delta_time, const float& gravity );
    void draw( const glm::quat& rotation, const ci::ColorA& color );
    void cubeDraw( const ci::ColorA& color );
    bool isActive();

private:

    void alphaUpdate(const float& delta_time);
    float getAlpha();

public:

    ci::vec3 mPosition;
    ci::vec3 mPrevPosition;
    ci::vec3 mVec;
    float mTime;
    float mVanishBeginTime;
    float mAlpha;
    int mTrajectoryCount;
};

class cParticleHolder
{
public:

    cParticleHolder( const ParticleParam& param );
    cParticleHolder( const ci::vec3& position,
                     const ParticleType& type,
                     const ParticleTextureType& texture_type,
                     const float& scale,
                     const float& time,
                     const int& count,
                     const float& speed,
                     const bool& lighting,
                     const ci::ColorA& color );
    ~cParticleHolder();

    void update( const float& delta_time );
    void draw( const glm::quat& rotation );

    bool isActive();

private:

    void createParticle();
    ci::vec3 createPosition();
    ci::vec3 createVec( const ci::vec3& particle_position );
    float createVanishTime();
    void createTrajectory( const ci::vec3& position, const float& vanish_time, const float& delta_time );
    void particleDraw( const glm::quat& rotation );
    void setTexture( const ParticleTextureType& texture_type );
    void setLight( bool is_lighting );

public:

    ParticleParam mParam;
    std::string mTextureName;
    Utility::softptr<Game::Light::cPointLightParam> mHandle;
    std::vector<std::shared_ptr<cParticle>> mParticles;
    std::deque<std::shared_ptr<cParticle>> mTrajectoryParticles;

    ci::TriMeshRef mMesh;
    ci::gl::VboMeshRef mVbo;

};

// /* 使い方 */
// /* コンストラクタ法 */
//
// Particle::cParticleManager::getInstance()->
//     create( vec3( 0, get_map_top_pos, 0 ),         // 生成位置の中心
//             Particle::ParticleType::SCATTER,       // 飛ばし方
//             Particle::ParticleTextureType::SPARK,  // 画像の種類
//             100.0f,                                // 生成時間
//             10,                                    // 生成個数
//             0.1f,                                  // 玉の速度 (内部でrandomをしている
//             false,                                 // ライトのenable
//             color);                                // カラー
//

// /* 変更したいパラメーターだけ変更していくチェーン記法 */
//
// Particle::cParticleManager::getInstance()->
//     create( Particle::ParticleParam()
//             .position( mCollider.getPosition()
//             .scale( vec3( 0.5f ) ) );    // 生成位置の中心
//

class cParticleManager : public Utility::cSingletonAble<cParticleManager>
{
public:

    cParticleManager();
    ~cParticleManager();

    void setup();
    void update( const float& delta_time );
    void draw();

    // パーティクル生成する
    // param : ParticleParam
    void create( const ParticleParam& param );

    void create( const ci::vec3& position,
                 const ParticleType& type = ParticleType::EXPROTION,
                 const ParticleTextureType& texture_type = ParticleTextureType::SPARK,
                 const float& scale = 0.3f,
                 const float& time = 5.0f,
                 const int& count = 10,
                 const float& speed = 0.1f,
                 const bool& lighting = false,
                 const ci::ColorA& color = ci::ColorA( 1, 1, 1, 1 ) );

    void create( const ci::vec3& position,
                 const ParticleType& type = ParticleType::EXPROTION,
                 const ParticleTextureType& texture_type = ParticleTextureType::SPARK,
                 const ci::vec3& scale = ci::vec3( 0.3f ),
                 const float& time = 5.0f,
                 const int& count = 10,
                 const float& speed = 0.1f,
                 const bool& lighting = false,
                 const ci::ColorA& color = ci::ColorA( 1, 1, 1, 1 ) );

private:

    void builbordUpdate();

private:

    std::vector<std::shared_ptr<cParticleHolder>> mParticleHolders;
    glm::quat mBuilbordRotate;

};
}

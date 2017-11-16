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
    SCATTER,    // 重力無しの飛び散り
    SPLASH,     // 重力ありの飛び散り
    EXPROTION,  // 爆発
    GLITTER,    // 動きなしキラキラ
};

enum class ParticleTextureType
{
    NONE,
    SAND,
    SPARK,
};

class cParticle
{
public:

    cParticle() {}
    cParticle( const ci::vec3& vec,
               const ci::vec3& position,
               const float& time );

    void update( const float& delta_time );
    bool isActive();

public:
    ci::vec3 mPosition;
    ci::vec3 mVec;
    float mTime;
};

class cParticleHolder
{
public:

    cParticleHolder( const ci::vec3& position,
                     const ParticleType& type,
                     const ParticleTextureType& texture_type,
                     const ci::vec3& scale,
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

    void sort();
    void create( const ci::vec3& position,
                 const float& time );
    void particleDraw( const glm::quat& rotation );

public:

    ci::vec3 mPosition;
    ci::vec3 mScale;
    ParticleType mType;
    std::string mTextureName;
    ci::ColorA mColor;
    float mTime;
    float mSpeed;
    bool mLighting;
    Utility::softptr<Game::Light::cPointLightParam> mHandle;
    std::vector<std::shared_ptr<cParticle>> mParticles;
};

// /*使い方*/
// Particle::cParticleManager::getInstance()->
//     create( vec3( 0, get_map_top_pos, 0 ),         // 生成位置の中心
//             Particle::ParticleType::SCATTER,       // 飛ばし方
//             Particle::ParticleTextureType::SPARK,  // 画像の種類
//             100.0f,                                // 生成時間
//             10,                                    // 生成個数
//             0.1f,                                  // 玉の速度 (内部でrandomをしている
//             false,                                 // ライトのenable
//             color);                                // カラー

class cParticleManager : public Utility::cSingletonAble<cParticleManager>
{
public:

    cParticleManager();
    ~cParticleManager();

    void setup();
    void update( const float& delta_time );
    void draw();

    // パーティクル生成する
    // position     : 生成位置 
    // type         : 飛ばす種類
    // texture_type : 画像の種類
    // scale        : スケール
    // count        : 生成個数
    // speed        : パーティクルの速さ
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

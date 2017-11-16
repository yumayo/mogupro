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
               const float& time);

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
                     const float& time,
                     const int& count );
    ~cParticleHolder();

    void update( const float& delta_time );
    void draw( const glm::quat& rotation );

    bool isActive();

private:

    void sortByCamera();
    void create( const ci::vec3& position,
                 const float& time );

private:

    ci::vec3 mPosition;
    ParticleType mType;
    Utility::softptr<Game::Light::cPointLightParam> mHandle;
    std::vector<std::shared_ptr<cParticle>> mParticles;
    std::string mTextureName;
    float mTime;
    ci::gl::TextureRef mTexture;
};

class cParticleManager : public Utility::cSingletonAble<cParticleManager>
{
public:

    cParticleManager();
    ~cParticleManager();

    void setup();
    void update( const float& delta_time );
    void draw();

    // パーティクル生成
    // position     : 生成位置 
    // type         : 飛ばす種類
    // texture_type : 画像の種類
    // time         : 生成時間
    void create( const ci::vec3& position,
                 const ParticleType& type,
                 const ParticleTextureType& texture_type,
                 const float& time );

    // パーティクル生成する
    // position     : 生成位置 
    // type         : 飛ばす種類
    // texture_type : 画像の種類
    // count        : 生成個数
    void create( const ci::vec3& position,
                 const ParticleType& type,
                 const ParticleTextureType& texture_type,
                 const float& time,
                 const int& count );

private:

    void builbordUpdate();

private:

    std::vector<std::shared_ptr<cParticleHolder>> mParticleHolders;
    glm::quat mBuilbordRotate;

};
}
